using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace Shared.VisualComponents
{
    public partial class EventLogEntryControl : UserControl
    {
        public EventLogEntryControl()
        {
            InitializeComponent();
        }

        public void SetData(Solimar.Licensing.Attribs.Sys_EventLogInfoAttribs.Sys_EventLogEntriesInfoAttribs _eventLogEntry)
        {
            if (_eventLogEntry != null)
            {
                //CR.FIX.14448 - Event log messages are xml encoded
                this.richTextBox1.Text = Solimar.Licensing.Attribs.AttribFormat.ConvertStringToRawString(_eventLogEntry.message.TVal);
                this.evtLogComputer.Text = _eventLogEntry.machineName.TVal;
                this.evtLogEventId.Text = _eventLogEntry.instanceId.TVal.ToString();
                this.evtLogLevel.Text = ((System.Diagnostics.EventLogEntryType)_eventLogEntry.entryType.TVal).ToString();
                this.evtLogLogged.Text = _eventLogEntry.timeWritten.TVal.ToString();
                this.evtLogName.Text = "Application";
                this.evtLogSource.Text = _eventLogEntry.source.TVal;
                this.evtLogTask.Text = _eventLogEntry.categoryNumber.TVal == 0 ? "None" : _eventLogEntry.category.TVal;
                this.evtLogUser.Text = _eventLogEntry.userName.TVal == string.Empty ? "N/A" : _eventLogEntry.userName.TVal;
            }
            else
            {
                this.richTextBox1.Text = string.Empty;
                this.evtLogComputer.Text = string.Empty;
                this.evtLogEventId.Text = string.Empty;
                this.evtLogLevel.Text = string.Empty;
                this.evtLogLogged.Text = string.Empty;
                this.evtLogName.Text = string.Empty;
                this.evtLogSource.Text = string.Empty;
                this.evtLogTask.Text = string.Empty;
                this.evtLogUser.Text = string.Empty;
            }
        }
    }
}
