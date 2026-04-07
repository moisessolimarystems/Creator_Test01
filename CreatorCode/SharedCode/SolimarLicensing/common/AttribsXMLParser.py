# AttribsXMLParser.py
# SLB 11.jun.2024 CR.32662.V03; SPDE's V4 imported and converted to work in the Licensing 
# environment.

from AttribsOutput import *
import re,string,xml.dom.minidom
import copy
import time                                                                                        # CR.30781; added.
import sys                                                                                         # CR.30781; added.

# class XmlDefinitionError:
# Attributes xml formatting error exceptions.
# Base class XmlDefintionError does most of the work.
#
# SLB 24.sep.2021 CR.30781; created.
class XmlDefinitionError(Exception):
   class_name = ''
   element_type = "unknown"
   element_name = ''
   problem = ''
   typedef_type = ''

   def __init__(self, class_name, element_type, element_name, typedef_type, error):
      self.class_name = class_name
      self.element_type = element_type
      self.element_name = element_name
      self.typedef_type = typedef_type
      self.problem = error

   def ToText(self):
      if len(self.typedef_type) == 0:
         return 'Problem with Xml definition for class <%s>. ElementName <%s>  ElementType <%s>. Details: %s' % (self.class_name, self.element_name, self.element_type, self.problem)
      else:
         return 'Problem with Xml definition for class <%s>. ElementName <%s>  ElementType <%s> TypedefType <%s>. Details: %s' % (self.class_name, self.element_name, self.element_type, self.typedef_type, self.problem)


# class MemberDefinitionError: 
# is raised when there is an attrib member xml formatting error
#
# SLB 24.sep.2021 CR.30781; created.
class MemberDefinitionError(XmlDefinitionError):
   def __init__(self, class_name, element_name, error):
      XmlDefinitionError.__init__(self, class_name, 'Member', element_name, '', error)

# class ClassDefinitionError:
# is raised when for an AttribsClass or AttribsClassDerived xml formatting error
#
# SLB 24.sep.2021 CR.30781; created.
class ClassDefinitionError(XmlDefinitionError):
   def __init__(self, class_name, element_name, error):
      XmlDefinitionError.__init__(self, class_name, 'Class', element_name, '', error)

# TypedefDefinitionError is raised when for a typedef xml formatting error
#
# SLB 24.sep.2021 CR.30781; created.
class TypedefDefinitionError(XmlDefinitionError):
   def __init__(self, class_name, element_name, typedef_type, error):
      XmlDefinitionError.__init__(self, class_name, 'TypeDef', element_name, typedef_type, error)

# debug and script performance helper classes.
# Useful for issues which are not just syntactical in nature (where exception tells you
# everything you need to know.

# global managed by the CDebugMsg object. This variable is used to manage indentation
# of printed message text.
g_debug_msg_lvl = 0

# class CDebugMsg:
# Base class "worker" class. This class should never be instantiated directly...
# Instead, use a derived class, depending on need.
# This class handles basic formatting and indentation of the debug messages.
#
# SLB 24.sep.2021 CR.30781; created.
class CDebugMsg():
   # constructor will print a "start" message if display_mode is 0.
   def __init__(self, display_text, display_mode):
      #needs to update g_debug_msg_lvl
      global g_debug_msg_lvl
      self.b_completed = False
      self.display_mode = display_mode
      self.spaces = self.make_spaces(g_debug_msg_lvl * 2)
      g_debug_msg_lvl = g_debug_msg_lvl + 1
      self.start = time.clock() * 1000
      self.display_text = display_text
      if self.display_mode == 0: print '%sstarted %s' % (self.spaces, self.display_text)

   # Use this to reset the start time to the current time stamp.
   def reset(self):
      if self.b_completed: return

      self.start = time.clock() * 1000
      if self.display_mode == 0: print '%sstarted %s' % (self.spaces, self.display_text)

   # complete() must be called to get a performance metrics message for the debug object.
   # This will generate a "complete" message depending upon the object's display_mode value.
   #    display_mode == 0: Always generates a print message.
   #    display_mode == 1: Only generates a print message if time since "start" exceeds 1000msecs.
   #    display_mode == 2: no print message generated.
   # Part of printed message is the calcualted elapsed time since "start" value.
   def complete(self):
      #if complete() call has already been made, exit immediately.
      if self.b_completed == True: return

      # indicate, for future calls, that complete() has been called.
      self.b_completed = True
      
      #needs to update g_debug_msg_lvl
      global g_debug_msg_lvl
      g_debug_msg_lvl = g_debug_msg_lvl - 1

      elapsed_time = self.elapsed_time()
      flag_msg = ''
      if elapsed_time > 10000:
         flag_msg = 'crazy high'
      elif elapsed_time > 1000:
         flag_msg = 'extremely high'
      elif elapsed_time > 100:
         flag_msg = 'high'
      if self.display_mode == 0: print '%scomplet %s  %s (ms) %s' % (self.spaces, self.display_text, elapsed_time, flag_msg)
      elif self.display_mode == 1 and elapsed_time > 1000:  print '%scomplet %s  %s (ms) %s' % (self.spaces, self.display_text, elapsed_time, flag_msg)

   # destructor: ensures cleanup happens if needed.
   def __del__(self):
      self.complete()
   
   # timer_message() can be used to generate a print message with the given text.
   # mesage will be formatted to be indented at the same level as other messages from 
   # this object, as well as output the elapsed time since "start".
   def timer_message(self, text):
      if self.display_mode == 2: return

      print '%scomplet %s  %s (ms)' % (self.spaces, text, self.elapsed_time())

   # elapsed_time() returns the elapsed time in milliseconds since start (int form)
   def elapsed_time(self):
      return time.clock()*1000 - self.start

   # make_spaces() helper method used by the class to generate an indentation string
   # of the specified length.
   def make_spaces(self, n):
      spaces = ''
      while n > 0:
         spaces = spaces + ' '
         n = n - 1
      return spaces

# This debugging class is used to track beginning and ending of execution for a method or section of 
# script.  
# Client code must call the complete() method to get the "end" message. 
#
# SLB 24.sep.2021 CR.30781; created.
class CDebugExecuteTime(CDebugMsg):
   # The b_enabled parm is useful for code which gets constantly executed, but only
   # wants a print debug message generated when a trigger is encountered.
   def __init__(self, method_name, display_text, b_enabled):
      text = '%s() %s' % (method_name, display_text)

      display_mode = 0
      if not b_enabled: display_mode = 2
      
      CDebugMsg.__init__(self, text, display_mode)


# This debugging class is used to only generate a debug print message if excecution time 
# exceeds the flagged value (curreint 1000msecs).
#
# SLB 24.sep.2021 CR.30781; created.
class CPerfTimer(CDebugMsg):
   def __init__(self, method_name, display_text):
      text = 'Timer for %s() %s' % (method_name, display_text)
      CDebugMsg.__init__(self, text, 1)




# outputs the c++ version of the derived attribs class header files
# SLB 22.mar.2017 CR.23181; added support for list(xmlstring) type.
class AttribsXMLParser:
   def __init__(self):
      self.collection_types = {
         'list(unsigned __int32)':  {'type': 'List', 'value_type': 'unsigned __int32', 'default_type_name': 'List_DWORD' },
         'list(string)':            {'type': 'List', 'value_type': 'string', 'default_type_name': 'List_string' },
         'list(byte[])':            {'type': 'List', 'value_type': 'byte[]', 'default_type_name': 'List_ByteArray' },
         'list(xmlstring)':         {'type': 'List', 'value_type': 'xmlstring', 'default_type_name': 'List_xmlstring' },                        # CR.23181; added.
         'map(unsigned __int32, unsigned __int32)':  {'type': 'Map', 'key_type': 'unsigned __int32', 'value_type': 'unsigned __int32', 'default_type_name': 'Map_DWORDDWORD' },
         'map(unsigned __int32, string)':            {'type': 'Map', 'key_type': 'unsigned __int32', 'value_type': 'string', 'default_type_name': 'Map_DWORDstring'  },
         'map(string, unsigned __int32)':            {'type': 'Map', 'key_type': 'string', 'value_type': 'unsigned __int32', 'default_type_name': 'Map_stringDWORD'  },
         'map(string, string)':                      {'type': 'Map', 'key_type': 'string', 'value_type': 'string', 'default_type_name': 'Map_stringstring'  },
         }


   # ClassToTypeDef() - translates an "AttribsClass" object into a "Typedef" object.
   # This allows the XML Parser to normalize the top level AttribsClass as a Typedef.
   #
   # SLB 24.sep.2021 CR.30781; created.
   def ClassToTypeDef(self, class_attribs, shallow_form):
      typedef_attribs = {}
      name =  class_attribs['name']

      typedef_attribs['name'] = name
      typedef_attribs['type'] = class_attribs['type']
      typedef_attribs['enum'] = None
      typedef_attribs['collection'] = None
      typedef_attribs['target_cpp_only'] = None
      typedef_attribs['typedef_type'] = 'class'
      typedef_attribs['inner_class'] = {}
      typedef_attribs['inner_class']['code_class_name'] = class_attribs['code_class_name']
      typedef_attribs['inner_class']['base_class'] = class_attribs['base_class']

      #Note: a deeper copy would assign the class_attribs to typedef_attribs['inner_class']
      return typedef_attribs

   # ParseXML() - Top level method which takes an xml DOM object and processes it into 
   # local objects which output handlers know what to do with.
   #
   # SLB 17.aug.2018 CR.17862.V08; Added var predefined_types
   # SLB 24.sep.2021 CR.30781; Now initializes pre-defined type with global_class definitions
   # before iterating through current xmlroot.
   # Moved filename assignment from ParseXMLClass() to here because ParseXMLClass() is now used
   # generically to handle ALL class defintions. It can do that because global classes have been
   # converted to typedef objects and are stored in the predefined_types object.
   def ParseXML(self, xmlroot, global_classes):
      #timer = CDebugExecuteTime('ParseXML', '', True)

      # initialize predefined types with values from global_classes.
      predefined_types = {}                                                                        # CR.17862.V08; added.

      # CR.30781; global objects will exist from previously processed attribs xml files.
      # Convert/add all current global objects into the predefined_types array so that those 
      # definitions will be available in a normalized format.
      for name, global_class in global_classes.items():
         # Add a very top level entry for the class entry because that is all
         # that is needed to reference this class by future classes.
         predefined_types[name] = self.ClassToTypeDef(global_class, 1)

      # for each top level attribs class (in the current attribs xml file)
      for attribs_class_xml in getChildElementsByTagName(xmlroot,'AttribsClass'):
         try:                                                                                      # CR.30781; added try/except.
            cls = self.ParseXMLClass(attribs_class_xml, 0, global_classes, predefined_types, '')   # CR.17862.V08; modified.
            name = cls['name']                                                                     # CR.30781; added.
            # if filename is not specified, generate one from the class name.
            if len(cls['filename']) == 0: cls['filename'] = "%sAttribs" % (name)                   # CR.30781; added.

            # update global_classes and predefined_types with this top level class before
            # next iteration.
            global_classes[name] = cls
            predefined_types[name] = self.ClassToTypeDef(cls, 1)

            # compute the includes of other classes needed by this class (when creating a derived 
            # class is currently when this happens)
            self.ParseXMLComputeClassIncludes(cls, cls, global_classes)
         except ClassDefinitionError as e:                                                         # CR.30781; added try/except.
            print ''
            print >> sys.stderr, e.ToText()
            print 'Compiler processing will continue excluding this class.'

   # ParseXMLClass() - method used to convert xml definitions for all classes (derived or not) into 
   # objects that are useable by the output emitters.       
   # Param Notes: 
   # inner_class:      is a bool that indicates whether this is a "sub-class" or a top level class.
   # global_classes:   are "top level" attribs classes which are defined in other files 
   # predefined_types: are types (INCLUDING subclasses) which are defined in this class's current
   #                   scope.
   # code_namespace:   is a string which represents the current namespace context. It is formatted
   #                   in C++ syntax. e.g. namespace1::namspace2::class1
   #                   It does NOT contain the current class name.
   #
   # SLB 17.aug.2018 CR.17862.V08; added predefined_types: {attrib_type, type}   e.g {"CJobList", "class"}
   # SLB 20.sep.2018 CR.18557.V01; added cpp_pragma_disable_warnings. If set, is expected to be a 
   # list of numbers seperated by spaces e.g. 4136 4534 this should only be used by the CPP 
   # generator script.
   # SLB 28.nov.2018 CR.7968; Improved support of base_class within an 'outer' class definition.
   # SLB 24.sep.2021 CR.30781; 'derived_base' is no longer a valid key in the attributes class.
   # Now raises custom exceptions for certain class and member definition errors encountered.
   # Also, catches these custom exception and displayes error messages.
   # Moved code for filename logic to ParseXML().
   # Attributes output changes:
   #  1) removed ['derived_base']
   #  2) added ['class_namespace'] - Purpose of this is to help give reference to error messages 
   #     when spitting out 'class_name'... instead messages can spit out 'class_namespace' which
   #     will give the rooted namespace of the current scope.
   #  3) added ['base_class_code_name'] - Purpose of this is to help Output emitters when they
   #     need to distinguish between the base class name tag and the actual base class code output 
   #     name. If this were not here, the output emitters would have to cache previously defined
   #     definitions so that it could do a look-up to find the base class code name.
   #  
   def ParseXMLClass(self, class_xml, inner_class, global_classes, predefined_types, code_namespace):
      name = class_xml.getAttribute('name')
      if len(name) == 0: raise ClassDefinitionError(code_namespace, 'n/a', 'No class name specified for this attribute class.')

      attributes = {}
      attributes['type'] = 'class'                                                                 # CR.30781; added.
      attributes['name'] = class_xml.getAttribute('name')
      attributes['descriptive_name'] = class_xml.getAttribute('descriptive_name')
      attributes['base'] = class_xml.getAttribute('base')
      attributes['filename'] = class_xml.getAttribute('filename')
      attributes['code_class_name'] = class_xml.getAttribute('code_class_name')
      attributes['tagname'] = ''
      attributes['comments'] = ''
      attributes['typedefs'] = {}
      attributes['members'] = {}
      attributes['class_namespace'] = code_namespace                                               # CR.30781; added.
      attributes['subtype'] = class_xml.getAttribute('subtype')
      attributes['module_id'] = class_xml.getAttribute('module_id')
      attributes['includes'] = []
      attributes['alternate_license_class'] = class_xml.getAttribute('alternate_license_class')
      attributes['interfaces'] = {}
      attributes['base_class_interfaces'] = {}
      attributes['cpp_linkage'] = class_xml.getAttribute('cpp_linkage')
      attributes['cpp_pragma_disable_warnings'] = class_xml.getAttribute('cpp_pragma_disable_warnings') # CR.18557.V01; added.
      attributes['base_class'] = class_xml.getAttribute('base_class')   
      attributes['base_class_type'] = class_xml.getAttribute('base_class_type')   
      attributes['tagname'] = class_xml.getAttribute('tagname')
      attributes['predefined_types'] = predefined_types;                                           # CR.17862.V08; added.
      attributes['base_class_code_name'] = ''                                                      # CR.30781; added.

      # If there is a base class, gather appropriate info.
      if len(attributes['base_class']) != 0:
         base_class_name = attributes['base_class']
         # Update the predefined types to include those from the base class.
         # CR.30781 Note: this code supercedes CR.7968 code for this.
         # Check for global_class definition first, it stores its base class info differently than
         # a "typedef" class.
         if base_class_name in global_classes:
            # found in global classses.
            attributes['predefined_types'].update(global_classes[base_class_name]['predefined_types'])
            attributes['predefined_types'].update(global_classes[base_class_name]['typedefs'])
            attributes['base_class_interfaces'].update(global_classes[base_class_name]['interfaces'])
            attributes['base_class_code_name'] = global_classes[base_class_name]['code_class_name']
         elif base_class_name in attributes['predefined_types']:
            base_class_typedefs = attributes['predefined_types'][base_class_name]['inner_class']['typedefs']
            attributes['predefined_types'].update(base_class_typedefs)
            attributes['base_class_code_name'] = attributes['predefined_types'][base_class_name]['inner_class']['code_class_name']
         else:
            # base class definition does not exist.. Critical error.
            raise ClassDefinitionError('%s::%s' % (code_namespace, name), 'base_class', 'No definition exists for the specified base class: %s' % (base_class_name)); # CR.30781; added.
         
      # calculate attribs output code class name (if this is a top-level class).
      if len(attributes['code_class_name']) == 0:
         if (not inner_class):
            attributes['code_class_name'] = "%sAttribs" % (attributes['name'],)
         else:
            attributes['code_class_name'] = attributes['name']

      # get the comments
      try:
         node = getChildElementsByTagName(class_xml,'comments')[0]
         for child in node.childNodes:
            # if child is text node
            if (child.nodeType == 3):
               attributes['comments'] += child.data

      except:
         attributes['comments'] = ''
      
      # Compute and assign the code namespace for this attribs class. 
      # This variable is used exclusively to generate more helpful attrib compiler error messages.
      new_code_namespace = attributes['code_class_name']                                           # CR.30781; added.
      if len(code_namespace):                                                                      # CR.30781; added.
         new_code_namespace = '%s::%s' % (code_namespace, attributes['code_class_name'])           # CR.30781; added.

      # parse the typedefs
      # Make a copy of the predefined_types. This class_type_list will get updated with every type added and will be passed
      # along to ParseXMLTypedef() every iteration. It is merely the info needed by the class to declare and initialize a 
      # member of this type. 
      class_type_list = predefined_types                                                           # CR.17862.V08; added.
      i = 0
      for typedef_xml in getChildElementsByTagName(class_xml,'typedef'):
         type_name = typedef_xml.getAttribute('name')                                              # CR.17862.V08; added.
         type_def = self.ParseXMLTypedef(typedef_xml, global_classes, class_type_list, new_code_namespace) # CR.17862.V08; modified.
         attributes['typedefs'][type_name] = type_def
         attributes['typedefs'][type_name]['output_order'] = i                                     # CR.17862.V08; modified.
         i += 1
         # CR.17862.V08; add this type to the known class_type_list. 
         # Note: since these are considered "predefined" types, all of the info is not really 
         # needed, but it is convenient to just pass it along.
         class_type_list[type_name] = type_def                                                     # CR.17862.V08; added.

      # add special attribute "ToolTipInfo" to all modules
      if (attributes['base'] == 'module'):
         toolTipName = 'toolTipInfo'
         parsed = {}
         parsed['name'] = toolTipName
         parsed['type'] = 'string'
         parsed['tagname'] = 'ToolTipInfo'
         parsed['default'] = ''
         parsed['sec_permit_read'] = ''
         parsed['sec_permit_write'] = ''
         parsed['depends_on_cpp'] = ''
         parsed['description'] = ''
         parsed['res_pkg_options_desired'] = ''
         parsed['res_validation_options'] = ''
         parsed['res_runtime_options'] = ''
         attributes['members'][toolTipName] = parsed
         attributes['members'][toolTipName]['output_order'] = i
         i += 1
      
      # parse/add the attrib members
      # CR.30781; If there is a problem with an attrib definition, an error message is printed 
      # and the xml continues on to the next attrib. This is to help develper "see" more errors
      # when compiling the script.
      for attrib_xml in getChildElementsByTagName(class_xml,'attrib'):
         try:                                                                                      # CR.30781; added try/except.
            name = attrib_xml.getAttribute('name')
            if len(name) == 0: raise MemberDefinitionError(new_code_namespace, 'Not defined', 'No \'name\' specified for this attribute.') # CR.30781; added.

            type = attrib_xml.getAttribute('type')
            if len(type) == 0: raise MemberDefinitionError(new_code_namespace, name, 'No \'type\' was specified for this attribute') # CR.30781; added.

            tagname = attrib_xml.getAttribute('tagname')
            if len(tagname) == 0: raise MemberDefinitionError(new_code_namespace, name, 'No \'tagname\' was specified for this attribute') # CR.30781; added.

            if (type in self.collection_types):
               typename = self.GetCollectionTypeName(type)
               # These typenames must be autogenerated by script... If it has yet to be defined
               # add it now.
               if (typename not in class_type_list):
                  attributes['typedefs'][typename] = self.CreateTypeForCollection(typename, type)
                  attributes['typedefs'][typename]['output_order'] = i
            attributes['members'][name] = self.ParseXMLAttribute(attrib_xml, global_classes)
            attributes['members'][name]['output_order'] = i
            attributes['members'][name]['derived'] = False
            i += 1
         except MemberDefinitionError as e:                                                        # CR.30781; added try/except.
            print ''
            print >> sys.stderr, e.ToText()
            print 'Compiler processing will continue.'

      # parse the interface variables
      for interface in getChildElementsByTagName(class_xml,'interface'):
         attributes['interfaces'][interface.getAttribute('name')] = self.ParseXMLInterface(interface, global_classes)
         attributes['interfaces'][interface.getAttribute('name')]['output_order'] = i
         i += 1
      
      return attributes

   # ParseXMLTypedef()
   #
   # SLB 17.aug.2018 CR.17862.V08; added predefined_types parm.
   # SLB 24.sep.2021 CR.30781; Removed 'derived_class' key from attributes output.
   # Modified handling of 'derived' types to use same code as 'class' types. Both use 
   # ParseXMLClass() now  
   def ParseXMLTypedef(self, typedef_xml, global_classes, predefined_types, code_namespace):
      attributes = {}
      attributes['name'] = typedef_xml.getAttribute('name')
      attributes['type'] = typedef_xml.getAttribute('type')
      attributes['enum'] = None
      attributes['inner_class'] = None
      attributes['collection'] = None
      attributes['target_cpp_only'] = typedef_xml.getAttribute('target_cpp_only')
      if len(attributes['target_cpp_only']) == 0:
         attributes['target_cpp_only'] = 'True'

      #typedef = {}         # {name, type, enum, inner_class, derived_class}
      
      if (attributes['type'] == 'enum'):
         attributes['enum'] = []
         max_order = 0
         for enum in getChildElementsByTagName(typedef_xml,'enum'):
            order = max_order+1
            if (len(enum.getAttribute('order'))==0):
               max_order += 1
            else:
               order = int(enum.getAttribute('order'))
               if (order>max_order):
                  max_order = order
            alias = enum.getAttribute('name')
            if (len(enum.getAttribute('alias'))>0):
               alias = enum.getAttribute('alias')
            attributes['enum'].append({'name': enum.getAttribute('name'), 'value': enum.getAttribute('value'), 'alias': alias, 'order': order})
      elif (attributes['type'] == 'class' or attributes['type'] == 'derived'):                     # CR.30781; modified.
         for attribs_class_xml in getChildElementsByTagName(typedef_xml,'AttribsClass'):
            attributes['inner_class'] = self.ParseXMLClass(attribs_class_xml, 1, global_classes, predefined_types, code_namespace)  # CR.17862.V08; modified.
            if (len(attributes['inner_class']['tagname'])==0):
               attributes['inner_class']['tagname'] = attributes['inner_class']['name']
         for attribs_class_xml in getChildElementsByTagName(typedef_xml,'AttribsClassDerived'):
            try:                                                                                   # CR.30781; Added try/except.
               attributes['inner_class'] = self.ParseXMLClass(attribs_class_xml, 1, global_classes, predefined_types, code_namespace)  # CR.17862.V08; modified.
               if (len(attributes['inner_class']['tagname'])==0):
                  attributes['inner_class']['tagname'] = attributes['inner_class']['name']
            except ClassDefinitionError as e:                                                      # CR.30781; Added try/except.
               print ''
               print >> sys.stderr, e.ToText()
               print 'Compiler processing will continue excluding this sub-class.'
      elif (attributes['type'] == 'collection'):
         for collection in getChildElementsByTagName(typedef_xml,'List'):
            attributes['collection'] = {'type': 'List', 'value_type': collection.getAttribute('value_type')}
            break
         for collection in getChildElementsByTagName(typedef_xml,'SlimList'):
            attributes['collection'] = {'type': 'SlimList', 'value_type': collection.getAttribute('value_type')}
            break
         for collection in getChildElementsByTagName(typedef_xml,'Map'):
            attributes['collection'] = {'type': 'Map', 'key_type': collection.getAttribute('key_type'), 'value_type': collection.getAttribute('value_type')}
            break
         for collection in getChildElementsByTagName(typedef_xml,'SlimMap'):
            attributes['collection'] = {'type': 'SlimMap', 'key_type': collection.getAttribute('key_type'), 'value_type': collection.getAttribute('value_type')}
            break
      else:
         print "Unknown typedef: %s %s " % (attributes['name'], attributes['type'])
      return attributes

   def CreateTypeForCollection(self, type_name, collection_type):
      attributes = {}
      if len(type_name) == 0:
         type_name = self.GetCollectionTypeName(collection_type)
      attributes['name'] = type_name
      attributes['enum'] = None
      attributes['inner_class'] = None
      attributes['type'] = 'collection'
      attributes['collection'] = self.collection_types[collection_type]

      return attributes

   def GetCollectionTypeName(self, collection_type):
      type_name = collection_type
      if collection_type in  self.collection_types:
         type_name = self.collection_types[collection_type]['default_type_name']
      return type_name

   # 
   # SLB 12.jul.2012 CR.15891.V02; Added support for new "depends_on" attribute   
   # SLB 21.aug.2012 CR.15891.V02; Added support for new "msg_alias" attribute
   # SLB 21.aug.2012 CR.15891.V04; Added support for new resource type attributes.
   # renamed the depends_on member to depends_on_cpp, for clarification.
   # SLB 21.jul.2015 CR.18952; renamed XML attribute msg_alias to description.
   #
   def ParseXMLAttribute(self, attrib_xml, known_classes):
      attributes = {}
      attributes['name'] = attrib_xml.getAttribute('name')
      type = attrib_xml.getAttribute('type')
      attributes['type'] = self.GetCollectionTypeName(type)
      #attributes['type'] = attrib_xml.getAttribute('type')

      attributes['tagname'] = attrib_xml.getAttribute('tagname')
      attributes['numencoding'] = attrib_xml.getAttribute('numencoding')
      attributes['default'] = attrib_xml.getAttribute('default')
      attributes['depends_on_cpp'] = attrib_xml.getAttribute('depends_on_cpp')                     #CR.15891.V02; added.
      attributes['description'] = attrib_xml.getAttribute('description')                           #CR.18952; modified. CR.15891.V02; added.
      attributes['res_pkg_options_desired'] = attrib_xml.getAttribute('res_pkg_options_desired')   #CR.15891.V04; added.
      attributes['res_validation_options'] = attrib_xml.getAttribute('res_validation_options')     #CR.15891.V04; added.
      attributes['res_runtime_options'] = attrib_xml.getAttribute('res_runtime_options')           #CR.15891.V04; added.
      attributes['target_cpp_only'] = attrib_xml.getAttribute('target_cpp_only')
      if (len(attributes['target_cpp_only'])==0):
         attributes['target_cpp_only'] = 'True'
      return attributes

   def ParseXMLInterface(self, interface_xml, known_classes):
      interface  = {}

      interface['name'] = interface_xml.getAttribute('name')
      interface['type'] = interface_xml.getAttribute('type')
      interface['tagname'] = interface_xml.getAttribute('tagname')
      interface['numencoding'] = 'hex'
      interface['numencoding'] = interface_xml.getAttribute('numencoding')
      interface['default'] = interface_xml.getAttribute('default')
      interface['depends_on_cpp'] = interface_xml.getAttribute('depends_on_cpp')                     #CR.15891.V02; added.
      interface['description'] = interface_xml.getAttribute('description')                           #CR.18952; modified. CR.15891.V02; added.
      interface['res_pkg_options_desired'] = interface_xml.getAttribute('res_pkg_options_desired')   #CR.15891.V04; added.
      interface['res_validation_options'] = interface_xml.getAttribute('res_validation_options')     #CR.15891.V04; added.
      interface['res_runtime_options'] = interface_xml.getAttribute('res_runtime_options')           #CR.15891.V04; added.
      #interface['base_class'] = interface_xml.getAttribute('base_class')   
      return interface
   
   # ParseXMLComputeClassIncludes() 
   # This is a recursive call meant to walk the class definition tree to figure out what include
   # files are needed at the top level.
   #
   # SLB 24.sep.2021 CR.30781; Re-worked. Method is more robust now. Now it will walk the class's 
   # typedef tree regardless of whether it is a derived class or not (before it would only do this 
   # if it was NOT a derived class).
   # No longer presumes that the base class is in an external file... Now checks that it is defined
   # in the globl_classes object first. 
   # If it is the top level class, it requires that the base class be defined in the global list,
   # otherwise it throws an exception.
   # Finally, it ensures that only one instance, of the same include file, gets added to the top
   # level class's include list.
   def ParseXMLComputeClassIncludes(self, top_level_class, attribs_class, global_classes):

      # First check the top-level of this class to see if a base_class is specified.
      # If so, it must exist in the global_classes list.
      b_toplevel = False
      if top_level_class['name'] == attribs_class['name']: b_toplevel = True

      base_class_name = attribs_class['base_class']
      if b_toplevel and len(base_class_name) != 0:
         if b_toplevel and base_class_name not in global_classes: raise ClassDefinitionError(attribs_class['name'], 'base_class', 'base class %s has not been defined' % (base_class_name))

      # if base class is in in global_classes, look up the filename and add it to the includes 
      # list.
      if base_class_name in global_classes:
         file_name = global_classes[base_class_name]['filename']
         # only add it if it is not already there.
         if len(file_name) and file_name not in top_level_class['includes']:
            top_level_class['includes'].append(file_name)

      # attribs_class has been checked. Now walk it's typedefs to see if there is a sub-class
      # which has a base class which is "global" (externally defined)
      for typedef in attribs_class['typedefs'].values():
         if typedef['type'] == 'class' or typedef['type'] == 'derived':
            # make recursive call for this class definition.
            self.ParseXMLComputeClassIncludes(top_level_class, typedef['inner_class'], global_classes)

#         if (typedef['type'] == 'derived'): 
#            base_class_name = typedef['inner_class']['base_class']
#            file_name = ""
#            # Only need file name for base classes in global list. i.e. not a subclass that has
#            # been inherited.
#            # does base class exist in global list?
#            if (base_class_name in global_classes):
#               file_name = global_classes[base_class_name]['filename']
#
#            # Update includes list... but only if the needed filename is not there already.
#            if (len(file_name) and file_name not in top_level_class['includes']):
#               top_level_class['includes'].append(file_name)
#
#         if (typedef['type'] == 'class' or typedef['type'] == 'derived'):
#            self.ParseXMLComputeClassIncludes(top_level_class, typedef['inner_class'], global_classes)
   
   
  