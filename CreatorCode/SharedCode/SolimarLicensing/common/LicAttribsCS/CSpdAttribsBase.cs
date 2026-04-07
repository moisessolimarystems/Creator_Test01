using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Diagnostics;
using System.Linq;
using System.Xml;

namespace Solimar
{
	namespace Licensing
	{
		namespace Attribs
		{

			public interface IAttribsMemberBase
			{
				string SVal { get; set; }

				object OVal { get; set; }

				System.Xml.XmlNode XVal { get; set; }

				object ZVal { get; set; }

				string GetKeyName();

				bool IsModified { get; }
			}
			public enum NumEncoding { Decimal = 10, Hex = 16 };
			public abstract class AttribsMemberBase<T> : System.ICloneable, IAttribsMemberBase
			{
				public AttribsMemberBase()
				{
					;
				}
				public AttribsMemberBase(string keyName, T defaultVal)
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
					get { return this.ToString(); }
					set { AssignFromString(value); }
				}

				public object OVal
				{
					get { return Get(); }
					set { Set((T)value); }
				}

				public System.Xml.XmlNode XVal
				{
					get { return null; }
					set
					{
						AssignFromXmlNode(value);
						// CR.Fix.32264.v01: When an attribute is set using XML, set isModified to true
						// so that the attribute will be included in the XML if it is serialized, even if
						// the attribute is set to its default value.
						isModified = true;
					}
				}
				public object ZVal
				{
					get { return null; }
					set { AssignFromObject(value); }
				}

				public static implicit operator string(AttribsMemberBase<T> t)
				{
					return t.ToString();
				}

				public static implicit operator T(AttribsMemberBase<T> t)
				{
					return t.Get();
				}

				public T TVal
				{
					get { return Get(); }
					set { Set(value); }
				}

				public T Value
				{
					get { return Get(); }
					set { Set(value); }
				}

				public T Get() { return m_tVal; }
				public void Set(T value)
				{
					if (!EqualityComparer<T>.Default.Equals(m_tVal, value))
					{
						SetAsModified(value);
					}
				}

				/// <summary>
				/// This method will set the value and the IsModified flag, regardless of current/new value.
				/// This is needed to serialize out the value, even if it's the default.
				/// In some caseses this is relied upon to indicate that the setting is coming from the UI.
				/// </summary>
				/// <param name="value"></param>
				public void SetAsModified(T value)
				{
					isModified = true;
					m_tVal = value;
				}

				public string GetKeyName()
				{
					return m_KeyName;
				}

				public virtual bool IsModified { get { return isModified; } }

				protected string m_KeyName;
				protected T m_tVal;
				protected bool isModified = false;

				protected static SpdAttribsBase InitializeAttribsBaseOfType(Type attribBaseType, XmlNode nodeData)
				{
					SpdAttribsBase attribsBase = SpdAttribsBase.CreateType(attribBaseType);
					// initialize the attribs class members with the data read from the xml
					attribsBase?.AssignMembersFromXmlNode(nodeData);

					return attribsBase;
				}

				protected static SpdAttribsBase InitializeAttribsBaseOfType(Type attribBaseType, object objData)
				{
					SpdAttribsBase attribsBase = SpdAttribsBase.CreateType(attribBaseType);
					// initialize the attribs class members with the data from VALUE portion of array
					attribsBase?.AssignMembersFromObject(objData);

					return attribsBase;
				}

				#region ICloneable Members

				public object Clone()
				{
					object n = this.GetType().GetConstructor(new System.Type[] { }).Invoke(new object[] { });
					((AttribsMemberBase<T>)n).SVal = this.SVal;
					return n;
				}

				#endregion
			}

			public abstract class AttribsMemberIntBase<T> : AttribsMemberBase<T>
			{
				public AttribsMemberIntBase(string keyName, T defaultVal, NumEncoding numEncoding) : base(keyName, defaultVal)
				{
					m_numEncoding = numEncoding;
				}

				protected NumEncoding m_numEncoding;

				public override string ToString() { return this.ToString(m_numEncoding); }
				public string ToString(NumEncoding numEncoding) { return AttribFormat.ConvertIntToString(Get(), numEncoding); }
			}

			public abstract class AttribsMemberUIntBase<T> : AttribsMemberBase<T>
			{
				public AttribsMemberUIntBase(string keyName, T defaultVal, NumEncoding numEncoding) : base(keyName, defaultVal)
				{
					m_numEncoding = numEncoding;
				}

				protected NumEncoding m_numEncoding;

				public override string ToString() { return this.ToString(m_numEncoding); }
				public string ToString(NumEncoding numEncoding) { return AttribFormat.ConvertIntToString(Get(), numEncoding); }
			}

			public class AttribsMemberString : AttribsMemberBase<string>, IComparable
			{
				public AttribsMemberString(string keyName, string defaultVal) :
					base(keyName, defaultVal)
				{
					;
				}
				public override string ToString()
				{
					return AttribFormat.ConvertRawStringToString(Get());
				}
				protected override void AssignFromString(string s)
				{
					Set(AttribFormat.ConvertStringToRawString(s));
				}

				public static implicit operator string(AttribsMemberString t)
				{
					return t.Get();
				}

				public override int GetHashCode()
				{
					return base.GetHashCode();
				}

				public override bool Equals(object obj)
				{
					if (obj is string str)
						return this.Value.Equals(str);
					else if (obj is AttribsMemberString attribStr)
						return this.Value.Equals(attribStr.Value);
					else
						return this.Value.Equals(obj);
				}

				public int CompareTo(object obj)
				{
					if (obj is string str)
						return this.Value.CompareTo(str);
					else if (obj is AttribsMemberString attribStr)
						return this.Value.CompareTo(attribStr.Value);
					else
						return this.Value.CompareTo(obj);
				}

				public bool IsEmpty() { return string.IsNullOrWhiteSpace(this); }
				public bool NotEmpty() { return !IsEmpty(); }
			}

			public class AttribsMemberXmlString : AttribsMemberBase<string>
			{
				public AttribsMemberXmlString(string keyName, string defaultVal) :
					base(keyName, defaultVal)
				{
					;
				}
				public override string ToString()
				{
					return Get();
				}
				protected override void AssignFromString(string s)
				{
					if (s.StartsWith("&lt;"))
						Set(AttribFormat.ConvertStringToRawString(s));
					else
						Set(s);
				}

				public static implicit operator string(AttribsMemberXmlString t)
				{
					return t.Get();
				}

				public override int GetHashCode()
				{
					return base.GetHashCode();
				}

				public override bool Equals(object obj)
				{
					if (obj is string comparison)
						return ((string)this).Equals(comparison);
					else
						return ((string)this).Equals(obj);
				}

				public bool IsEmpty() { return string.IsNullOrWhiteSpace(this); }
				public bool NotEmpty() { return !IsEmpty(); }
			}

			public class AttribsUInt64 : AttribsMemberUIntBase<UInt64>
			{
				public AttribsUInt64(string keyName, UInt64 defaultVal, NumEncoding numEncoding = NumEncoding.Hex) :
					base(keyName, defaultVal, numEncoding) {; }
				protected override void AssignFromString(string s) { Set(AttribFormat.ConvertStringToUInt64(s, m_numEncoding)); }
			}

			public class AttribsUInt32 : AttribsMemberUIntBase<UInt32>
			{
				public AttribsUInt32(string keyName, UInt32 defaultVal, NumEncoding numEncoding = NumEncoding.Hex) :
					base(keyName, defaultVal, numEncoding) {; }
				protected override void AssignFromString(string s) { Set(AttribFormat.ConvertStringToUInt32(s, m_numEncoding)); }
			}

			public class AttribsUInt16 : AttribsMemberUIntBase<UInt16>
			{
				public AttribsUInt16(string keyName, UInt16 defaultVal, NumEncoding numEncoding = NumEncoding.Hex) :
					base(keyName, defaultVal, numEncoding) {; }
				protected override void AssignFromString(string s) { Set(AttribFormat.ConvertStringToUInt16(s, m_numEncoding)); }
			}

			public class AttribsByte : AttribsMemberUIntBase<Byte>
			{
				public AttribsByte(string keyName, Byte defaultVal, NumEncoding numEncoding = NumEncoding.Hex) :
					base(keyName, defaultVal, numEncoding) {; }
				protected override void AssignFromString(string s) { Set(AttribFormat.ConvertStringToByte(s, m_numEncoding)); }
			}

			public class AttribsInt64 : AttribsMemberIntBase<Int64>
			{
				public AttribsInt64(string keyName, Int64 defaultVal, NumEncoding numEncoding = NumEncoding.Hex) :
					base(keyName, defaultVal, numEncoding) {; }
				protected override void AssignFromString(string s) { Set(AttribFormat.ConvertStringToInt64(s, m_numEncoding)); }
			}

			public class AttribsInt32 : AttribsMemberIntBase<Int32>
			{
				public AttribsInt32(string keyName, Int32 defaultVal, NumEncoding numEncoding = NumEncoding.Hex) :
					base(keyName, defaultVal, numEncoding) {; }
				protected override void AssignFromString(string s) { Set(AttribFormat.ConvertStringToInt32(s, m_numEncoding)); }
			}

			public class AttribsInt16 : AttribsMemberIntBase<Int16>
			{
				public AttribsInt16(string keyName, Int16 defaultVal, NumEncoding numEncoding = NumEncoding.Hex) :
					base(keyName, defaultVal, numEncoding) {; }
				protected override void AssignFromString(string s) { Set(AttribFormat.ConvertStringToInt16(s, m_numEncoding)); }
			}

			public class AttribsSByte : AttribsMemberIntBase<SByte>
			{
				public AttribsSByte(string keyName, SByte defaultVal, NumEncoding numEncoding = NumEncoding.Hex) :
					base(keyName, defaultVal, numEncoding) {; }
				protected override void AssignFromString(string s) { Set(AttribFormat.ConvertStringToSByte(s, m_numEncoding)); }
			}

			public class AttribsSingle : AttribsMemberBase<Single>
			{
				public AttribsSingle(string keyName, Single defaultVal) :
					base(keyName, defaultVal) {; }
				public override string ToString() { return AttribFormat.ConvertFloatToString(Get()); }
				protected override void AssignFromString(string s) { Set(AttribFormat.ConvertStringToFloat(s)); }
				public static implicit operator decimal(AttribsSingle t) { return Convert.ToDecimal(t.Value); }
			}

			public class AttribsDouble : AttribsMemberBase<Double>
			{
				public AttribsDouble(string keyName, Double defaultVal) :
					base(keyName, defaultVal) {; }
				public override string ToString() { return AttribFormat.ConvertFloatToString(Get()); }
				protected override void AssignFromString(string s) { Set(AttribFormat.ConvertStringToDouble(s)); }
				public static implicit operator decimal(AttribsDouble t) { return Convert.ToDecimal(t.Value); }
			}

			public class AttribsBoolean : AttribsMemberBase<Boolean>
			{
				public AttribsBoolean(string keyName, Boolean defaultVal) :
					base(keyName, defaultVal) {; }
				public override string ToString() { return AttribFormat.ConvertBoolToString(Get()); }
				protected override void AssignFromString(string s) { Set(AttribFormat.ConvertStringToBool(s)); }
			}

			public class AttribsDateTime : AttribsMemberBase<DateTime>
			{
				public AttribsDateTime(string keyName, DateTime defaultVal) :
					base(keyName, defaultVal) {; }
				public override string ToString() { return AttribFormat.ConvertDateTimeToString(Get()); }
				protected override void AssignFromString(string s) { Set(AttribFormat.ConvertStringToDateTime(s)); }
			}

			public class AttribsMemberEnum<T> : AttribsMemberBase<T>, IComparable<T> where T : Enum
			{
				public AttribsMemberEnum(string keyName, T defaultVal) :
					base(keyName, defaultVal)
				{ m_enum_t = typeof(T); }

				public System.Type m_enum_t = null;
				public override string ToString() { return System.Enum.Format(m_enum_t, Get(), "x"); }
				protected override void AssignFromString(string s)
				{
					int enumInt = AttribFormat.ConvertStringToInt32(s);
					// CR 29603 - JMF 2020.July.27 - First test to see if the value for an enum exists in client version of attributes.
					// If not, it likely means that the client is out of date. So add Warning Event Log informing user that an update might be needed to keep host and client in sync.
					if (m_enum_t.IsEnumDefined(enumInt))
					{
						Set((T)Enum.Parse(m_enum_t, enumInt.ToString()));
					}
					else
					{
						string message = $"Problem: AttribsMemberEnum::AssignFromString() - Inavlid Enum Value. Type \"{m_enum_t.ToString()}\" has no defined value \"{enumInt}\"\r\nThis could be due to the Host being at more recent version. Consider updating";
						try
						{
							Process proc = System.Diagnostics.Process.GetCurrentProcess();
							string eventSource = $"{proc?.ProcessName ?? ""}.SpdAttribsCS";
							EventLog logEntry = new EventLog("Application", ".", eventSource);

							logEntry.WriteEntry(message, EventLogEntryType.Warning, 0);
						}
						catch (Exception ex)
						{
							Trace.WriteLine($"Trying to write Warning to Event Log message: '{message}' and hit exception: {ex.ToString()}");
						}
					}
				}
				protected override void AssignFromObject(object ovalue)
				{
					Set((T)System.Enum.Parse(m_enum_t, ovalue.ToString()));
				}

				public static implicit operator Int32(AttribsMemberEnum<T> t)
				{
					return AttribFormat.ConvertStringToInt32(t.ToString(), NumEncoding.Hex);
				}
				public static implicit operator T(AttribsMemberEnum<T> t)
				{
					return t.Get();
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

				public int CompareTo(T other)
				{
					return this.Get().CompareTo(other);
				}

				public System.Enum EVal
				{
					get { return Get(); }
					set { Set((T)value); }
				}

				public System.Type EnumType
				{
					get { return m_enum_t; }
				}
			}

			public class AttribsMemberDWORDList : AttribsMemberBase<List<UInt32>>, IList<UInt32>
			{
				public AttribsMemberDWORDList(string keyName, List<UInt32> defaultVal) :
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

					foreach (UInt32 o in this.Value)
					{
						xml_writer.WriteElementString("N", String.Format("{0:x}", (UInt32)o));
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
					this.Value.Clear();
					foreach (System.Xml.XmlNode child in node.FirstChild.ChildNodes)
					{
						this.Value.Add(AttribFormat.ConvertStringToUInt32(child.InnerXml, NumEncoding.Hex));
					}
				}
				protected override void AssignFromObject(object ovalue)
				{
					this.Value.Clear();
					this.Value.AddRange((IEnumerable<UInt32>)ovalue);
				}

				public int Count => ((ICollection<uint>)Value).Count;

				public bool IsReadOnly => ((ICollection<uint>)Value).IsReadOnly;

				public uint this[int index] { get => ((IList<uint>)Value)[index]; set { ((IList<uint>)Value)[index] = value; isModified = true; } }

				public int IndexOf(uint item)
				{
					return ((IList<uint>)Value).IndexOf(item);
				}

				public void Insert(int index, uint item)
				{
					((IList<uint>)Value).Insert(index, item);
					isModified = true;
				}

				public void RemoveAt(int index)
				{
					((IList<uint>)Value).RemoveAt(index);
					isModified = true;
				}

				public void Add(uint item)
				{
					((ICollection<uint>)Value).Add(item);
					isModified = true;
				}

				public void Clear()
				{
					if (Count > 0)
					{
						((ICollection<uint>)Value).Clear();
						isModified = true;
					}
				}

				public bool Contains(uint item)
				{
					return ((ICollection<uint>)Value).Contains(item);
				}

				public void CopyTo(uint[] array, int arrayIndex)
				{
					((ICollection<uint>)Value).CopyTo(array, arrayIndex);
				}

				public bool Remove(uint item)
				{
					isModified = true;
					return ((ICollection<uint>)Value).Remove(item);
				}

				public IEnumerator<uint> GetEnumerator()
				{
					return ((IEnumerable<uint>)Value).GetEnumerator();
				}

				IEnumerator IEnumerable.GetEnumerator()
				{
					return ((IEnumerable)Value).GetEnumerator();
				}

				public override bool IsModified => Count > 0 || base.IsModified;
			}

			public class AttribsMemberStringList : AttribsMemberBase<List<string>>, IList<string>
			{
				public AttribsMemberStringList(string keyName, List<string> defaultVal) :
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

					foreach (string o in this.Value)
					{
						xml_writer.WriteElementString("N", o);
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
					this.Value.Clear();
					if (!node.HasChildNodes) return;
					foreach (System.Xml.XmlNode child in node.FirstChild.ChildNodes)
					{
						this.Value.Add(child.InnerText);
					}
				}
				protected override void AssignFromObject(object ovalue)
				{
					this.Value.Clear();
					this.Value.AddRange((IEnumerable<string>)ovalue);
				}

				public int Count => ((ICollection<string>)Value).Count;

				public bool IsReadOnly => ((ICollection<string>)Value).IsReadOnly;

				public string this[int index] { get => ((IList<string>)Value)[index]; set { ((IList<string>)Value)[index] = value; isModified = true; } }

				public int IndexOf(string item)
				{
					return ((IList<string>)Value).IndexOf(item);
				}

				public void Insert(int index, string item)
				{
					((IList<string>)Value).Insert(index, item);
					isModified = true;
				}

				public void RemoveAt(int index)
				{
					((IList<string>)Value).RemoveAt(index);
					isModified = true;
				}

				public void Add(string item)
				{
					((ICollection<string>)Value).Add(item);
					isModified = true;
				}

				public void Clear()
				{
					if (Count > 0)
					{
						((ICollection<string>)Value).Clear();
						isModified = true;
					}
				}

				public bool Contains(string item)
				{
					return ((ICollection<string>)Value).Contains(item);
				}

				public void CopyTo(string[] array, int arrayIndex)
				{
					((ICollection<string>)Value).CopyTo(array, arrayIndex);
				}

				public bool Remove(string item)
				{
					isModified = true;
					return ((ICollection<string>)Value).Remove(item);
				}

				public IEnumerator<string> GetEnumerator()
				{
					return ((IEnumerable<string>)Value).GetEnumerator();
				}

				IEnumerator IEnumerable.GetEnumerator()
				{
					return ((IEnumerable)Value).GetEnumerator();
				}

				public override bool IsModified => Count > 0 || base.IsModified;
			}

			// AttribsMemberByteArrayList - theoretically, replaces the AttribsMemberGenericList<T>, where
			// T = byte[]. 
			// It allows for xml to be structured in the same manner as the past, but wraps the List<byte[]>
			// object vs List<AttribsMemberBuffer> for more uniform C# usage.
			// Created for backward compatibility with licensing code, This type was not actively used in 
			// SPDE at the time this was created.
			//
			// Implementation notes: Basically copied AttribsMemberDWORDList class and replace instances of 
			// uint[] with byte[]. Then, worked on ToString() and AssignFromXmlNode() methods to meet
			// requirements.
			//
			// SLB 10.apr.2025 CR.33850; Added/Created.
			public class AttribsMemberByteArrayList : AttribsMemberBase<List<byte[]>>, IList<byte[]>
			{
				public AttribsMemberByteArrayList(string keyName, List<byte[]> defaultVal) :
					base(keyName, defaultVal)
				{
					;
				}
				public override string ToString()
				{
					System.IO.StringWriter string_writer = new System.IO.StringWriter();
					System.Xml.XmlTextWriter xml_writer = new System.Xml.XmlTextWriter(string_writer);

					// write the container start tag
					xml_writer.WriteStartElement("Vector");

					// write a text node so that we don't get a class tag that looks like <tag/> and instead we get <tag></tag> (the c++ version of the attribs does not use a legitimate xml parser)
					xml_writer.WriteWhitespace(" ");

					foreach (byte[] o in this.Value)
					{
						// use AttribsMemberBuffer to generate xml for byte[] entry.
						AttribsMemberBuffer attrib_buf = new AttribsMemberBuffer("CBuffer", new byte[0]);
						attrib_buf.Value = o;
						SpdAttribsBase.WriteElementData(xml_writer, "N", attrib_buf.ToString());
					}

					// write the container close tag
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
					this.Value.Clear();
					// SLB; Do some xml validation before continuing.
					if (node.Name != this.m_KeyName) return;
					if (node.ChildNodes.Count != 1) return;
					node = node.FirstChild;
					if (node.Name != "Vector") return;

					AttribsMemberBuffer attrib_buf = new AttribsMemberBuffer("CBuffer", new byte[0]);
					foreach (System.Xml.XmlNode child in node.ChildNodes)
					{
						// Valid entries will have a node name of "N".
						if (child.Name != "N") continue;

						// use AttribsMemberBuffer object to handle xml
						// deconstruction for each byte[] node in the child list.
						attrib_buf.XVal = child.FirstChild;
						this.Value.Add(attrib_buf.TVal);
					}
				}
				protected override void AssignFromObject(object ovalue)
				{
					this.Value.Clear();
					this.Value.AddRange((IEnumerable<byte[]>)ovalue);
				}

				public int Count => ((ICollection<byte[]>)Value).Count;

				public bool IsReadOnly => ((ICollection<byte[]>)Value).IsReadOnly;

				public byte[] this[int index] { get => ((IList<byte[]>)Value)[index]; set { ((IList<byte[]>)Value)[index] = value; isModified = true; } }

				public int IndexOf(byte[] item)
				{
					return ((IList<byte[]>)Value).IndexOf(item);
				}

				public void Insert(int index, byte[] item)
				{
					((IList<byte[]>)Value).Insert(index, item);
					isModified = true;
				}

				public void RemoveAt(int index)
				{
					((IList<byte[]>)Value).RemoveAt(index);
					isModified = true;
				}

				public void Add(byte[] item)
				{
					((ICollection<byte[]>)Value).Add(item);
					isModified = true;
				}

				public void Clear()
				{
					if (Count > 0)
					{
						((ICollection<byte[]>)Value).Clear();
						isModified = true;
					}
				}

				public bool Contains(byte[] item)
				{
					return ((ICollection<byte[]>)Value).Contains(item);
				}

				public void CopyTo(byte[][] array, int arrayIndex)
				{
					((ICollection<byte[]>)Value).CopyTo(array, arrayIndex);
				}

				public bool Remove(byte[] item)
				{
					isModified = true;
					return ((ICollection<byte[]>)Value).Remove(item);
				}

				public IEnumerator<byte[]> GetEnumerator()
				{
					return ((IEnumerable<byte[]>)Value).GetEnumerator();
				}

				IEnumerator IEnumerable.GetEnumerator()
				{
					return ((IEnumerable)Value).GetEnumerator();
				}

				public override bool IsModified => Count > 0 || base.IsModified;
			}

			/* class AttribsMemberBuffer
			 * 
			 * SLB 14.apr.2025 CR.33850.V01; Removed ToInt() from this class, now that the list<byte[]> 
			 * attribs data is wrapped in a different manner (no longer contains the data, internally,
			 * as AttribsMemberBuffer objects). 
			 * ToInt() was very specific to the License Key's "layout" buffers and was only added to
			 * cleanup consumer code. Removing will have no repercussions.
			 */
			public class AttribsMemberBuffer : AttribsMemberBase<byte[]>
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

					byte[] data = Get();
					xml_writer.WriteElementString("Size", data.Length.ToString());
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

				/* AssignFromXmlNode().
				 * 
				 * SLB 02.apr.2025 CR.33850; Do not assume that desired childnode is 
				 * the 2nd node. 
				 * SLB 14.apr.2025 CR.33850.V01; Now handles being passed different 
				 * heirarchies being passed in.
				 * -----------------------------------------------------------------*/
				protected override void AssignFromXmlNode(System.Xml.XmlNode node)
				{
					// SLB CR.33850.V01; Depending on how the AttribsMemberBuffer is being
					// stored, as member of the attribs class or as an item in a
					// collection, the passed in node will differ. 
					// As long as the "CBuffer" node can be found, the node can be 
					// assigned.
					while (node.Name != "CBuffer" && node.HasChildNodes) node = node.FirstChild; // CR.33850.V01; added.
					if (node.Name != "CBuffer") return;                             // CR.33850.V01; added.

					// Search for correct tagname ("Data") in childnodes list.
					foreach (System.Xml.XmlNode child in node.ChildNodes)           // CR.33850; added for() {}.
					{
						if (child.Name != "Data") continue;                         // CR.33850; added.

						Set(AttribFormat.ConvertHexToBytes(child.InnerXml));        // CR.33850; modified.
						return;
					}
				}

				protected override void AssignFromObject(object ovalue)
				{
					//Not implemented
				}

				public byte this[int index]
				{
					get
					{
						return m_tVal[index];
					}
					set
					{
						m_tVal[index] = value;
						isModified = true;
					}
				}

				public int Length { get { return m_tVal.Length; } }					// CR.33850; modified.

				// CR.33850; Added implicit operators to make consuming code cleaner.
				public static implicit operator byte[](AttribsMemberBuffer o) => o.m_tVal; // CR.33850; added.
			}


			public class AttribsMemberGenericList<T> : AttribsMemberBase<List<T>>, IList<T>
			{
				public AttribsMemberGenericList(string keyName, List<T> defaultVal) :
					base(keyName, defaultVal)
				{

				}

				public override string ToString()
				{
					System.IO.StringWriter string_writer = new System.IO.StringWriter();
					System.Xml.XmlTextWriter xml_writer = new System.Xml.XmlTextWriter(string_writer);

					// write the class start tag
					xml_writer.WriteStartElement("Vector");

					// write a text node so that we don't get a class tag that looks like <tag/> and instead we get <tag></tag> (the c++ version of the attribs does not use a legitimate xml parser)
					xml_writer.WriteWhitespace(" ");

					List<T> tmpList = Get();
					if (tmpList.Count > 0)
					{
						object parallelLock = new object();
						System.Collections.Concurrent.ConcurrentDictionary<int, string> concurrentDict = new System.Collections.Concurrent.ConcurrentDictionary<int, string>();
						System.Threading.Tasks.Parallel.For(
							0,
							tmpList.Count,
							index =>
							{
								T entry = default(T);
								lock (parallelLock)
								{
									entry = tmpList[index];
								}
								concurrentDict.AddOrUpdate(
							index,
							typeof(Solimar.Licensing.Attribs.SpdAttribsBase).IsInstanceOfType(entry) ? entry.ToString() : AttribFormat.ConvertTToString(entry),
							(existingId, existingValue) => existingValue);
							}
						);
						foreach (var obj in concurrentDict.Values)
						{
							xml_writer.WriteStartElement("N");
							xml_writer.WriteRaw(obj);
							xml_writer.WriteEndElement();
						}
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

				/* AssignFromXmlNode()
				 * 
				 * SLB 02.apr.2025 CR.33850; Fix for handling lists of AttribsMemberBuffer
				 * objects. They are a special case that don't fall into either SpdAttribsBase
				 * or simple type conversion.
				 * SLB 14.apr.2025 CR.33850.V01; Added some XmlNode validation.
				 * -----------------------------------------------------------------*/
				protected override void AssignFromXmlNode( 
					System.Xml.XmlNode node)
				{
					this.Value.Clear();

					// CR.33850.V01; Do some xml validation before continuing.
					if (node.Name != this.m_KeyName) return;
					if (node.ChildNodes.Count != 1) return;
					node = node.FirstChild;
					if (node.Name != "Vector") return;

					if (typeof(T).IsSubclassOf(typeof(Solimar.Licensing.Attribs.SpdAttribsBase)))
					{
						object parallelLock = new object();
						System.Collections.Concurrent.ConcurrentDictionary<int, object> concurrentDict = new System.Collections.Concurrent.ConcurrentDictionary<int, object>();
						System.Threading.Tasks.Parallel.For(
							0,
							node.ChildNodes.Count,                                  // CR.33850.V01; modified.
							index =>
							{
								System.Xml.XmlNode xmlNode = null;
								lock (parallelLock)
								{
									xmlNode = node.ChildNodes[index];               // CR.33850.V01; modified.
								}
								// create a new attribs class of type typeof(T)
								SpdAttribsBase attribsClass = InitializeAttribsBaseOfType(typeof(T), xmlNode.FirstChild);
								concurrentDict.AddOrUpdate(index, attribsClass, (existingId, existingValue) => existingValue);
							}
						);
						foreach (var obj in concurrentDict.Values)
						{
							this.Value.Add((T)obj);
						}
					}
					else if (typeof(T) == typeof(AttribsMemberBuffer))				// CR.33850; added else if () {}
					{
						// AttribsMemberBuffer/AttribsMemberByteArrayList is a
						// special case. Though it manages a list of byte[], it's
						// underlying type is not a "basic" type.
						// If performance is an issue, using the parallel task approach,
						// use in SpdAttribBase case, would be a starting point.
						foreach (System.Xml.XmlNode child in node.FirstChild.ChildNodes)
						{
							// initialize attribs_buf from xmlnode.
							AttribsMemberBuffer attribs_buf = new AttribsMemberBuffer("CBuffer", new byte[0]);
							attribs_buf.XVal = child.FirstChild;
							
							// add a new entry into the Value list.
							object obj = attribs_buf;
							this.Value.Add((T)obj);
						}
					}
					else
					{
						foreach (System.Xml.XmlNode child in node.ChildNodes)       // CR.33850.V01; modified.
						{
							this.Value.Add((T)AttribFormat.ConvertStringToT(typeof(T), child.InnerXml));
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

					this.Value.Clear();
					if (typeof(T).IsSubclassOf(typeof(SpdAttribsBase)))
					{
						for (int idx = 0; idx < valueArray.Length; idx++)
						{
							// create a new attribs class of type typeof(T)
							object attribsBase = InitializeAttribsBaseOfType(typeof(T), valueArray.GetValue(idx));
							// add the attribs class to the array
							this.Value.Add((T)attribsBase);
						}
					}
					else
					{
						for (int idx = 0; idx < valueArray.Length; idx++)
						{
							//this.Value.Add(AttribFormat.ConvertStringToT(typeof(T),child.InnerXml));
							this.Value.Add((T)valueArray.GetValue(idx));
						}
					}
				}

				public int Count => ((ICollection<T>)Value).Count;

				public bool IsReadOnly => ((ICollection<T>)Value).IsReadOnly;

				public T this[int index] { get => ((IList<T>)Value)[index]; set { ((IList<T>)Value)[index] = value; isModified = true; } }

				public int IndexOf(T item)
				{
					return ((IList<T>)Value).IndexOf(item);
				}

				public void Insert(int index, T item)
				{
					((IList<T>)Value).Insert(index, item);
					isModified = true;
				}

				public void RemoveAt(int index)
				{
					((IList<T>)Value).RemoveAt(index);
					isModified = true;
				}

				public void Add(T item)
				{
					((ICollection<T>)Value).Add(item);
					isModified = true;
				}

				public void Clear()
				{
					if (Count > 0)
					{
						((ICollection<T>)Value).Clear();
						isModified = true;
					}
				}

				public bool Contains(T item)
				{
					return ((ICollection<T>)Value).Contains(item);
				}

				public void CopyTo(T[] array, int arrayIndex)
				{
					((ICollection<T>)Value).CopyTo(array, arrayIndex);
				}

				public bool Remove(T item)
				{
					isModified = true;
					return ((ICollection<T>)Value).Remove(item);
				}

				public IEnumerator<T> GetEnumerator()
				{
					return ((IEnumerable<T>)Value).GetEnumerator();
				}

				IEnumerator IEnumerable.GetEnumerator()
				{
					return ((IEnumerable)Value).GetEnumerator();
				}
				public override bool IsModified => Count > 0 || base.IsModified;
			}

			public class AttribsMemberStreamlinedList<T> : AttribsMemberBase<List<T>>, IList<T>
			{
				public AttribsMemberStreamlinedList(string keyName, List<T> defaultVal) :
					base(keyName, defaultVal)
				{
				}

				public override string ToString()
				{
					System.IO.StringWriter string_writer = new System.IO.StringWriter();
					System.Xml.XmlTextWriter xml_writer = new System.Xml.XmlTextWriter(string_writer);

					List<T> tmpList = Get();
					if (tmpList.Count > 0)
					{
						object parallelLock = new object();
						System.Collections.Concurrent.ConcurrentDictionary<int, string> concurrentDict = new System.Collections.Concurrent.ConcurrentDictionary<int, string>();
						System.Threading.Tasks.Parallel.For(
							0,
							tmpList.Count,
							index =>
							{
								T entry = default(T);
								lock (parallelLock)
								{
								entry = tmpList[index];
								}
								string addValue = typeof(Solimar.Licensing.Attribs.SpdAttribsBase).IsInstanceOfType(entry) ? ((Solimar.Licensing.Attribs.SpdAttribsBase)(object)entry).ToStringNoClassTag() : AttribFormat.ConvertTToString(entry);
								concurrentDict.AddOrUpdate(
										index,
										addValue,
										(existingId, existingValue) => existingValue);
							});
						foreach (var obj in concurrentDict.Values)
						{
							xml_writer.WriteStartElement("N");
							xml_writer.WriteRaw(obj);
							xml_writer.WriteEndElement();
						}
					}

					return string_writer.ToString();
				}
				protected override void AssignFromString(string s)
				{
					if (s.Length == 0) return;
					AssignFromXmlNode(XMLDocumentHelper.GetDocumentElementFromString(s));
				}
				protected override void AssignFromXmlNode(System.Xml.XmlNode node)
				{
					this.Value.Clear();
					System.Type valType = typeof(T);
					if (valType.IsSubclassOf(typeof(Solimar.Licensing.Attribs.SpdAttribsBase)))
					{
						object parallelLock = new object();
						System.Collections.Concurrent.ConcurrentDictionary<int, object> concurrentDict = new System.Collections.Concurrent.ConcurrentDictionary<int, object>();
						//System.Threading.Tasks.Parallel.For(
						//    0,
						//    node.ChildNodes.Count,
						//    index =>
						//    {
						//        System.Xml.XmlNode xmlNode = null;
						//        lock (parallelLock)
						//        {
						//            xmlNode = node.ChildNodes[index];
						//        }
						//        // create a new attribs class of type valType
						//        SpdAttribsBase attribsClass = InitializeAttribsBaseOfType(valType, xmlNode); // We are no longer using class name in slim lists
						//        concurrentDict.AddOrUpdate(index, attribsClass, (existingId, existingValue) => existingValue);
						//    }
						//);
						for (int index = 0; index < node.ChildNodes.Count; index++)
						{
							System.Xml.XmlNode xmlNode = null;
							lock (parallelLock)
							{
							xmlNode = node.ChildNodes[index];
							}
							// create a new attribs class of type valType
							SpdAttribsBase attribsClass = InitializeAttribsBaseOfType(valType, xmlNode); // We are no longer using class name in slim lists
							concurrentDict.AddOrUpdate(index, attribsClass, (existingId, existingValue) => existingValue);
						}
						foreach (var obj in concurrentDict.Values)
						{
							this.Value.Add((T)obj);
						}
					}
					else
					{
						foreach (System.Xml.XmlNode child in node.ChildNodes) // We are no longer using class name in slim lists
						{
							this.Value.Add((T)AttribFormat.ConvertStringToT(valType, child.InnerXml));
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

					this.Value.Clear();
					System.Type valType = typeof(T);
					if (valType.IsSubclassOf(typeof(SpdAttribsBase)))
					{
						for (int idx = 0; idx < valueArray.Length; idx++)
						{
							// create a new attribs class of type valType
							object attribsClass = InitializeAttribsBaseOfType(valType, valueArray.GetValue(idx));
							// add the attribs class to the array
							this.Value.Add((T)attribsClass);
						}
					}
					else
					{
						for (int idx = 0; idx < valueArray.Length; idx++)
						{
							//this.Value.Add(AttribFormat.ConvertStringToT(valType,child.InnerXml));
							this.Value.Add((T)valueArray.GetValue(idx));
						}
					}
				}

				public int Count => ((ICollection<T>)Value).Count;

				public bool IsReadOnly => ((ICollection<T>)Value).IsReadOnly;

				public T this[int index] { get => ((IList<T>)Value)[index]; set { ((IList<T>)Value)[index] = value; isModified = true; } }

				public int IndexOf(T item)
				{
					return ((IList<T>)Value).IndexOf(item);
				}

				public void Insert(int index, T item)
				{
					((IList<T>)Value).Insert(index, item);
					isModified = true;
				}

				public void RemoveAt(int index)
				{
					((IList<T>)Value).RemoveAt(index);
					isModified = true;
				}

				public void Add(T item)
				{
					((ICollection<T>)Value).Add(item);
					isModified = true;
				}

				public void Clear()
				{
					if (Count > 0)
					{
						((ICollection<T>)Value).Clear();
						isModified = true;
					}
				}

				public bool Contains(T item)
				{
					return ((ICollection<T>)Value).Contains(item);
				}

				public void CopyTo(T[] array, int arrayIndex)
				{
					((ICollection<T>)Value).CopyTo(array, arrayIndex);
				}

				public bool Remove(T item)
				{
					isModified = true;
					return ((ICollection<T>)Value).Remove(item);
				}

				public IEnumerator<T> GetEnumerator()
				{
					return ((IEnumerable<T>)Value).GetEnumerator();
				}

				IEnumerator IEnumerable.GetEnumerator()
				{
					return ((IEnumerable)Value).GetEnumerator();
				}
				public override bool IsModified => Count > 0 || base.IsModified;
			}

			public class AttribsMemberGenericMap<K, V> : AttribsMemberBase<Dictionary<K, V>>, IDictionary<K, V>
			{
				public AttribsMemberGenericMap(string keyName, Dictionary<K, V> defaultVal) :
					base(keyName, defaultVal)
				{
				}

				public override string ToString()
				{
					System.IO.StringWriter string_writer = new System.IO.StringWriter();
					System.Xml.XmlTextWriter xml_writer = new System.Xml.XmlTextWriter(string_writer);

					// write the class start tag
					xml_writer.WriteStartElement("Map");

					// write a text node so that we don't get a class tag that looks like <tag/> and instead we get <tag></tag> (the c++ version of the attribs does not use a legitimate xml parser)
					xml_writer.WriteWhitespace(" ");

					foreach (KeyValuePair<K, V> i in this.Value)
					{
						xml_writer.WriteStartElement("N");

						// write the Key string
						xml_writer.WriteStartElement("K");
						if (typeof(Solimar.Licensing.Attribs.SpdAttribsBase).IsInstanceOfType(i.Key))
							xml_writer.WriteRaw(i.Key.ToString());
						else
							xml_writer.WriteRaw(AttribFormat.ConvertTToString<K>(i.Key));
						xml_writer.WriteEndElement();

						// write the Value string
						xml_writer.WriteStartElement("V");
						if (typeof(Solimar.Licensing.Attribs.SpdAttribsBase).IsInstanceOfType(i.Value))
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


				/* AssignFromXmlNode()
				 * 
				 * SLB 11.apr.2025 CR.33850.V01; Added some XmlNode validation.
				 * -----------------------------------------------------------------*/
				protected override void AssignFromXmlNode(System.Xml.XmlNode node)
				{
					this.Value.Clear();
					foreach (System.Xml.XmlNode child in node.FirstChild.ChildNodes)
					{
						// CR.33850.V01; do some validation of node structure.
						if (child.Name != "N") continue;
						if (child.ChildNodes.Count != 2) continue;

						XmlNode kNode = child.ChildNodes[0];
						if (kNode.Name != "K") continue;
						XmlNode vNode = child.ChildNodes[1];
						if (vNode.Name != "V") continue;

						object key;
						object val;
						System.Type keyType = typeof(K);
						System.Type valType = typeof(V);

						if (keyType.IsSubclassOf(typeof(Solimar.Licensing.Attribs.SpdAttribsBase)))
						{
							// create a new attribs class of type m_ValueType
							key = InitializeAttribsBaseOfType(keyType, kNode.FirstChild); // CR.33850.V01; modified.
						}
						else
						{
							key = AttribFormat.ConvertStringToT(keyType, kNode.InnerXml); // CR.33850.V01; modified.
						}

						if (valType.IsSubclassOf(typeof(Solimar.Licensing.Attribs.SpdAttribsBase)))
						{
							if (!vNode.HasChildNodes) continue;                     // CR.33850.V01; added.

							// create a new attribs class of type m_ValueType
							val = InitializeAttribsBaseOfType(valType, vNode.FirstChild); // CR.33850.V01; modified.
						}
						else
						{
							val = AttribFormat.ConvertStringToT(valType, vNode.InnerXml); // CR.33850.V01; modified.
						}

						this.Value.Add((K)key, (V)val);
					}
				}
				protected override void AssignFromObject(object ovalue)
				{
					this.Value.Clear();
					const int TAG = 0;
					const int VALUE = 1;
					object[,] tagValueArray = (object[,])ovalue;
					//object[,] tagValueArray = {{"tag","tag","tag"},{"value","value","value"}};
					//Dictionary<K, V> stream_map = new Dictionary<K, V>();
					int tagValuePairs = tagValueArray.Length / 2;
					for (int idx = 0; idx < tagValuePairs; idx++)
					{
						//stream_map[tagValueArray[idx, TAG].ToString()] = tagValueArray[idx, VALUE];
						//}



						//foreach (System.Xml.XmlNode child in node.FirstChild.ChildNodes)
						//{
						object key;
						object val;
						System.Type keyType = typeof(K);
						System.Type valType = typeof(V);

						if (keyType.IsSubclassOf(typeof(Solimar.Licensing.Attribs.SpdAttribsBase)))
						{
							// create and save new attribs class of key type
							key = InitializeAttribsBaseOfType(keyType, tagValueArray[idx, VALUE]);
						}
						else
						{
							key = tagValueArray[idx, TAG];//AttribFormat.ConvertStringToT(m_KeyType,child.ChildNodes[0].InnerXml);
						}

						if (valType.IsSubclassOf(typeof(Solimar.Licensing.Attribs.SpdAttribsBase)))
						{
							// create and save new attribs class of value type
							val = InitializeAttribsBaseOfType(valType, tagValueArray[idx, VALUE]);
						}
						else
						{
							val = AttribFormat.ConvertStringToT(keyType, tagValueArray[idx, VALUE].ToString());//m_KeyType,child.ChildNodes[1].InnerXml);
						}

						this.Value.Add((K)key, (V)val);
					}
				}

				public bool ContainsKey(K key)
				{
					return ((IDictionary<K, V>)Value).ContainsKey(key);
				}

				public void Add(K key, V value)
				{
					((IDictionary<K, V>)Value).Add(key, value);
					isModified = true;
				}

				public bool Remove(K key)
				{
					isModified = true;
					return ((IDictionary<K, V>)Value).Remove(key);
				}

				public bool TryGetValue(K key, out V value)
				{
					return ((IDictionary<K, V>)Value).TryGetValue(key, out value);
				}

				public void Add(KeyValuePair<K, V> item)
				{
					((ICollection<KeyValuePair<K, V>>)Value).Add(item);
					isModified = true;
				}

				public void Clear()
				{
					if (Count > 0)
					{
						((ICollection<KeyValuePair<K, V>>)Value).Clear();
						isModified = true;
					}
				}

				public bool Contains(KeyValuePair<K, V> item)
				{
					return ((ICollection<KeyValuePair<K, V>>)Value).Contains(item);
				}

				public void CopyTo(KeyValuePair<K, V>[] array, int arrayIndex)
				{
					((ICollection<KeyValuePair<K, V>>)Value).CopyTo(array, arrayIndex);
				}

				public bool Remove(KeyValuePair<K, V> item)
				{
					isModified = true;
					return ((ICollection<KeyValuePair<K, V>>)Value).Remove(item);
				}

				public IEnumerator<KeyValuePair<K, V>> GetEnumerator()
				{
					return ((IEnumerable<KeyValuePair<K, V>>)Value).GetEnumerator();
				}

				IEnumerator IEnumerable.GetEnumerator()
				{
					return ((IEnumerable)Value).GetEnumerator();
				}

				public ICollection<K> Keys => ((IDictionary<K, V>)Value).Keys;

				public ICollection<V> Values => ((IDictionary<K, V>)Value).Values;

				public int Count => ((ICollection<KeyValuePair<K, V>>)Value).Count;

				public bool IsReadOnly => ((ICollection<KeyValuePair<K, V>>)Value).IsReadOnly;

				public V this[K key] { get => ((IDictionary<K, V>)Value)[key]; set { ((IDictionary<K, V>)Value)[key] = value; isModified = true; } }

				public override bool IsModified => Count > 0 || base.IsModified;
			}

			public class AttribsMemberStreamlinedMap<K, V> : AttribsMemberBase<Dictionary<K, V>>, IDictionary<K, V>
			{
				public AttribsMemberStreamlinedMap(string keyName, Dictionary<K, V> defaultVal) :
					base(keyName, defaultVal)
				{
				}

				public override string ToString()
				{
					System.IO.StringWriter string_writer = new System.IO.StringWriter();
					System.Xml.XmlTextWriter xml_writer = new System.Xml.XmlTextWriter(string_writer);

					foreach (KeyValuePair<K, V> i in (Dictionary<K, V>)Value)
					{
						xml_writer.WriteStartElement("N");

						// write the Key string
						xml_writer.WriteStartElement("K");
						if (typeof(Solimar.Licensing.Attribs.SpdAttribsBase).IsInstanceOfType(i.Key))
							xml_writer.WriteRaw(i.Key.ToString());
						else
							xml_writer.WriteRaw(AttribFormat.ConvertTToString(i.Key));
						xml_writer.WriteEndElement();

						// write the Value string
						xml_writer.WriteStartElement("V");
						if (typeof(Solimar.Licensing.Attribs.SpdAttribsBase).IsInstanceOfType(i.Value))
							xml_writer.WriteRaw(i.Value.ToString());
						else
							xml_writer.WriteRaw(AttribFormat.ConvertTToString(i.Value));
						xml_writer.WriteEndElement();

						xml_writer.WriteEndElement();
					}

					return string_writer.ToString();
				}
				protected override void AssignFromString(string s)
				{
					if (s.Length == 0) return;
					AssignFromXmlNode(XMLDocumentHelper.GetDocumentElementFromString(s));
				}

				/* AssignFromXmlNode()
				 * 
				 * SLB 11.apr.2025 CR.33850.V01; Added some XmlNode validation.
				 * -----------------------------------------------------------------*/
				protected override void AssignFromXmlNode(System.Xml.XmlNode node)
				{
					m_tVal.Clear();
					foreach (System.Xml.XmlNode child in node.ChildNodes)
					{
						// CR.33850.V01; do some validation of node structure.
						if (child.Name != "N") continue;
						if (child.ChildNodes.Count != 2) continue;

						XmlNode kNode = child.ChildNodes[0];
						if (kNode.Name != "K") continue;
						XmlNode vNode = child.ChildNodes[1];
						if (vNode.Name != "V") continue;

						object key;
						object val;
						System.Type keyType = typeof(K);
						System.Type valType = typeof(V);

						if (keyType.IsSubclassOf(typeof(Solimar.Licensing.Attribs.SpdAttribsBase)))
						{
							// create a new attribs class of type m_ValueType
							// Note: Slim maps no longer use class name node
							key = InitializeAttribsBaseOfType(keyType, kNode);      // CR.33850.V01; modified.
						}
						else
						{
							//key = Convert.ToUInt32(Convert.ToUInt64(child.ChildNodes[0].InnerXml, 10)); // This was needed when the engine sent as decimal instead of hex
							key = AttribFormat.ConvertStringToT(keyType, kNode.InnerXml); // CR.33850.V01; modified.
						}

						if (valType.IsSubclassOf(typeof(Solimar.Licensing.Attribs.SpdAttribsBase)))
						{
							// create a new attribs class of type m_ValueType
							// Note: Slim maps no longer use class name node
							val = InitializeAttribsBaseOfType(valType, vNode); // CR.33850.V01; modified.
						}
						else
						{
							val = AttribFormat.ConvertStringToT(valType, vNode.InnerXml);  // CR.33850.V01; modified.
						}

						m_tVal.Add((K)key, (V)val);
					}
					isModified = true;
				}
				protected override void AssignFromObject(object ovalue)
				{
					m_tVal.Clear();
					const int TAG = 0;
					const int VALUE = 1;
					object[,] tagValueArray = (object[,])ovalue;
					//object[,] tagValueArray = {{"tag","tag","tag"},{"value","value","value"}};
					//Dictionary<K, V> stream_map = new Dictionary<K, V>();
					int tagValuePairs = tagValueArray.Length / 2;
					for (int idx = 0; idx < tagValuePairs; idx++)
					{
						//stream_map[tagValueArray[idx, TAG].ToString()] = tagValueArray[idx, VALUE];
						//}

						//foreach (System.Xml.XmlNode child in node.FirstChild.ChildNodes)
						//{
						object key;
						object val;
						System.Type keyType = typeof(K);
						System.Type valType = typeof(V);

						if (keyType.IsSubclassOf(typeof(Solimar.Licensing.Attribs.SpdAttribsBase)))
						{
							// create and save new attribs class of key type
							key = InitializeAttribsBaseOfType(keyType, tagValueArray[idx, VALUE]);
						}
						else
						{
							key = tagValueArray[idx, TAG];//AttribFormat.ConvertStringToT(m_KeyType,child.ChildNodes[0].InnerXml);
						}

						if (valType.IsSubclassOf(typeof(Solimar.Licensing.Attribs.SpdAttribsBase)))
						{
							// create and save new attribs class of value type
							val = InitializeAttribsBaseOfType(valType, tagValueArray[idx, VALUE]);
						}
						else
						{
							val = AttribFormat.ConvertStringToT(valType, tagValueArray[idx, VALUE].ToString());//m_KeyType,child.ChildNodes[1].InnerXml);
						}
						m_tVal.Add((K)key, (V)val);
					}
					isModified = true;
				}

				public ICollection<K> Keys => ((IDictionary<K, V>)Value).Keys;

				public ICollection<V> Values => ((IDictionary<K, V>)Value).Values;

				public int Count => ((ICollection<KeyValuePair<K, V>>)Value).Count;

				public bool IsReadOnly => ((ICollection<KeyValuePair<K, V>>)Value).IsReadOnly;

				public V this[K key] { get => ((IDictionary<K, V>)Value)[key]; set { ((IDictionary<K, V>)Value)[key] = value; isModified = true; } }

				public bool ContainsKey(K key)
				{
					return ((IDictionary<K, V>)Value).ContainsKey(key);
				}

				public void Add(K key, V value)
				{
					((IDictionary<K, V>)Value).Add(key, value);
					isModified = true;
				}

				public bool Remove(K key)
				{
					isModified = true;
					return ((IDictionary<K, V>)Value).Remove(key);
				}

				public bool TryGetValue(K key, out V value)
				{
					return ((IDictionary<K, V>)Value).TryGetValue(key, out value);
				}

				public void Add(KeyValuePair<K, V> item)
				{
					((ICollection<KeyValuePair<K, V>>)Value).Add(item);
					isModified = true;
				}

				public void Clear()
				{
					if (Count > 0)
					{
						((ICollection<KeyValuePair<K, V>>)Value).Clear();
						isModified = true;
					}
				}

				public bool Contains(KeyValuePair<K, V> item)
				{
					return ((ICollection<KeyValuePair<K, V>>)Value).Contains(item);
				}

				public void CopyTo(KeyValuePair<K, V>[] array, int arrayIndex)
				{
					((ICollection<KeyValuePair<K, V>>)Value).CopyTo(array, arrayIndex);
				}

				public bool Remove(KeyValuePair<K, V> item)
				{
					isModified = true;
					return ((ICollection<KeyValuePair<K, V>>)Value).Remove(item);
				}

				public IEnumerator<KeyValuePair<K, V>> GetEnumerator()
				{
					return ((IEnumerable<KeyValuePair<K, V>>)Value).GetEnumerator();
				}

				IEnumerator IEnumerable.GetEnumerator()
				{
					return ((IEnumerable)Value).GetEnumerator();
				}

				public override bool IsModified => Count > 0 || base.IsModified;
			}

			public class SpdAttribsBase : System.ICloneable
			{
				protected string m_ClassName = null;
				protected string m_TagName = null;

				public SpdAttribsBase(string ClassName)
				{
					m_ClassName = ClassName;
					m_TagName = string.Empty;
				}

				//           public SpdAttribsBase(string className, string tagName, bool bDontAssignFromString)
				//           {
				//m_ClassName = className;
				//m_TagName = tagName;
				//           }

				public string GetSpdClassName()
				{
					return m_ClassName;
				}

				public string GetTagName()
				{
					return m_TagName;
				}

				public void Set(SpdAttribsBase setfrom)
				{
					this.AssignMembersFromStream(setfrom.Stream);
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
					return Serialize(true);
				}

				public string ToStringNoClassTag()
				{
					return Serialize(false);
				}

				private string Serialize(bool includeClassTag)
				{
					System.IO.StringWriter string_writer = new System.IO.StringWriter();
					System.Xml.XmlTextWriter xml_writer = new System.Xml.XmlTextWriter(string_writer);

					if (includeClassTag)
					{
						// write the class start tag
						xml_writer.WriteStartElement(this.m_ClassName);

						// write a text node so that we don't get a class tag that looks like <tag/> and instead we get <tag></tag> (the c++ version of the attribs does not use a legitimate xml parser)
						xml_writer.WriteWhitespace(" ");
					}

					// get a list of the members
					System.Reflection.MemberInfo[] members = this.GetType().GetMembers(System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.Public);
					// for each member
					foreach (System.Reflection.MemberInfo member in members)
					{
						// if the member is a field, and derives from AttribsMemberBase, 'serialize' it
						if (member.MemberType == System.Reflection.MemberTypes.Field)
						{
							if (TypeDerivesFromMemberBase(((System.Reflection.FieldInfo)member).FieldType))
							{
							IAttribsMemberBase attribs_member = (IAttribsMemberBase)(((System.Reflection.FieldInfo)member).GetValue(this));
							if (attribs_member.IsModified)
								WriteElementData(xml_writer, attribs_member.GetKeyName(), attribs_member.ToString());
							}
							else if (TypeDerivesFromAttribsBase(((System.Reflection.FieldInfo)member).FieldType))
							{
							SpdAttribsBase attribs_member = (SpdAttribsBase)(((System.Reflection.FieldInfo)member).GetValue(this));

							string tagName = attribs_member.GetTagName();
							string memberStream = attribs_member.ToString();
							if (string.IsNullOrWhiteSpace(tagName) || string.IsNullOrWhiteSpace(memberStream))
								continue; // We didn't used to write out classes with no tag name // throw new Exception($"Invalid Tag Name for {this.m_ClassName} - {attribs_member.m_ClassName}");
							WriteElementData(xml_writer, tagName, memberStream);
							}
						}
					}

					// write the class close tag
					if (includeClassTag)
						xml_writer.WriteEndElement();

					return string_writer.ToString();
				}

				/* WriteElementData().
				 * 
				 * SLB 11.apr.2025 CR.33850.V01; Made public so it could be used in 
				 * SpdAttribMember classes as well.
				 * -----------------------------------------------------------------*/
				public static void WriteElementData(                                // CR.33850.V01; modified.
					System.Xml.XmlTextWriter xml_writer,
					string elementName,
					string elementData)
				{
					xml_writer.WriteStartElement(elementName);
					xml_writer.WriteRaw(elementData);
					xml_writer.WriteEndElement();
				}

				public T AssignMembersFromStream<T>(string AttribsStream) where T:SpdAttribsBase
				{
					AssignMembersFromStream(AttribsStream);
					return (T)this;
				}

				public void AssignMembersFromStream(string AttribsStream)
				{
					if (AttribsStream == null || AttribsStream.Length == 0) return;
					AssignMembersFromXmlNode(XMLDocumentHelper.GetDocumentElementFromString(AttribsStream));
				}
				// Replacing AssignMembersFromXmlNode() where performance is an issue.
				public void AssignMembersFromObject(object o)
				{
					const int TAG = 0;
					const int VALUE = 1;
					object[,] tagValueArray = (object[,])o;
					//object[,] tagValueArray = {{"tag","tag","tag"},{"value","value","value"}};
					Dictionary<string, object> stream_map = new Dictionary<string, object>();
					int tagValuePairs = tagValueArray.Length / 2;
					for (int idx = 0; idx < tagValuePairs; idx++)
					{
						stream_map[tagValueArray[idx, TAG].ToString()] = tagValueArray[idx, VALUE];
					}

					// iterate over the class members, assign them values using the values provided in the value array
					System.Reflection.MemberInfo[] members = this.GetType().GetMembers(System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.Public);
					foreach (System.Reflection.MemberInfo member in members)
					{
						// if the member is a field, and derives from AttribsMemberBase, 'serialize' it
						if (member.MemberType == System.Reflection.MemberTypes.Field)
						{
							if (TypeDerivesFromMemberBase(((System.Reflection.FieldInfo)member).FieldType))
							{
							IAttribsMemberBase attribs_member = (IAttribsMemberBase)(((System.Reflection.FieldInfo)member).GetValue(this));
							string keyName = attribs_member.GetKeyName();
							if (stream_map.ContainsKey(keyName))
							{
								//Set XVal property which calls AssignFromXmlNode(), AssignFromXmlNode() is overridden in
								//complex container types.
								attribs_member.ZVal = (object)stream_map[keyName];
							}
							}
							else if (TypeDerivesFromAttribsBase(((System.Reflection.FieldInfo)member).FieldType))
							{
							SpdAttribsBase attribs_member = (SpdAttribsBase)(((System.Reflection.FieldInfo)member).GetValue(this));
							string keyName = attribs_member.GetTagName();
							if (stream_map.ContainsKey(keyName))
							{
								attribs_member.AssignMembersFromObject((object)stream_map[keyName]);
							}
							else if (stream_map.ContainsKey(attribs_member.m_ClassName))
							{
								attribs_member.AssignMembersFromObject((object)stream_map[attribs_member.m_ClassName]);
							}
							}
						}
					}
				}

				public void AssignMembersFromXmlNode(System.Xml.XmlNode node)
				{
					Dictionary<string, System.Xml.XmlNode> stream_map = new Dictionary<string, System.Xml.XmlNode>();

					foreach (System.Xml.XmlNode child in node.ChildNodes)
					{
						if (child.NodeType == System.Xml.XmlNodeType.Element)
							stream_map[child.Name] = child;
					}

					// iterate over the class members, assign them values using the values provided in the stream
					System.Reflection.MemberInfo[] members = this.GetType().GetMembers(System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.Public);
					foreach (System.Reflection.MemberInfo member in members)
					{
						string memberName = member.Name;
						// if the member is a field, and derives from AttribsMemberBase, 'serialize' it
						if (member.MemberType == System.Reflection.MemberTypes.Field)
						{
							if (TypeDerivesFromMemberBase(((System.Reflection.FieldInfo)member).FieldType))
							{
							IAttribsMemberBase attribs_member = (IAttribsMemberBase)((System.Reflection.FieldInfo)member).GetValue(this);
							string keyName = attribs_member.GetKeyName();
							if (stream_map.ContainsKey(attribs_member.GetKeyName()))
							{
								attribs_member.XVal = stream_map[keyName];
							}
							}
							else if (TypeDerivesFromAttribsBase(((System.Reflection.FieldInfo)member).FieldType))
							{
							SpdAttribsBase attribs_class = (SpdAttribsBase)((System.Reflection.FieldInfo)member).GetValue(this);
							string tagName = attribs_class.GetTagName();
							if (stream_map.ContainsKey(tagName))
							{
								System.Xml.XmlNode nodeToPass = stream_map[tagName];
								nodeToPass = nodeToPass.FirstChild; // Skip over a node?
								attribs_class.AssignMembersFromXmlNode(nodeToPass);
							}
							else if (stream_map.ContainsKey(attribs_class.m_ClassName))
							{
								System.Xml.XmlNode nodeToPass = stream_map[attribs_class.m_ClassName];
								attribs_class.AssignMembersFromXmlNode(nodeToPass);
							}
							}
						}
					}
				}


				public static implicit operator string(SpdAttribsBase t)
				{
					return t.ToString();
				}

				public static bool TypeDerivesFromMemberBase(System.Type t)
				{
					if (t == null)
						return false;
					if (typeof(IAttribsMemberBase).IsAssignableFrom(t))
						return true;
					if (t == typeof(AttribsMemberBase<>))
						return true;
					return TypeDerivesFromMemberBase(t.BaseType);
				}

				public static bool TypeDerivesFromAttribsBase(System.Type t)
				{
					if (t == null)
						return false;
					if (t == typeof(SpdAttribsBase))
						return true;
					return TypeDerivesFromAttribsBase(t.BaseType);
				}

				internal static SpdAttribsBase CreateType(Type attribBaseType)
				{
					object createdObject = null;
					// CR 31153 - Most derived types now have a constructor with string and bool parameters
					// So try the default constructor first still, but check new constructor style next, if needed
					var attribConstructor = attribBaseType.GetConstructor(new System.Type[] { });
					if (attribConstructor != null)
					{
						createdObject = attribConstructor.Invoke(new object[] { });
					}
					else
					{
						attribConstructor = attribBaseType.GetConstructor(new System.Type[] { typeof(string), typeof(bool) });
						if (attribConstructor != null)
							createdObject = attribConstructor.Invoke(new object[] { "ignored", false });
					}

					if (createdObject is SpdAttribsBase attribsBase)
						return attribsBase;
					else
						return null;
				}

				#region ICloneable Members

				public object Clone()
				{
					SpdAttribsBase n = CreateType(this.GetType());
					if (n != null) n.Stream = this.Stream;
					return n;
				}

				#endregion
			}


			/* A class that knows how to convert between primitive 
			 * types and their SPD Attribs string representations */
			public class AttribFormat
			{
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
					if (typeof(AttribsMemberXmlString) == T)
						return new AttribsMemberXmlString("fromString", s);
					if (typeof(System.Enum) == T || typeof(System.Enum) == T.BaseType)
						return ConvertStringToInt32(s); // Engine wants enums serialized as int
														//return Enum.Parse(T, s);
					return null;
				}
				public static string ConvertTToString<T>(T value)
				{
					return ConvertTToString<T>(value, Type.GetTypeCode(typeof(T)));
				}
				public static string ConvertTToString<T>(T value, TypeCode typeCode, NumEncoding numEncoding = NumEncoding.Hex)
				{
					switch (typeCode)
					{
						case TypeCode.SByte:
						case TypeCode.Int16:
						case TypeCode.Int32:
						case TypeCode.Int64:
							return AttribFormat.ConvertIntToString(value, numEncoding);
						case TypeCode.Byte:
						case TypeCode.UInt16:
						case TypeCode.UInt32:
						case TypeCode.UInt64:
							return AttribFormat.ConvertUIntToString(value, numEncoding);
						case TypeCode.Single:
						case TypeCode.Double:
							return ConvertFloatToString(value);
						case TypeCode.Boolean:
							return ConvertBoolToString((bool)(object)value);
						case TypeCode.DateTime:
							return ConvertDateTimeToString((DateTime)(object)value);
						default:
							Type type = value.GetType();
							if (typeof(System.Enum) == type || typeof(System.Enum) == type.BaseType)
							return ConvertIntToString(value);  // Engine wants enums serialized as int
							Object valObj = value; // Need value typecast as object for remainging calls
							if (typeof(byte[]) == type)
							return ConvertBytesToHex((byte[])valObj);
							if (typeof(string) == type)
							return ConvertRawStringToString((string)valObj);
							if (typeof(AttribsMemberXmlString) == type)
							{
							// CR.Fix.32145.v01: Line below was "return (string)valObj;", which caused
							// exception "Unable to cast... to type 'System.String'".
							return valObj.ToString();
							}
							return null; // Should not hit this
					}
				}
				//public static string ConvertTToString(object o)
				//{
				//   System.Type T = o.GetType();
				//   if (typeof(UInt64) == T)
				//      return ConvertUIntToString<UInt64>((UInt64)o);
				//   if (typeof(UInt32) == T)
				//      return ConvertUIntToString<UInt32>((UInt32)o);
				//   if (typeof(UInt16) == T)
				//      return ConvertUIntToString<UInt16>((UInt16)o);
				//   if (typeof(Byte) == T)
				//      return ConvertByteToString((Byte)o);
				//   if (typeof(Int64) == T)
				//      return ConvertInt64ToString((Int64)o);
				//   if (typeof(Int32) == T)
				//      return ConvertInt32ToString((Int32)o);
				//   if (typeof(Int16) == T)
				//      return ConvertInt16ToString((Int16)o);
				//   if (typeof(SByte) == T)
				//      return ConvertSByteToString((SByte)o);
				//   if (typeof(float) == T)
				//      return ConvertFloatToString((float)o);
				//   if (typeof(double) == T)
				//      return ConvertDoubleToString((double)o);
				//   if (typeof(bool) == T)
				//      return ConvertBoolToString((bool)o);
				//   if (typeof(byte[]) == T)
				//      return ConvertBytesToHex((byte[])o);
				//   if (typeof(string) == T)
				//      return ConvertRawStringToString((string)o);
				//   if (typeof(AttribsMemberXmlString) == T)
				//      return (string)o;
				//   if (typeof(System.Enum) == T || typeof(System.Enum) == T.BaseType)
				//      return ConvertInt32ToString((Int32)o);  // Engine wants enums serialized as int
				//                                              //return Enum.GetName(T, o);
				//   return null;
				//}

				private static string GetCompositeFormatString(NumEncoding numEncoding)
				{
					return $"{{0:{(numEncoding == NumEncoding.Hex ? 'x' : 'd')}}}";
				}

				public static byte[] ConvertHexToBytes(string s)
				{
					if (s == null) return new byte[] { };

					byte[] ret = new byte[s.Length / 2];
					for (int i = 0; i < s.Length / 2; ++i)
					{
						ret[i] = Convert.ToByte(s.Substring(i * 2, 2), 16);
					}
					return ret;
				}
				public static string ConvertBytesToHex(byte[] bytes)
				{
					if (bytes == null || bytes.Length == 0) return "";

					System.Text.StringBuilder ret = new System.Text.StringBuilder(bytes.Length * 2, bytes.Length * 2);
					foreach (byte b in bytes)
					{
						ret.AppendFormat("{0:x2}", b);
					}
					return ret.ToString();
				}

				public static string ConvertIntToString<T>(T val, NumEncoding numEncoding = NumEncoding.Hex)
				{
					Int64 temp64 = Convert.ToInt64(val);
					return String.Format(GetCompositeFormatString(numEncoding), temp64);
				}

				public static string ConvertUIntToString<T>(T val, NumEncoding numEncoding = NumEncoding.Hex)
				{
					UInt64 temp64 = Convert.ToUInt64(val);
					return String.Format(GetCompositeFormatString(numEncoding), temp64);
				}
				public static UInt64 ConvertStringToUInt64(string s, NumEncoding numEncoding = NumEncoding.Hex)
				{
					if (string.IsNullOrWhiteSpace(s)) return 0; // CR 30619 - Check for empty string, as Convert.ToXXXX will throw an exception instead of returning 0
					// There is now an optional type of encoding, NumEncoding.
					// If using new (non-hex) encoding, old values could have already been saved as hex.
					// So fall back to hex if there is a problem with other encoding
					try { return Convert.ToUInt64(s, (int)numEncoding); }
					catch (FormatException) when (numEncoding != NumEncoding.Hex) { return Convert.ToUInt64(s, 16); }
				}
				public static UInt32 ConvertStringToUInt32(string s, NumEncoding numEncoding = NumEncoding.Hex)
				{
					// Need to do the following check because at one point the values were stored in their "native" format, but are now 64-bit
					if (s != null && s.Length == 8) // It's a 32-bit number represented in hex
						return Convert.ToUInt32(s, (int)numEncoding);
					else
					{
						UInt64 temp64 = ConvertStringToUInt64(s, numEncoding);
						if (temp64 >= UInt32.MaxValue) // CR 30858 - This means it was probably saved as a signed int
						{
							Int64 signed = (Int64)temp64;
							if (signed < Int32.MaxValue)
							{
							try { return (UInt32)(Int32)signed; }
							catch (Exception) { ; } // returning MaxValue below
							}
							return UInt32.MaxValue;
						}
						else return Convert.ToUInt32(temp64);
					}
				}
				public static UInt16 ConvertStringToUInt16(string s, NumEncoding numEncoding = NumEncoding.Hex)
				{
					// Need to do the following check because at one point the values were stored in their "native" format, but are now 64-bit
					if (s != null && s.Length == 4) // It's a 16-bit number represented in hex
						return Convert.ToUInt16(s, (int)numEncoding);
					else
					{
						UInt64 temp64 = ConvertStringToUInt64(s, numEncoding);
						return temp64 == UInt64.MaxValue ? // This is to account for some enums now using -1 as undefined
							UInt16.MaxValue :
							Convert.ToUInt16(temp64);
					}
				}

				public static Byte ConvertStringToByte(string s, NumEncoding numEncoding = NumEncoding.Hex)
				{
					// Need to do the following check because at one point the values were stored in their "native" format, but are now 64-bit
					if (s != null && s.Length == 2) // It's a 8-bit number represented in hex
						return Convert.ToByte(s, 16);
					else
					{
						UInt64 temp64 = ConvertStringToUInt64(s, numEncoding);
						return temp64 == UInt64.MaxValue ? // This is to account for some enums now using -1 as undefined
							Byte.MaxValue :
							Convert.ToByte(temp64);
					}
				}

				public static Int64 ConvertStringToInt64(string s, NumEncoding numEncoding = NumEncoding.Hex)
				{
					if (string.IsNullOrWhiteSpace(s)) return 0; // CR 30619 - Check for empty string, as Convert.ToXXXX will throw an exception instead of returning 0
					// There is now an optional type of encoding, NumEncoding.
					// If using new (non-hex) encoding, old values could have already been saved as hex.
					// So fall back to hex if there is a problem with other encoding
					try { return Convert.ToInt64(s, (int)numEncoding); }
					catch (FormatException) when (numEncoding != NumEncoding.Hex) { return Convert.ToInt64(s, 16); }
				}

				public static Int32 ConvertStringToInt32(string s, NumEncoding numEncoding = NumEncoding.Hex)
				{
					// Need to do the following check because at one point the values were stored in their "native" format, but are now 64-bit
					if (s != null && s.Length == 8) // It's a 32-bit number represented in hex
						return Convert.ToInt32(s, 16);
					else
					{
						Int64 temp64 = ConvertStringToInt64(s, numEncoding);
						return temp64 == Int64.MaxValue ? // This is to account for some enums now using -1 as undefined
							Int32.MaxValue :
							Convert.ToInt32(temp64);
					}
				}

				public static Int16 ConvertStringToInt16(string s, NumEncoding numEncoding = NumEncoding.Hex)
				{
					// Need to do the following check because at one point the values were stored in their "native" format, but are now 64-bit
					if (s.Length == 4) // It's a 16-bit number represented in hex
						return Convert.ToInt16(s, 16);
					else
					{
						Int64 temp64 = ConvertStringToInt64(s, numEncoding);
						return temp64 == Int64.MaxValue ? // This is to account for some enums now using -1 as undefined
							Int16.MaxValue :
							Convert.ToInt16(temp64);
					}
				}
				public static SByte ConvertStringToSByte(string s, NumEncoding numEncoding = NumEncoding.Hex)
				{
					// Need to do the following check because at one point the values were stored in their "native" format, but are now 64-bit
					if (s.Length == 2) // It's a 8-bit number represented in hex
						return Convert.ToSByte(s, 16);
					else
					{
						Int64 temp64 = ConvertStringToInt64(s, numEncoding);
						return temp64 == Int64.MaxValue ? // This is to account for some enums now using -1 as undefined
							SByte.MaxValue :
							Convert.ToSByte(temp64);
					}
				}

				public static string ConvertFloatToString<T>(T i)
				{
					return String.Format(cultureInfo, "{0:g}", i);
				}
				public static string ConvertBoolToString(bool i)
				{
					return (i ? "1" : "0");
				}
				public static string ConvertDateTimeToString(DateTime i)
				{
					// CR 27101 - 11.Dec.2018 - Now that client is creating some DateTimes (specifically Task Triggers for this CR) those need to be converted to UTC before sending
					return TimeZoneInfo.ConvertTimeToUtc(i).ToString("yyyy-MM-dd HH:mm:ss.ffff", cultureInfo);
				}

				public static float ConvertStringToFloat(string s)
				{
					return string.IsNullOrWhiteSpace(s) ? 0 : Convert.ToSingle(s, cultureInfo);
				}
				public static double ConvertStringToDouble(string s)
				{
					return string.IsNullOrWhiteSpace(s) ? 0 : Convert.ToDouble(s, cultureInfo);
				}
				public static DateTime ConvertStringToDateTime(string s)
				{
					// CR 26999.Item4 - 16.Nov.2018 - AP Agent is sending over times as UTC.  This should be the standard going forward.
					DateTime convertedDateTime = string.IsNullOrWhiteSpace(s) ? new DateTime() : Convert.ToDateTime(s);
					return TimeZoneInfo.ConvertTimeFromUtc(convertedDateTime, TimeZoneInfo.Local);// DateTime.ParseExact(s, "yyyy-MM-dd HH:mm:ss.fff", cultureInfo);
				}
				public static bool ConvertStringToBool(string s)
				{
					return (s == "0" || s == "" ? false : true);
				}

				/* ConvertStringToRawString()
				 * 
				 * SLB 07.aug.2025 CR.34340; Fix for LicenseViewer's event log entries,
				 * imported from diagnostic data files, having '&#13' and '&#10' 
				 * displayed throught multi-line messages.
				 * -----------------------------------------------------------------*/
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
						// CR.34340; This format (decimal) occurs in serialized
						// eventlog data from licensing's Diagnostic Data files.
						if (s.IndexOf("&#") >= 0)									// CR.34340; added if () {}.
						{
							// replace CR,LF and HT encodings with their respective
							// values.
							s = s.Replace("&#10;", "\x0A");
							s = s.Replace("&#13;", "\x0D");
							s = s.Replace("&#09;", "\x09");
						}

					}
					return s;
				}
				public static string ConvertRawStringToString(string s)
				{
					//System.IO.StringWriter string_writer = new System.IO.StringWriter();
					//System.Xml.XmlTextWriter xml_writer = new System.Xml.XmlTextWriter(string_writer);
					//xml_writer.WriteString(s);
					//return string_writer.ToString();
					return System.Security.SecurityElement.Escape(s); ;
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
						// CR 12228 - JMF - Oct.07.2011 - Matt has managed to import a configuration where the ampersand was not escaped.
						// So now I have added a fix to auto-repair this issue if it is encountered again.
						string new_xml = xml.Replace("&", "&amp;");
						try { xml_reader_doc.LoadXml(new_xml); }
						catch (Exception) { throw; }
					}
					// how to disable returning whitespace nodes?
					// xml_reader.WhitespaceHandling = System.Xml.WhitespaceHandling.None;
					return xml_reader_doc.DocumentElement;
				}
			}
		}
	}
}
