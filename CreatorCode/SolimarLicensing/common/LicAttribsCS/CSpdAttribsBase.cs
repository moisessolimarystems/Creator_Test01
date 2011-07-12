using System;
using System.Collections;
using System.Collections.Specialized;

namespace Solimar
{
    namespace Licensing
	{
		namespace Attribs
		{
			public abstract class AttribsMemberBase : System.ICloneable
			{
				public AttribsMemberBase()
				{
					;
				}
				public AttribsMemberBase(string keyName, Object defaultVal)
				{
					m_KeyName = keyName;
					m_tVal = defaultVal;
				}
				
				public override abstract string ToString();
				protected abstract void AssignFromString(string s);
				protected virtual void AssignFromXmlNode(System.Xml.XmlNode node)
				{
					AssignFromString(node.InnerXml);
				}
                protected virtual void AssignFromObject(object ovalue)
                {
                    OVal = ovalue;
                    //AssignFromString(ovalue.ToString());
                }
				public string SVal
				{
					get {return this.ToString();}
					set {AssignFromString(value);}
				}
				
				public object OVal
				{
					get {return m_tVal;}
					set {m_tVal = value;}
				}
				
				public System.Xml.XmlNode XVal
				{
					get {return null;}
					set {AssignFromXmlNode(value);}
				}
                public object ZVal
                {
                    get {return null;}
                    set {AssignFromObject(value);}
                }
								
				public static implicit operator string(AttribsMemberBase t)
				{
					return t.ToString();
				}
				
				public string GetKeyName()
				{
					return m_KeyName;
				}

				protected string m_KeyName;
				protected Object m_tVal;

				#region ICloneable Members
				
				public object Clone()
				{
					object n = this.GetType().GetConstructor(new System.Type[]{}).Invoke(new object[]{});
					((AttribsMemberBase)n).SVal = this.SVal;
					return n;
				}
				
				#endregion
			}
			
			public class AttribsMemberString : AttribsMemberBase
			{
				public AttribsMemberString(string keyName, string defaultVal) : 
					base(keyName, defaultVal)
				{
					;
				}
				public override string ToString()
				{
					return AttribFormat.ConvertRawStringToString((string)m_tVal);
				}
				protected override void AssignFromString(string s)
				{
					m_tVal = AttribFormat.ConvertStringToRawString(s);
				}
				
				public static implicit operator string(AttribsMemberString t)
				{
					return (string)t.m_tVal;
				}
				
				public string TVal
				{
					get {return this;}
					set {m_tVal = (Object)value;}
				}
			}

			public class AttribsMemberQWORD : AttribsMemberBase
			{
				public AttribsMemberQWORD(string keyName, UInt64 defaultVal) : 
					base(keyName, defaultVal)
				{
					;
				}
				public override string ToString()
				{
					return AttribFormat.ConvertUInt64ToString((UInt64)m_tVal);
				}
				protected override void AssignFromString(string s)
				{
					m_tVal = AttribFormat.ConvertStringToUInt64(s);
				}
				
				public static implicit operator UInt64(AttribsMemberQWORD t)
				{
					return (UInt64)t.m_tVal;
				}
				
				public UInt64 TVal
				{
					get {return this;}
					set {m_tVal = (Object)value;}
				}
			}

			public class AttribsMemberDWORD : AttribsMemberBase
			{
				public AttribsMemberDWORD(string keyName, UInt32 defaultVal) : 
					base(keyName, defaultVal)
				{
					;
				}
				public override string ToString()
				{
					return AttribFormat.ConvertUInt32ToString((UInt32)m_tVal);
				}
				protected override void AssignFromString(string s)
				{
					m_tVal = AttribFormat.ConvertStringToUInt32(s);
				}
				public static implicit operator UInt32(AttribsMemberDWORD t)
				{
					return (UInt32)t.m_tVal;
				}
				
				public UInt32 TVal
				{
					get {return this;}
					set {m_tVal = (Object)value;}
				}
			}

			public class AttribsMemberWORD : AttribsMemberBase
			{
				public AttribsMemberWORD(string keyName, UInt16 defaultVal) : 
					base(keyName, defaultVal)
				{
					;
				}
				public override string ToString()
				{
					return AttribFormat.ConvertUInt16ToString((UInt16)m_tVal);
				}
				protected override void AssignFromString(string s)
				{
					m_tVal = AttribFormat.ConvertStringToUInt16(s);
				}
				public static implicit operator UInt16(AttribsMemberWORD t)
				{
					return (UInt16)t.m_tVal;
				}
				
				public UInt16 TVal
				{
					get {return this;}
					set {m_tVal = (Object)value;}
				}
			}

			public class AttribsMemberBYTE : AttribsMemberBase
			{
				public AttribsMemberBYTE(string keyName, Byte defaultVal) : 
					base(keyName, defaultVal)
				{
					;
				}
				public override string ToString()
				{
					return AttribFormat.ConvertByteToString((Byte)m_tVal);
				}
				protected override void AssignFromString(string s)
				{
					m_tVal = AttribFormat.ConvertStringToByte(s);
				}
				public static implicit operator Byte(AttribsMemberBYTE t)
				{
					return (Byte)t.m_tVal;
				}
				
				public Byte TVal
				{
					get {return this;}
					set {m_tVal = (Object)value;}
				}
			}

			public class AttribsMemberQINT : AttribsMemberBase
			{
				public AttribsMemberQINT(string keyName, Int64 defaultVal) : 
					base(keyName, defaultVal)
				{
					;
				}
				public override string ToString()
				{
					return AttribFormat.ConvertInt64ToString((Int64)m_tVal);
				}
				protected override void AssignFromString(string s)
				{
					m_tVal = AttribFormat.ConvertStringToInt64(s);
				}
				public static implicit operator Int64(AttribsMemberQINT t)
				{
					return (Int64)t.m_tVal;
				}
				
				public Int64 TVal
				{
					get {return this;}
					set {m_tVal = (Object)value;}
				}
			}

			public class AttribsMemberINT : AttribsMemberBase
			{
				public AttribsMemberINT(string keyName, Int32 defaultVal) : 
					base(keyName, defaultVal)
				{
					;
				}
				public override string ToString()
				{
					return AttribFormat.ConvertInt32ToString((Int32)m_tVal);
				}
				protected override void AssignFromString(string s)
				{
					m_tVal = AttribFormat.ConvertStringToInt32(s);
				}
				public static implicit operator Int32(AttribsMemberINT t)
				{
					return (Int32)t.m_tVal;
				}
				
				public Int32 TVal
				{
					get {return this;}
					set {m_tVal = (Object)value;}
				}
			}

			public class AttribsMemberSINT : AttribsMemberBase
			{
				public AttribsMemberSINT(string keyName, Int16 defaultVal) : 
					base(keyName, defaultVal)
				{
					;
				}
				public override string ToString()
				{
					return AttribFormat.ConvertInt16ToString((Int16)m_tVal);
				}
				protected override void AssignFromString(string s)
				{
					m_tVal = AttribFormat.ConvertStringToInt16(s);
				}
				public static implicit operator Int16(AttribsMemberSINT t)
				{
					return (Int16)t.m_tVal;
				}
				
				public Int16 TVal
				{
					get {return this;}
					set {m_tVal = (Object)value;}
				}
			}

			public class AttribsMemberCHAR : AttribsMemberBase
			{
				public AttribsMemberCHAR(string keyName, SByte defaultVal) : 
					base(keyName, defaultVal)
				{
					;
				}
				public override string ToString()
				{
					return AttribFormat.ConvertSByteToString((SByte)m_tVal);
				}
				protected override void AssignFromString(string s)
				{
					m_tVal = AttribFormat.ConvertStringToSByte(s);
				}
				public static implicit operator SByte(AttribsMemberCHAR t)
				{
					return (SByte)t.m_tVal;
				}
				
				public SByte TVal
				{
					get {return this;}
					set {m_tVal = (Object)value;}
				}
			}

			public class AttribsMemberBOOL : AttribsMemberBase
			{
				public AttribsMemberBOOL(string keyName, bool defaultVal) : 
					base(keyName, defaultVal)
				{
					;
				}
				public override string ToString()
				{
					return AttribFormat.ConvertBoolToString(this);
				}
				protected override void AssignFromString(string s)
				{
					m_tVal = AttribFormat.ConvertStringToBool(s);
				}
				public static implicit operator bool(AttribsMemberBOOL t)
				{
					return (bool)t.m_tVal;
				}
				
				public bool TVal
				{
					get {return this;}
					set {m_tVal = (Object)value;}
				}
			}
			
			public class AttribsMemberFloat : AttribsMemberBase
			{
				public AttribsMemberFloat(string keyName, float defaultVal) : 
					base(keyName, defaultVal)
				{
					;
				}
				public override string ToString()
				{
					return AttribFormat.ConvertFloatToString((float)m_tVal);
				}
				protected override void AssignFromString(string s)
				{
					m_tVal = AttribFormat.ConvertStringToFloat(s);
				}
				public static implicit operator float(AttribsMemberFloat t)
				{
					return (float)t.m_tVal;
				}
				
				public float TVal
				{
					get {return this;}
					set {m_tVal = (Object)value;}
				}
			}
			
			public class AttribsMemberDouble : AttribsMemberBase
			{
				public AttribsMemberDouble(string keyName, double defaultVal) : 
					base(keyName, defaultVal)
				{
					;
				}
				public override string ToString()
				{
					return AttribFormat.ConvertDoubleToString((double)m_tVal);
				}
				protected override void AssignFromString(string s)
				{
					m_tVal = AttribFormat.ConvertStringToDouble(s);
				}
				public static implicit operator double(AttribsMemberDouble t)
				{
					return (double)t.m_tVal;
				}
				
				public double TVal
				{
					get {return this;}
					set {m_tVal = (Object)value;}
				}
			}
			
			public class AttribsMemberDateTime : AttribsMemberBase
			{
				public AttribsMemberDateTime(string keyName, DateTime defaultVal) : 
					base(keyName, defaultVal)
				{
					;
				}
				public override string ToString()
				{
					return AttribFormat.ConvertDateTimeToString((DateTime)m_tVal);
				}
				protected override void AssignFromString(string s)
				{
					m_tVal = AttribFormat.ConvertStringToDateTime(s);
				}
				public static implicit operator DateTime(AttribsMemberDateTime t)
				{
					return (DateTime)t.m_tVal;
				}
				
				public DateTime TVal
				{
					get {return this;}
					set {m_tVal = (Object)value;}
				}
			}
			
			public class AttribsMemberEnum : AttribsMemberBase
			{
				public AttribsMemberEnum(string keyName, System.Enum defaultVal, System.Type enum_t) : 
					base(keyName, defaultVal)
				{
					m_enum_t = enum_t;
				}
				public override string ToString()
				{
					return System.Enum.Format(m_enum_t, m_tVal, "x");
				}
				protected override void AssignFromString(string s)
				{
					m_tVal = System.Enum.Parse(m_enum_t, AttribFormat.ConvertStringToUInt32(s).ToString());
				}
            protected override void AssignFromObject(object ovalue)
            {
               m_tVal = System.Enum.Parse(m_enum_t, ovalue.ToString());
            }

				public static implicit operator UInt32(AttribsMemberEnum t)
				{
					return Convert.ToUInt32(t.ToString(),16);
				}
				public static implicit operator System.Enum(AttribsMemberEnum t)
				{
					return (System.Enum)t.m_tVal;
				}

				public virtual string GetEnumAlias(System.Enum eval)
				{
					return eval.ToString("g");
				}

				public virtual System.Enum GetEnumFromAlias(string alias)
				{
					return (System.Enum)System.Enum.Parse(m_enum_t, alias);
				}

				public virtual int GetEnumOrdering(System.Enum eval)
				{
					return Convert.ToInt32(eval);
				}
				
				public System.Enum EVal
				{
					get {return (System.Enum)m_tVal;}
					set {m_tVal = value;}
				}
				
				public System.Type EnumType
				{
					get {return m_enum_t;}
				}
				
				private System.Type m_enum_t = null;
			}
			
			public class AttribsMemberAttribsClass : AttribsMemberBase
			{
				public AttribsMemberAttribsClass(string keyName, LicensingAttribsBase defaultVal) : 
					base(keyName, defaultVal)
				{
					;
				}
				public override string ToString()
				{
					return m_tVal.ToString();
				}
				protected override void AssignFromString(string s)
				{
					((LicensingAttribsBase)m_tVal).AssignMembersFromStream(s);
				}
				protected override void AssignFromXmlNode(System.Xml.XmlNode node)
				{
					((LicensingAttribsBase)m_tVal).AssignMembersFromXmlNode(node.FirstChild);
				}
                protected override void AssignFromObject(object o)
                {
                    //o represents two-demensional array, stored in VALUE
                    ((LicensingAttribsBase)m_tVal).AssignMembersFromObject(o);
                }
				
				public static implicit operator LicensingAttribsBase(AttribsMemberAttribsClass t)
				{
					return (LicensingAttribsBase)t.m_tVal;
				}
			}
			
			public class AttribsMemberDWORDList : AttribsMemberBase
			{
				public AttribsMemberDWORDList(string keyName, ArrayList defaultVal) : 
					base(keyName, defaultVal)
				{
					;
				}
				public override string ToString()
				{
					System.IO.StringWriter string_writer = new System.IO.StringWriter();
					System.Xml.XmlTextWriter xml_writer = new System.Xml.XmlTextWriter(string_writer);
					
					// write the class start tag
					xml_writer.WriteStartElement("Vector");
					
					// write a text node so that we don't get a class tag that looks like <tag/> and instead we get <tag></tag> (the c++ version of the attribs does not use a legitimate xml parser)
					xml_writer.WriteWhitespace(" ");
					
					foreach (UInt32 o in (ArrayList)m_tVal)
					{
						xml_writer.WriteElementString("N",String.Format("{0:x}",(UInt32)o));
					}
					
					// write the class close tag
					xml_writer.WriteEndElement();
					
					return string_writer.ToString();
				}
				protected override void AssignFromString(string s)
				{
					if (s.Length == 0) return;
					AssignFromXmlNode(XMLDocumentHelper.GetDocumentElementFromString(s));
				}
				protected override void AssignFromXmlNode(System.Xml.XmlNode node)
				{
					((ArrayList)m_tVal).Clear();
					foreach (System.Xml.XmlNode child in node.FirstChild.ChildNodes)
					{
						((ArrayList)m_tVal).Add(Convert.ToUInt32(child.InnerXml,16));
					}
				}
                protected override void AssignFromObject(object ovalue)
                {
                    ((ArrayList)m_tVal).Clear();
                    Object[] values = (Object[])ovalue;
                    for (int idx=0; idx<values.Length; idx++)
                    {
                        ((ArrayList)m_tVal).Add(values[idx]);
                    }
                }
				public static implicit operator ArrayList(AttribsMemberDWORDList t)
				{
					return (ArrayList)t.m_tVal;
				}
				
				public ArrayList TVal
				{
					get {return this;}
					set {m_tVal = (Object)value;}
				}
			}

			public class AttribsMemberStringList : AttribsMemberBase
			{
				public AttribsMemberStringList(string keyName, ArrayList defaultVal) : 
					base(keyName, defaultVal)
				{
					;
				}
				public override string ToString()
				{
					System.IO.StringWriter string_writer = new System.IO.StringWriter();
					System.Xml.XmlTextWriter xml_writer = new System.Xml.XmlTextWriter(string_writer);
					
					// write the class start tag
					xml_writer.WriteStartElement("Vector");
					
					// write a text node so that we don't get a class tag that looks like <tag/> and instead we get <tag></tag> (the c++ version of the attribs does not use a legitimate xml parser)
					xml_writer.WriteWhitespace(" ");
					
					foreach (string o in (ArrayList)m_tVal)
					{
						xml_writer.WriteElementString("N",o);
					}
					
					// write the class close tag
					xml_writer.WriteEndElement();
					
					return string_writer.ToString();
				}
				protected override void AssignFromString(string s)
				{
					if (s.Length == 0) return;
					AssignFromXmlNode(XMLDocumentHelper.GetDocumentElementFromString(s));
				}
				protected override void AssignFromXmlNode(System.Xml.XmlNode node)
				{
					((ArrayList)m_tVal).Clear();
					//foreach (System.Xml.XmlNode child in node.ChildNodes)
					if (node.FirstChild != null)
					{
						foreach (System.Xml.XmlNode child in node.FirstChild.ChildNodes)
						{
							((ArrayList)m_tVal).Add(child.InnerText);
						}
					}
				}
                protected override void AssignFromObject(object ovalue)
                {
                    ((ArrayList)m_tVal).Clear();
                    ((ArrayList)m_tVal).AddRange((System.Collections.ICollection)ovalue);
                }

				public static implicit operator ArrayList(AttribsMemberStringList t)
				{
					return (ArrayList)t.m_tVal;
				}
				
				public ArrayList TVal
				{
					get {return this;}
					set {m_tVal = (Object)value;}
				}
			}

			public class AttribsMemberBufferList : AttribsMemberBase
			{
				public AttribsMemberBufferList(string keyName, ArrayList defaultVal) : 
					base(keyName, defaultVal)
				{
					;
				}
				public override string ToString()
				{
					System.IO.StringWriter string_writer = new System.IO.StringWriter();
					System.Xml.XmlTextWriter xml_writer = new System.Xml.XmlTextWriter(string_writer);
					
					// write the class start tag
					xml_writer.WriteStartElement("Vector");
					
					// write a text node so that we don't get a class tag that looks like <tag/> and instead we get <tag></tag> (the c++ version of the attribs does not use a legitimate xml parser)
					xml_writer.WriteWhitespace(" ");
					
					foreach (AttribsMemberBuffer o in (ArrayList)m_tVal)
					{
						xml_writer.WriteStartElement("N");
						xml_writer.WriteRaw(o.ToString());
						xml_writer.WriteEndElement();
					}
					
					// write the class close tag
					xml_writer.WriteEndElement();
					
					return string_writer.ToString();
				}
				protected override void AssignFromString(string s)
				{
					if (s.Length == 0) return;
					AssignFromXmlNode(XMLDocumentHelper.GetDocumentElementFromString(s));
				}
				protected override void AssignFromXmlNode(System.Xml.XmlNode node)
				{
					((ArrayList)m_tVal).Clear();
					foreach (System.Xml.XmlNode child in node.FirstChild.ChildNodes)
					{
						AttribsMemberBuffer b = new AttribsMemberBuffer("", new byte[]{});
						b.SVal = child.InnerXml;
						((ArrayList)m_tVal).Add(b);
					}
				}
                protected override void AssignFromObject(object ovalue)
                {
                    //Not implemented
                }

				public static implicit operator ArrayList(AttribsMemberBufferList t)
				{
					return (ArrayList)t.m_tVal;
				}
				
				public ArrayList TVal
				{
					get {return this;}
					set {m_tVal = (Object)value;}
				}
			}
			
			public class AttribsMemberDWORDtoDWORDMap : AttribsMemberBase
			{
				public AttribsMemberDWORDtoDWORDMap(string keyName, SortedList defaultVal) : 
					base(keyName, defaultVal)
				{
					;
				}
				public override string ToString()
				{
					System.IO.StringWriter string_writer = new System.IO.StringWriter();
					System.Xml.XmlTextWriter xml_writer = new System.Xml.XmlTextWriter(string_writer);
					
					// write the class start tag
					xml_writer.WriteStartElement("Map");
					
					// write a text node so that we don't get a class tag that looks like <tag/> and instead we get <tag></tag> (the c++ version of the attribs does not use a legitimate xml parser)
					xml_writer.WriteWhitespace(" ");
					
					foreach (DictionaryEntry i in (SortedList)m_tVal)
					{
						xml_writer.WriteStartElement("N");
						xml_writer.WriteElementString("K",String.Format("{0:x}",(UInt32)i.Key));
						xml_writer.WriteElementString("V",String.Format("{0:x}",(UInt32)i.Value));
						xml_writer.WriteEndElement();
					}
					
					// write the class close tag
					xml_writer.WriteEndElement();
					
					return string_writer.ToString();
				}
				protected override void AssignFromString(string s)
				{
					if (s.Length == 0) return;
					AssignFromXmlNode(XMLDocumentHelper.GetDocumentElementFromString(s));
				}
				protected override void AssignFromXmlNode(System.Xml.XmlNode node)
				{
					((SortedList)m_tVal).Clear();
					foreach (System.Xml.XmlNode child in node.FirstChild.ChildNodes)
					{
						object key = Convert.ToUInt32(child.ChildNodes[0].InnerXml,16);
						object val = Convert.ToUInt32(child.ChildNodes[1].InnerXml,16);
						((SortedList)m_tVal).Add(key, val);
					}
				}
                protected override void AssignFromObject(object ovalue)
                {
                    const int KEY = 0; 
                    const int VALUE = 1;
                    
                    //object[,] tagValueArray = {{"key","key","key"},{"value","value","value"}};
                    object[,] tagValueArray = (object[,])ovalue;
                    int tagValuePairs = tagValueArray.Length/2;

                    ((SortedList)m_tVal).Clear();
                    for (int idx=0; idx<tagValuePairs; idx++)
                    {
                        ((SortedList)m_tVal).Add(tagValueArray[idx,KEY],tagValueArray[idx,VALUE]);
                    }
                }

				public static implicit operator SortedList(AttribsMemberDWORDtoDWORDMap t)
				{
					return (SortedList)t.m_tVal;
				}
				
				public SortedList TVal
				{
					get {return this;}
					set {m_tVal = (Object)value;}
				}
			}
			
			
			public class AttribsMemberDWORDtoStringMap : AttribsMemberBase
			{
				public AttribsMemberDWORDtoStringMap(string keyName, SortedList defaultVal) : 
					base(keyName, defaultVal)
				{
					;
				}
				public override string ToString()
				{
					System.IO.StringWriter string_writer = new System.IO.StringWriter();
					System.Xml.XmlTextWriter xml_writer = new System.Xml.XmlTextWriter(string_writer);
					
					// write the class start tag
					xml_writer.WriteStartElement("Map");
					
					// write a text node so that we don't get a class tag that looks like <tag/> and instead we get <tag></tag> (the c++ version of the attribs does not use a legitimate xml parser)
					xml_writer.WriteWhitespace(" ");
					
					foreach (DictionaryEntry i in (SortedList)m_tVal)
					{
						xml_writer.WriteStartElement("N");
						xml_writer.WriteElementString("K",String.Format("{0:x}",(UInt32)i.Key));
						xml_writer.WriteElementString("V",(string)i.Value);
						xml_writer.WriteEndElement();
					}
					
					// write the class close tag
					xml_writer.WriteEndElement();
					
					return string_writer.ToString();
				}
				protected override void AssignFromString(string s)
				{
					if (s.Length == 0) return;
					AssignFromXmlNode(XMLDocumentHelper.GetDocumentElementFromString(s));
				}
				protected override void AssignFromXmlNode(System.Xml.XmlNode node)
				{
					((SortedList)m_tVal).Clear();
					foreach (System.Xml.XmlNode child in node.FirstChild.ChildNodes)
					{
						object key = Convert.ToUInt32(child.ChildNodes[0].InnerXml,16);
						object val = child.ChildNodes[1].InnerText;
						((SortedList)m_tVal).Add(key, val);
					}
				}
                protected override void AssignFromObject(object ovalue)
                {
                    const int KEY = 0; 
                    const int VALUE = 1;
                    
                    //object[,] tagValueArray = {{"key","key","key"},{"value","value","value"}};
                    object[,] tagValueArray = (object[,])ovalue;
                    int tagValuePairs = tagValueArray.Length/2;

                    ((SortedList)m_tVal).Clear();
                    for (int idx=0; idx<tagValuePairs; idx++)
                    {
                        ((SortedList)m_tVal).Add(tagValueArray[idx,KEY],tagValueArray[idx,VALUE]);
                    }    
                }
				public static implicit operator SortedList(AttribsMemberDWORDtoStringMap t)
				{
					return (SortedList)t.m_tVal;
				}
				
				public SortedList TVal
				{
					get {return this;}
					set {m_tVal = (Object)value;}
				}
			}
			
			
			public class AttribsMemberStringtoDWORDMap : AttribsMemberBase
			{
				public AttribsMemberStringtoDWORDMap(string keyName, SortedList defaultVal) : 
					base(keyName, defaultVal)
				{
					;
				}
				public override string ToString()
				{
					System.IO.StringWriter string_writer = new System.IO.StringWriter();
					System.Xml.XmlTextWriter xml_writer = new System.Xml.XmlTextWriter(string_writer);
					
					// write the class start tag
					xml_writer.WriteStartElement("Map");
					
					// write a text node so that we don't get a class tag that looks like <tag/> and instead we get <tag></tag> (the c++ version of the attribs does not use a legitimate xml parser)
					xml_writer.WriteWhitespace(" ");
					
					foreach (DictionaryEntry i in (SortedList)m_tVal)
					{
						xml_writer.WriteStartElement("N");
						xml_writer.WriteElementString("K",(string)i.Key);
						xml_writer.WriteElementString("V",String.Format("{0:x}",(UInt32)i.Value));
						xml_writer.WriteEndElement();
					}
					
					// write the class close tag
					xml_writer.WriteEndElement();
					
					return string_writer.ToString();
				}
				protected override void AssignFromString(string s)
				{
					if (s.Length == 0) return;
					AssignFromXmlNode(XMLDocumentHelper.GetDocumentElementFromString(s));
				}
				protected override void AssignFromXmlNode(System.Xml.XmlNode node)
				{
					((SortedList)m_tVal).Clear();
					foreach (System.Xml.XmlNode child in node.FirstChild.ChildNodes)
					{
						object key = child.ChildNodes[0].InnerText;
						object val = Convert.ToUInt32(child.ChildNodes[1].InnerXml,16);
						((SortedList)m_tVal).Add(key, val);
					}
				}
                protected override void AssignFromObject(object ovalue)
                {
                    const int KEY = 0; 
                    const int VALUE = 1;
                    
                    //object[,] tagValueArray = {{"key","key","key"},{"value","value","value"}};
                    object[,] tagValueArray = (object[,])ovalue;
                    int tagValuePairs = tagValueArray.Length/2;

                    ((SortedList)m_tVal).Clear();
                    for (int idx=0; idx<tagValuePairs; idx++)
                    {
                        ((SortedList)m_tVal).Add(tagValueArray[idx,KEY],tagValueArray[idx,VALUE]);
                    }    
                }
                public static implicit operator SortedList(AttribsMemberStringtoDWORDMap t)
				{
					return (SortedList)t.m_tVal;
				}
				
                public SortedList TVal
				{
					get {return this;}
					set {m_tVal = (Object)value;}
				}
			}
			
			
			public class AttribsMemberStringtoStringMap : AttribsMemberBase
			{
				public AttribsMemberStringtoStringMap(string keyName, SortedList defaultVal) : 
					base(keyName, defaultVal)
				{
					;
				}
				public override string ToString()
				{
					System.IO.StringWriter string_writer = new System.IO.StringWriter();
					System.Xml.XmlTextWriter xml_writer = new System.Xml.XmlTextWriter(string_writer);
					
					// write the class start tag
					xml_writer.WriteStartElement("Map");
					
					// write a text node so that we don't get a class tag that looks like <tag/> and instead we get <tag></tag> (the c++ version of the attribs does not use a legitimate xml parser)
					xml_writer.WriteWhitespace(" ");
					
					foreach (DictionaryEntry i in (SortedList)m_tVal)
					{
						xml_writer.WriteStartElement("N");
						xml_writer.WriteElementString("K",(string)i.Key);
						xml_writer.WriteElementString("V",(string)i.Value);
						xml_writer.WriteEndElement();
					}
					
					// write the class close tag
					xml_writer.WriteEndElement();
					
					return string_writer.ToString();
				}
				protected override void AssignFromString(string s)
				{
					if (s.Length == 0) return;
					AssignFromXmlNode(XMLDocumentHelper.GetDocumentElementFromString(s));
				}
				protected override void AssignFromXmlNode(System.Xml.XmlNode node)
				{
					((SortedList)m_tVal).Clear();
					foreach (System.Xml.XmlNode child in node.FirstChild.ChildNodes)
					{
						object key = child.ChildNodes[0].InnerText;
						object val = child.ChildNodes[1].InnerText;
						((SortedList)m_tVal).Add(key, val);
					}
				}
                protected override void AssignFromObject(object ovalue)
                {
                    const int KEY = 0; 
                    const int VALUE = 1;
                    
                    //object[,] tagValueArray = {{"key","key","key"},{"value","value","value"}};
                    object[,] tagValueArray = (object[,])ovalue;
                    int tagValuePairs = tagValueArray.Length/2;

                    ((SortedList)m_tVal).Clear();
                    for (int idx=0; idx<tagValuePairs; idx++)
                    {
                        ((SortedList)m_tVal).Add(tagValueArray[idx,KEY],tagValueArray[idx,VALUE]);
                    }    
                }
				public static implicit operator SortedList(AttribsMemberStringtoStringMap t)
				{
					return (SortedList)t.m_tVal;
				}
				
				public SortedList TVal
				{
					get {return this;}
					set {m_tVal = (Object)value;}
				}
			}
			
			
            public class AttribsMemberBuffer : AttribsMemberBase
			{
				public AttribsMemberBuffer(string keyName, byte[] defaultVal) : 
					base(keyName, defaultVal)
				{
					;
				}
				public override string ToString()
				{
					System.IO.StringWriter string_writer = new System.IO.StringWriter();
					System.Xml.XmlTextWriter xml_writer = new System.Xml.XmlTextWriter(string_writer);
					
					// write the class start tag
					xml_writer.WriteStartElement("CBuffer");
					
					// write a text node so that we don't get a class tag that looks like <tag/> and instead we get <tag></tag> (the c++ version of the attribs does not use a legitimate xml parser)
					xml_writer.WriteWhitespace(" ");
					
					byte[] data = (byte[])m_tVal;
					xml_writer.WriteElementString("Size",data.Length.ToString());
					xml_writer.WriteStartElement("Data");
					xml_writer.WriteString(AttribFormat.ConvertBytesToHex(data));
					xml_writer.WriteFullEndElement();
					
					// write the class close tag
					xml_writer.WriteEndElement();
					
					return string_writer.ToString();
				}
				protected override void AssignFromString(string s)
				{
					if (s.Length == 0) return;
					AssignFromXmlNode(XMLDocumentHelper.GetDocumentElementFromString(s));
				}
				protected override void AssignFromXmlNode(System.Xml.XmlNode node)
				{
					m_tVal = AttribFormat.ConvertHexToBytes(node.LastChild.FirstChild.Value);
				}
                protected override void AssignFromObject(object ovalue)
                {
                    //Not implemented
                }

				
				public static implicit operator byte[](AttribsMemberBuffer t)
				{
					return (byte[])t.m_tVal;
				}
				
				public byte[] TVal
				{
					get {return this;}
					set {m_tVal = (Object)value;}
				}
				
			}
						
			public class AttribsMemberGenericList : AttribsMemberBase
			{
				public AttribsMemberGenericList(string keyName, System.Type TValue, ArrayList defaultVal) : 
					base(keyName, defaultVal)
				{
					m_ValueType = TValue;
				}

				private System.Type m_ValueType;
				public System.Type ValueType
				{
					get{return m_ValueType;}
				}
				
				public override string ToString()
				{
					System.IO.StringWriter string_writer = new System.IO.StringWriter();
					System.Xml.XmlTextWriter xml_writer = new System.Xml.XmlTextWriter(string_writer);
					
					// write the class start tag
					xml_writer.WriteStartElement("Vector");
					
					// write a text node so that we don't get a class tag that looks like <tag/> and instead we get <tag></tag> (the c++ version of the attribs does not use a legitimate xml parser)
					xml_writer.WriteWhitespace(" ");
					
					foreach (object o in (ArrayList)m_tVal)
					{
						xml_writer.WriteStartElement("N");
						
						// write the Value string
                        if (typeof(Solimar.Licensing.Attribs.LicensingAttribsBase).IsInstanceOfType(o))
							xml_writer.WriteRaw(o.ToString());
						else
							xml_writer.WriteRaw(AttribFormat.ConvertTToString(o));

						xml_writer.WriteEndElement();
					}
					
					// write the class close tag
					xml_writer.WriteEndElement();
					
					return string_writer.ToString();
				}
				protected override void AssignFromString(string s)
				{
					if (s.Length == 0) return;
					AssignFromXmlNode(XMLDocumentHelper.GetDocumentElementFromString(s));
				}
				protected override void AssignFromXmlNode(System.Xml.XmlNode node)
				{
					((ArrayList)m_tVal).Clear();
                    if (m_ValueType.IsSubclassOf(typeof(Solimar.Licensing.Attribs.LicensingAttribsBase)))
					{
						foreach (System.Xml.XmlNode child in node.FirstChild.ChildNodes)
						{
							// create a new attribs class of type m_ValueType
							LicensingAttribsBase attribs_class = (LicensingAttribsBase)m_ValueType.GetConstructor(new System.Type[]{}).Invoke(new object[]{});
							// initialize the attribs class members with the data read from the xml
							attribs_class.AssignMembersFromXmlNode(child.FirstChild);
							// add the attribs class to the array
							((ArrayList)m_tVal).Add(attribs_class);
						}
					}
					else
					{
						foreach (System.Xml.XmlNode child in node.FirstChild.ChildNodes)
						{
							((ArrayList)m_tVal).Add(AttribFormat.ConvertStringToT(m_ValueType,child.InnerXml));
						}
					}
				}
                protected override void AssignFromObject(object ovalue)
                {
                    //const int TAG = 0;
                    //const int VALUE = 1;
                    System.Array valueArray = (System.Array)ovalue;
                    //object[,] tagValueArray = {{"tag","tag","tag"},{"value","value","value"}};
                    //int tagValuePairs = tagValueArray.Length/2;

                    ((ArrayList)m_tVal).Clear();
                    if (m_ValueType.IsSubclassOf(typeof(Solimar.Licensing.Attribs.LicensingAttribsBase)))
                    { 
                        for (int idx=0; idx<valueArray.Length; idx++)
                        {
                            // create a new attribs class of type m_ValueType
                            LicensingAttribsBase attribs_class = (LicensingAttribsBase)m_ValueType.GetConstructor(new System.Type[]{}).Invoke(new object[]{});
                            // initialize the attribs class members with the data from VALUE portion of array
                            attribs_class.AssignMembersFromObject(valueArray.GetValue(idx));
                            // add the attribs class to the array
                            ((ArrayList)m_tVal).Add(attribs_class);
                        }
                    }
                    else
                    {
                        for (int idx=0; idx<valueArray.Length; idx++)
                        {
                            //((ArrayList)m_tVal).Add(AttribFormat.ConvertStringToT(m_ValueType,child.InnerXml));
                            ((ArrayList)m_tVal).Add(valueArray.GetValue(idx));
                        }
                    }
                }

				public static implicit operator ArrayList(AttribsMemberGenericList t)
				{
					return (ArrayList)t.m_tVal;
				}
				
				public ArrayList TVal
				{
					get {return this;}
					set {m_tVal = (Object)value;}
				}
			}
			
			public class AttribsMemberGenericMap : AttribsMemberBase
			{
				public AttribsMemberGenericMap(string keyName, System.Type TKey, System.Type TValue, SortedList defaultVal) : 
					base(keyName, defaultVal)
				{
					m_KeyType = TKey;
					m_ValueType = TValue;
				}
				
				private System.Type m_KeyType;
				public System.Type KeyType
				{
					get{return m_KeyType;}
				}
				private System.Type m_ValueType;
				public System.Type ValueType
				{
					get{return m_ValueType;}
				}
				
				public override string ToString()
				{
					System.IO.StringWriter string_writer = new System.IO.StringWriter();
					System.Xml.XmlTextWriter xml_writer = new System.Xml.XmlTextWriter(string_writer);
					
					// write the class start tag
					xml_writer.WriteStartElement("Map");
					
					// write a text node so that we don't get a class tag that looks like <tag/> and instead we get <tag></tag> (the c++ version of the attribs does not use a legitimate xml parser)
					xml_writer.WriteWhitespace(" ");
					
					foreach (DictionaryEntry i in (SortedList)m_tVal)
					{
						xml_writer.WriteStartElement("N");
						
						// write the Key string
						xml_writer.WriteStartElement("K");
                        if (typeof(Solimar.Licensing.Attribs.LicensingAttribsBase).IsInstanceOfType(i.Key))
							xml_writer.WriteRaw(i.Key.ToString());
						else
							xml_writer.WriteRaw(AttribFormat.ConvertTToString(i.Key));
						xml_writer.WriteEndElement();
						
						// write the Value string
						xml_writer.WriteStartElement("V");
                        if (typeof(Solimar.Licensing.Attribs.LicensingAttribsBase).IsInstanceOfType(i.Value))
							xml_writer.WriteRaw(i.Value.ToString());
						else
							xml_writer.WriteRaw(AttribFormat.ConvertTToString(i.Value));
						xml_writer.WriteEndElement();
						
						xml_writer.WriteEndElement();
					}
					
					// write the class close tag
					xml_writer.WriteEndElement();
					
					return string_writer.ToString();
				}
				protected override void AssignFromString(string s)
				{
					if (s.Length == 0) return;
					AssignFromXmlNode(XMLDocumentHelper.GetDocumentElementFromString(s));
				}
				protected override void AssignFromXmlNode(System.Xml.XmlNode node)
				{
					((SortedList)m_tVal).Clear();
					foreach (System.Xml.XmlNode child in node.FirstChild.ChildNodes)
					{
						object key;
						object val;

                        if (m_KeyType.IsSubclassOf(typeof(Solimar.Licensing.Attribs.LicensingAttribsBase)))
						{
							// create a new attribs class of type m_ValueType
							LicensingAttribsBase attribs_class = (LicensingAttribsBase)m_KeyType.GetConstructor(new System.Type[]{}).Invoke(new object[]{});
							// initialize the attribs class members with the data read from the xml
							attribs_class.AssignMembersFromXmlNode(child.ChildNodes[0].FirstChild);
							// save the key for insertion
							key = attribs_class;
						}
						else
						{
							key = AttribFormat.ConvertStringToT(m_KeyType,child.ChildNodes[0].InnerXml);
						}

                        if (m_ValueType.IsSubclassOf(typeof(Solimar.Licensing.Attribs.LicensingAttribsBase)))
						{
							// create a new attribs class of type m_ValueType
							LicensingAttribsBase attribs_class = (LicensingAttribsBase)m_ValueType.GetConstructor(new System.Type[]{}).Invoke(new object[]{});
							// initialize the attribs class members with the data read from the xml
							attribs_class.AssignMembersFromXmlNode(child.ChildNodes[1].FirstChild);
							// save the value for insertion
							val = attribs_class;
						}
						else
						{
							val = AttribFormat.ConvertStringToT(m_ValueType,child.ChildNodes[1].InnerXml);
						}
						
						((SortedList)m_tVal).Add(key, val);
					}
				}
            protected override void AssignFromObject(object ovalue)
            {
               ((SortedList)m_tVal).Clear();
               const int TAG = 0;
               const int VALUE = 1;
               object[,] tagValueArray = (object[,])ovalue;
               //object[,] tagValueArray = {{"tag","tag","tag"},{"value","value","value"}};
               System.Collections.SortedList stream_map = new System.Collections.SortedList();
               int tagValuePairs = tagValueArray.Length/2;
               for (int idx=0; idx<tagValuePairs; idx++)
               {
                  //stream_map[tagValueArray[idx, TAG].ToString()] = tagValueArray[idx, VALUE];
               //}
               
               
               
               //foreach (System.Xml.XmlNode child in node.FirstChild.ChildNodes)
               //{
                  object key;
                  object val;

                  if (m_KeyType.IsSubclassOf(typeof(Solimar.Licensing.Attribs.LicensingAttribsBase)))
                  {
                     // create a new attribs class of type m_ValueType
                     LicensingAttribsBase attribs_class = (LicensingAttribsBase)m_KeyType.GetConstructor(new System.Type[]{}).Invoke(new object[]{});
                     // initialize the attribs class members with the data read from the xml
                     attribs_class.AssignMembersFromObject(tagValueArray[idx, VALUE]/*child.ChildNodes[0].FirstChild*/);
                     // save the key for insertion
                     key = attribs_class;
                  }
                  else
                  {
                     key = tagValueArray[idx, TAG];//AttribFormat.ConvertStringToT(m_KeyType,child.ChildNodes[0].InnerXml);
                  }

                  if (m_ValueType.IsSubclassOf(typeof(Solimar.Licensing.Attribs.LicensingAttribsBase)))
                  {
                     // create a new attribs class of type m_ValueType
                     LicensingAttribsBase attribs_class = (LicensingAttribsBase)m_ValueType.GetConstructor(new System.Type[]{}).Invoke(new object[]{});
                     // initialize the attribs class members with the data read from the xml
                     attribs_class.AssignMembersFromObject(tagValueArray[idx, VALUE]);//attribs_class.AssignMembersFromXmlNode(child.ChildNodes[1].FirstChild);
                     // save the value for insertion
                     val = attribs_class;
                  }
                  else
                  {
                     val = AttribFormat.ConvertStringToT(m_KeyType, tagValueArray[idx, VALUE].ToString());//m_KeyType,child.ChildNodes[1].InnerXml);
                  }
						
                  ((SortedList)m_tVal).Add(key, val);
               }
            }
				public static implicit operator SortedList(AttribsMemberGenericMap t)
				{
					return (SortedList)t.m_tVal;
				}
				
				public SortedList TVal
				{
					get {return this;}
					set {m_tVal = (Object)value;}
				}
			}
			


			public class LicensingAttribsBase : System.ICloneable
			{
				protected string m_ClassName = null;
				
				public LicensingAttribsBase(string ClassName)
				{
					m_ClassName = ClassName;
				}
				
				public string GetLicensingClassName()
				{
					return m_ClassName;
				}
				
				public string Stream
				{
					get
					{
						return this;
					}
					set
					{
						AssignMembersFromStream(value);
					}
				}

				public override string ToString()
				{
					System.IO.StringWriter string_writer = new System.IO.StringWriter();
					System.Xml.XmlTextWriter xml_writer = new System.Xml.XmlTextWriter(string_writer);
					
					// write the class start tag
					xml_writer.WriteStartElement(this.m_ClassName);
					
					// write a text node so that we don't get a class tag that looks like <tag/> and instead we get <tag></tag> (the c++ version of the attribs does not use a legitimate xml parser)
					xml_writer.WriteWhitespace(" ");
					
					// get a list of the members
					System.Reflection.MemberInfo[] members = this.GetType().GetMembers(System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.Public);
					
					// for each member
					foreach (System.Reflection.MemberInfo member in members)
					{
						// if the member is a field, and derives from AttribsMemberBase, 'serialize' it
						if (member.MemberType == System.Reflection.MemberTypes.Field && TypeDerivesFromMemberBase(((System.Reflection.FieldInfo)member).FieldType))
						{
							AttribsMemberBase attribs_member = (AttribsMemberBase)(((System.Reflection.FieldInfo)member).GetValue(this));
							xml_writer.WriteStartElement(attribs_member.GetKeyName());
							xml_writer.WriteRaw(attribs_member.ToString());
							xml_writer.WriteEndElement();
						}
					}
					
					// write the class close tag
					xml_writer.WriteEndElement();
					
					return string_writer.ToString();
				}
				
				public void AssignMembersFromStream(string AttribsStream)
				{
                    if (AttribsStream.Length == 0) return;
					AssignMembersFromXmlNode(XMLDocumentHelper.GetDocumentElementFromString(AttribsStream));
				}
                // Replacing AssignMembersFromXmlNode() where performance is an issue.
                public void AssignMembersFromObject(object o)
                {
                    const int TAG = 0;
                    const int VALUE = 1;
                    object[,] tagValueArray = (object[,])o;
                    //object[,] tagValueArray = {{"tag","tag","tag"},{"value","value","value"}};
                    System.Collections.SortedList stream_map = new System.Collections.SortedList();
                    int tagValuePairs = tagValueArray.Length/2;
                    for (int idx=0; idx<tagValuePairs; idx++)
                    {
                        stream_map[tagValueArray[idx, TAG].ToString()] = tagValueArray[idx, VALUE];
                    }

                    // iterate over the class members, assign them values using the values provided in the value array
                    System.Reflection.MemberInfo[] members = this.GetType().GetMembers(System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.Public);
                    foreach (System.Reflection.MemberInfo member in members)
                    {
                        // if the member is a field, and derives from AttribsMemberBase, 'serialize' it
                        if (member.MemberType == System.Reflection.MemberTypes.Field && TypeDerivesFromMemberBase(((System.Reflection.FieldInfo)member).FieldType)) 
                        {
                            AttribsMemberBase attribs_member = (AttribsMemberBase)(((System.Reflection.FieldInfo)member).GetValue(this));
                            if (stream_map.ContainsKey(attribs_member.GetKeyName()))
                            {
                                //Set XVal property which calls AssignFromXmlNode(), AssignFromXmlNode() is overridden in
                                //complex container types.
                                attribs_member.ZVal = (object)stream_map[attribs_member.GetKeyName()];
                            }
                        }
                    }
                }
				
                public void AssignMembersFromXmlNode(System.Xml.XmlNode node)
				{
					System.Collections.SortedList stream_map = new System.Collections.SortedList();
					
					foreach (System.Xml.XmlNode child in node.ChildNodes)
					{
						if (child.NodeType == System.Xml.XmlNodeType.Element)
							stream_map[child.Name] = child;
					}
					
					// iterate over the class members, assign them values using the values provided in the stream
					System.Reflection.MemberInfo[] members = this.GetType().GetMembers(System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.Public);
					foreach (System.Reflection.MemberInfo member in members)
					{
						// if the member is a field, and derives from AttribsMemberBase, 'serialize' it
						if (member.MemberType == System.Reflection.MemberTypes.Field && TypeDerivesFromMemberBase(((System.Reflection.FieldInfo)member).FieldType)) 
						{
							AttribsMemberBase attribs_member = (AttribsMemberBase)(((System.Reflection.FieldInfo)member).GetValue(this));
							if (stream_map.ContainsKey(attribs_member.GetKeyName()))
							{
								attribs_member.XVal = (System.Xml.XmlNode)stream_map[attribs_member.GetKeyName()];
							}
						}
					}
				}

				
				public static implicit operator string(LicensingAttribsBase t)
				{
					return t.ToString();
				}

				private bool TypeDerivesFromMemberBase(System.Type t)
				{
					if (t == null)
						return false;
                    if (t == typeof(Solimar.Licensing.Attribs.AttribsMemberBase))
						return true;
					return TypeDerivesFromMemberBase(t.BaseType);
				}

				#region ICloneable Members

				public object Clone()
				{
					object n = this.GetType().GetConstructor(new System.Type[]{}).Invoke(new object[]{});
					((LicensingAttribsBase)n).Stream = this.Stream;
					return n;
				}

				#endregion
			}


			/* A class that knows how to convert between primitive 
			 * types and their Licensing Attribs string representations */
			public class AttribFormat
			{
				//public static SortedList
				private static System.Globalization.CultureInfo cultureInfo = new System.Globalization.CultureInfo("en-US", false);

				public static object ConvertStringToT(System.Type T, string s)
				{
					if (typeof(UInt64) == T)
						return ConvertStringToUInt64(s);
					if (typeof(UInt32) == T)
						return ConvertStringToUInt32(s);
					if (typeof(UInt16) == T)
						return ConvertStringToUInt16(s);
					if (typeof(Byte) == T)
						return ConvertStringToByte(s);
					if (typeof(Int64) == T)
						return ConvertStringToInt64(s);
					if (typeof(Int32) == T)
						return ConvertStringToInt32(s);
					if (typeof(Int16) == T)
						return ConvertStringToInt16(s);
					if (typeof(SByte) == T)
						return ConvertStringToSByte(s);
					if (typeof(float) == T)
						return ConvertStringToFloat(s);
					if (typeof(double) == T)
						return ConvertStringToDouble(s);
					if (typeof(bool) == T)
						return ConvertStringToBool(s);
					if (typeof(byte[]) == T)
						return ConvertHexToBytes(s);
					if (typeof(string) == T)
						return ConvertStringToRawString(s);
					return null;
				}
				public static string ConvertTToString(object o)
				{
					System.Type T = o.GetType();
					if (typeof(UInt64) == T)
						return ConvertUInt64ToString((UInt64)o);
					if (typeof(UInt32) == T)
						return ConvertUInt32ToString((UInt32)o);
					if (typeof(UInt16) == T)
						return ConvertUInt16ToString((UInt16)o);
					if (typeof(Byte) == T)
						return ConvertByteToString((Byte)o);
					if (typeof(Int64) == T)
						return ConvertInt64ToString((Int64)o);
					if (typeof(Int32) == T)
						return ConvertInt32ToString((Int32)o);
					if (typeof(Int16) == T)
						return ConvertInt16ToString((Int16)o);
					if (typeof(SByte) == T)
						return ConvertSByteToString((SByte)o);
					if (typeof(float) == T)
						return ConvertFloatToString((float)o);
					if (typeof(double) == T)
						return ConvertDoubleToString((double)o);
					if (typeof(bool) == T)
						return ConvertBoolToString((bool)o);
					if (typeof(byte[]) == T)
						return ConvertBytesToHex((byte[])o);
					if (typeof(string) == T)
						return ConvertRawStringToString((string)o);
					return null;
				}

				public static byte[] ConvertHexToBytes(string s)
				{
					if (s == null) return new byte[]{};
					
					byte[] ret = new byte[s.Length/2];
					for (int i = 0; i < s.Length/2; ++i)
					{
						ret[i] = Convert.ToByte(s.Substring(i*2, 2), 16);
					}
					return ret;
				}
				public static string ConvertBytesToHex(byte[] bytes)
				{
					if (bytes==null || bytes.Length==0) return "";

					System.Text.StringBuilder ret = new System.Text.StringBuilder(bytes.Length*2, bytes.Length*2);
					foreach (byte b in bytes)
					{
						ret.AppendFormat("{0:x2}",b);
					}
					return ret.ToString();
				}
				
				public static string ConvertUInt64ToString(UInt64 i)
				{
					return String.Format("{0:x}",i);
				}
				public static UInt64 ConvertStringToUInt64(string s)
				{
					return Convert.ToUInt64(s,16);
				}

				public static string ConvertUInt32ToString(UInt32 i)
				{
					UInt64 temp64 = Convert.ToUInt64(i);
					return String.Format("{0:x}",temp64);
				}
				public static UInt32 ConvertStringToUInt32(string s)
				{
					// Need to do the following check because at one point the values were stored in their "native" format, but are now 64-bit
					if (s.Length == 8) // It's a 32-bit number represented in hex
						return Convert.ToUInt32(s, 16);
					else
					{
						UInt64 temp64 = Convert.ToUInt64(s, 16);
						return Convert.ToUInt32(temp64);
					}
				}

				public static string ConvertUInt16ToString(UInt16 i)
				{
					UInt64 temp64 = Convert.ToUInt64(i);
					return String.Format("{0:x}",temp64);
				}
				public static UInt16 ConvertStringToUInt16(string s)
				{
					// Need to do the following check because at one point the values were stored in their "native" format, but are now 64-bit
					if (s.Length == 4) // It's a 16-bit number represented in hex
						return Convert.ToUInt16(s, 16);
					else
					{
						UInt64 temp64 = Convert.ToUInt64(s, 16);
						return Convert.ToUInt16(temp64);
					}
				}

				public static string ConvertByteToString(Byte i)
				{
					UInt64 temp64 = Convert.ToUInt64(i);
					return String.Format("{0:x}",temp64);
				}
				public static Byte ConvertStringToByte(string s)
				{
					// Need to do the following check because at one point the values were stored in their "native" format, but are now 64-bit
					if (s.Length == 2) // It's a 8-bit number represented in hex
						return Convert.ToByte(s, 16);
					else
					{
						UInt64 temp64 = Convert.ToUInt64(s, 16);
						return Convert.ToByte(temp64);
					}
				}
				
				public static string ConvertInt64ToString(Int64 i)
				{
					return String.Format("{0:x}",i);
				}
				public static Int64 ConvertStringToInt64(string s)
				{
					return Convert.ToInt64(s,16);
				}

				public static string ConvertInt32ToString(Int32 i)
				{
					Int64 temp64 = Convert.ToInt64(i);
					return String.Format("{0:x}",temp64);
				}
				public static Int32 ConvertStringToInt32(string s)
				{
					// Need to do the following check because at one point the values were stored in their "native" format, but are now 64-bit
					if (s.Length == 8) // It's a 32-bit number represented in hex
						return Convert.ToInt32(s, 16);
					else
					{
						Int64 temp64 = Convert.ToInt64(s, 16);
						return Convert.ToInt32(temp64);
					}
				}

				public static string ConvertInt16ToString(Int16 i)
				{
					Int64 temp64 = Convert.ToInt64(i);
					return String.Format("{0:x}",temp64);
				}
				public static Int16 ConvertStringToInt16(string s)
				{
					// Need to do the following check because at one point the values were stored in their "native" format, but are now 64-bit
					if (s.Length == 4) // It's a 16-bit number represented in hex
						return Convert.ToInt16(s, 16);
					else
					{
						Int64 temp64 = Convert.ToInt64(s, 16);
						return Convert.ToInt16(temp64);
					}
				}

				public static string ConvertSByteToString(SByte i)
				{
					Int64 temp64 = Convert.ToInt64(i);
					return String.Format("{0:x}",temp64);
				}
				public static SByte ConvertStringToSByte(string s)
				{
					// Need to do the following check because at one point the values were stored in their "native" format, but are now 64-bit
					if (s.Length == 2) // It's a 8-bit number represented in hex
						return Convert.ToSByte(s, 16);
					else
					{
						Int64 temp64 = Convert.ToInt64(s, 16);
						return Convert.ToSByte(temp64);
					}
				}

				public static string ConvertFloatToString(float i)
				{
					return String.Format(cultureInfo, "{0:g}",i);
				}
				public static float ConvertStringToFloat(string s)
				{
					return Convert.ToSingle(s, cultureInfo);
				}

				public static string ConvertDoubleToString(double i)
				{
					return String.Format(cultureInfo, "{0:g}",i);
				}
				public static double ConvertStringToDouble(string s)
				{
					return Convert.ToDouble(s, cultureInfo);
				}

				public static string ConvertDateTimeToString(DateTime i)
				{
					return i.ToString("yyyy-MM-dd HH:mm:ss.ffff", cultureInfo);
				}
				public static DateTime ConvertStringToDateTime(string s)
				{
					return DateTime.ParseExact(s, "yyyy-MM-dd HH:mm:ss.ffff", cultureInfo);
				}

				public static string ConvertBoolToString(bool i)
				{
					return (i ? "1" : "0");
				}
				public static bool ConvertStringToBool(string s)
				{
					return (s == "0" || s == "" ? false : true);
				}
				public static string ConvertStringToRawString(string s)
				{
					if (s.IndexOf('&') >= 0)
					{
						s = s.Replace("&lt;", "<");
						s = s.Replace("&gt;", ">");
						s = s.Replace("&amp;", "&");
						s = s.Replace("&apos;", "'");
						s = s.Replace("&quot;", "\"");
						if (s.IndexOf("&#x") >= 0) // very rare
						{
							s = s.Replace("&#x0;", "\x00");
							s = s.Replace("&#x1;", "\x01");
							s = s.Replace("&#x2;", "\x02");
							s = s.Replace("&#x3;", "\x03");
							s = s.Replace("&#x4;", "\x04");
							s = s.Replace("&#x5;", "\x05");
							s = s.Replace("&#x6;", "\x06");
							s = s.Replace("&#x7;", "\x07");
							s = s.Replace("&#x8;", "\x08");
							s = s.Replace("&#xB;", "\x0B");
							s = s.Replace("&#xC;", "\x0C");
							s = s.Replace("&#xE;", "\x0E");
							s = s.Replace("&#xF;", "\x0F");
							s = s.Replace("&#x10;", "\x10");
							s = s.Replace("&#x11;", "\x11");
							s = s.Replace("&#x12;", "\x12");
							s = s.Replace("&#x13;", "\x13");
							s = s.Replace("&#x14;", "\x14");
							s = s.Replace("&#x15;", "\x15");
							s = s.Replace("&#x16;", "\x16");
							s = s.Replace("&#x17;", "\x17");
							s = s.Replace("&#x18;", "\x18");
							s = s.Replace("&#x19;", "\x19");
							s = s.Replace("&#x1A;", "\x1A");
							s = s.Replace("&#x1B;", "\x1B");
							s = s.Replace("&#x1C;", "\x1C");
							s = s.Replace("&#x1D;", "\x1D");
							s = s.Replace("&#x1E;", "\x1E");
							s = s.Replace("&#x1F;", "\x1F");
						}
					}
					return s;
				}
				public static string ConvertRawStringToString(string s)
				{
					System.IO.StringWriter string_writer = new System.IO.StringWriter();
					System.Xml.XmlTextWriter xml_writer = new System.Xml.XmlTextWriter(string_writer);
					xml_writer.WriteString(s);
					return string_writer.ToString();
				}
			}

			public class XMLDocumentHelper
			{
				public static System.Xml.XmlElement GetDocumentElementFromString(string xml)
				{
					System.Xml.XmlDocument xml_reader_doc = new System.Xml.XmlDocument();

					try
					{
						xml_reader_doc.LoadXml(xml);
					}
					catch (System.Xml.XmlException)
					{
						//Replace non-escaped non-printable char that cause exceptions in the XML Parser
						foreach (char replaceChar in new char[] { '\x0001', '\x0002', '\x0003', '\x0004', '\x0005', '\x0006', '\x0007', '\x0008', '\x000B', '\x000C', '\x000E', '\x000F',
						 '\x0010', '\x0011', '\x0012', '\x0013', '\x0014', '\x0015', '\x0016', '\x0017', '\x0018', '\x0019', '\x001A', '\x001B', '\x001C', '\x001D', '\x001E', '\x001F'})
							xml = xml.Replace(replaceChar, '.');
						xml_reader_doc.LoadXml(xml);
					}
					return xml_reader_doc.DocumentElement;
				}
			}
		}
	}
}
