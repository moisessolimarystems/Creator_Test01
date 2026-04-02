using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Reflection;

namespace Shared.VisualComponents
{
    public class ControlHelper
    {
        static private BindingFlags AllBindings
        {
            get { return BindingFlags.IgnoreCase | BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance | BindingFlags.Static; }
        }

        /// <summary>Removes all the events of _eventName tied to a given control.</summary>
        /// <example>RemoveEvents(button1, "Click");</example>
        /// <param name="_control"></param>
        /// <param name="_eventName"></param>
        static public void RemoveEvents(Control _control, string _eventName)
        {
            //System.Diagnostics.Trace.WriteLine(string.Format("RemoveEvents() - Type: {0}", _control.GetType()));
            Type tp = _control.GetType();

            //System.Diagnostics.Trace.WriteLine(string.Format("RemoveEvents() - Get Event - Type: {0}, Event: {1}", tp.ToString(), _eventName));
            EventInfo tEInfo = tp.GetEvent(_eventName, AllBindings);
            if (tEInfo != null)
            {
                //System.Diagnostics.Trace.WriteLine(string.Format("EventInfo: {0}, {1}", tEInfo.Name, tEInfo.EventHandlerType.ToString()));
                Type dt = tEInfo.DeclaringType;

                //Comment in to see all the Fields, use if event is not being removed correcty, then name could be wrong
                //foreach (FieldInfo tFInfo in dt.GetFields(AllBindings))
                //{
                //    System.Diagnostics.Trace.WriteLine(string.Format("{0} - FieldInfo: {1}", dt.Name, tFInfo.Name));
                //}

                FieldInfo fi = dt.GetField(string.Format("Event{0}", tEInfo.Name), AllBindings);
                if (fi == null) //Some Fields are Event or Event_
                    fi = dt.GetField(string.Format("Event_{0}", tEInfo.Name), AllBindings);

                if (fi != null)
                {
                    object obj = fi.GetValue(_control);
                    if (obj != null)
                    {
                        PropertyInfo pi = tp.GetProperty("Events", AllBindings);
                        System.ComponentModel.EventHandlerList list = (System.ComponentModel.EventHandlerList)pi.GetValue(_control, null);
                        list.RemoveHandler(obj, list[obj]);
                    }
                }
            }
        }
    }
}
