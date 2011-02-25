using System;
using System.Collections.Generic;
using System.Text;

namespace Shared.VisualComponents
{
    public class EventLogMgr : IEventLog
    {
        public EventLogMgr()
        {
            m_eventLog = new System.Diagnostics.EventLog();
            m_eventLog.Log = "Application";
            m_eventLog.MachineName = ".";
        }
        protected System.Diagnostics.EventLog m_eventLog = null;

        public string MachineName
        {
            get { return m_eventLog.MachineName; }
            set { m_eventLog.MachineName = value; }
        }

        #region IEventLog
        public System.Collections.Generic.IList<Solimar.Licensing.Attribs.Sys_EventLogInfoAttribs.Sys_EventLogEntriesInfoAttribs> GetAllEntries()
        {
            //System.Diagnostics.Trace.WriteLine(string.Format("GetAllEntries() - Start - MachineName: {0}", m_eventLog.MachineName));
        
            System.Collections.Generic.IList<Solimar.Licensing.Attribs.Sys_EventLogInfoAttribs.Sys_EventLogEntriesInfoAttribs> eventLogList = new System.Collections.Generic.List<Solimar.Licensing.Attribs.Sys_EventLogInfoAttribs.Sys_EventLogEntriesInfoAttribs>();
            const int MAXSIZE = 100;
            int currentCount = 0;
            for (int idx = m_eventLog.Entries.Count - 1;
                    idx >= 0 && currentCount < MAXSIZE;
                    currentCount++, idx--)
            {
                System.Diagnostics.EventLogEntry eventLogEntry = m_eventLog.Entries[idx];
                Solimar.Licensing.Attribs.Sys_EventLogInfoAttribs.Sys_EventLogEntriesInfoAttribs sysEventLogInfo = new Solimar.Licensing.Attribs.Sys_EventLogInfoAttribs.Sys_EventLogEntriesInfoAttribs();
                sysEventLogInfo.category.TVal = eventLogEntry.Category;
                sysEventLogInfo.categoryNumber.TVal = (uint)eventLogEntry.CategoryNumber;
                sysEventLogInfo.data.TVal = eventLogEntry.Data;
                sysEventLogInfo.entryType.TVal = (uint)eventLogEntry.EntryType;
                sysEventLogInfo.eventID.TVal = (uint)eventLogEntry.EventID;
                sysEventLogInfo.index.TVal = (uint)eventLogEntry.Index;
                sysEventLogInfo.instanceId.TVal = (uint)eventLogEntry.InstanceId;
                sysEventLogInfo.machineName.TVal = eventLogEntry.MachineName;

                //CR.FIX.14448 - Event log messages are xml encoded
                sysEventLogInfo.message.TVal = Solimar.Licensing.Attribs.AttribFormat.ConvertRawStringToString(eventLogEntry.Message);
                //sysEventLogInfo.replacementStrings.TVal = eventLogEntry.ReplacementStrings;
                sysEventLogInfo.source.TVal = eventLogEntry.Source;
                sysEventLogInfo.timeGenerated.TVal = eventLogEntry.TimeGenerated;
                sysEventLogInfo.timeWritten.TVal = eventLogEntry.TimeWritten;
                sysEventLogInfo.userName.TVal = eventLogEntry.UserName;
              
                eventLogList.Add(sysEventLogInfo);
            }
            
            //System.Diagnostics.Trace.WriteLine(string.Format("GetAllEntries() - End - MachineName: {0}", m_eventLog.MachineName));
            return eventLogList;
            
        }
        public string GetMachineName() { return MachineName; }
        #endregion
    }
    public class RemoteEventLogMgr : EventLogMgr
    {
        public RemoteEventLogMgr(string _server) : base()
        {
            m_eventLog.MachineName = _server;
        }
    }

    /// <summary>Expose an interface to gain access to event log information</summary>
    public interface IEventLog
    {
        /// <summary>Returns the Machine Name for the EventLog.</summary>
        string GetMachineName();

        /// <summary>Returns the list of eventlog entries.</summary>
        System.Collections.Generic.IList<Solimar.Licensing.Attribs.Sys_EventLogInfoAttribs.Sys_EventLogEntriesInfoAttribs> GetAllEntries();
    }
}
