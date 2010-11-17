using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Solimar.Licensing.Attribs;

namespace SolimarLicenseDiagnosticDataViewer
{
    public partial class Sys_EventLogInfoAttribs_DisplayForm : Base_DisplayForm<Sys_EventLogInfoAttribs>
    {
        public Sys_EventLogInfoAttribs_DisplayForm()
        {
            InitializeComponent();
            Internal_Initialize();
        }

        private void Internal_Initialize()
        {
            tsErrorButton.Tag = new Solimar.Licensing.Common.Collections.Tuple<string, System.Diagnostics.EventLogEntryType>("Errors", System.Diagnostics.EventLogEntryType.Error);
            tsInfoButton.Tag = new Solimar.Licensing.Common.Collections.Tuple<string, System.Diagnostics.EventLogEntryType>("Messages", System.Diagnostics.EventLogEntryType.Information);
            tsWarningButton.Tag = new Solimar.Licensing.Common.Collections.Tuple<string, System.Diagnostics.EventLogEntryType>("Warnings", System.Diagnostics.EventLogEntryType.Warning);
            tsSourceComboBox.Items.Add(new EventLog_ComboSourceItem());
            tsSourceComboBox.SelectedIndex = 0;
            msgListView.SetSortColumn(1, SortOrder.Descending);
        }

        private Sys_EventLogInfoAttribs m_data = null;
        public override void SetData(Sys_EventLogInfoAttribs _data)
        {
            m_data = _data;
            if (m_data != null)
            {

                RefreshUI();
            }
        }

        private IList<Solimar.Licensing.Attribs.Sys_EventLogInfoAttribs.Sys_EventLogEntriesInfoAttribs> GetAllEntries()
        {
            System.Collections.Generic.IList<Solimar.Licensing.Attribs.Sys_EventLogInfoAttribs.Sys_EventLogEntriesInfoAttribs> eventLogList = new System.Collections.Generic.List<Solimar.Licensing.Attribs.Sys_EventLogInfoAttribs.Sys_EventLogEntriesInfoAttribs>();
            foreach (Solimar.Licensing.Attribs.Sys_EventLogInfoAttribs.Sys_EventLogEntriesInfoAttribs eventEntryInfo in m_data.entryList.TVal)
                eventLogList.Add(eventEntryInfo);
            return eventLogList;
        }

        private void RefreshUI(object sender, EventArgs e)
        {
            RefreshUI();
        }
        private void RefreshUI()
        {
            try
            {
                UseWaitCursor = true;

                //Track Previously selected item
                int? selEventIdx = null;
                //if (this.TheListView.SelectedItems.Count > 0 && this.TheListView.SelectedItems[0].Tag is System.Diagnostics.EventLogEntry)
                //    selEventIdx = (this.TheListView.SelectedItems[0].Tag as System.Diagnostics.EventLogEntry).Index;
                if (this.msgListView.SelectedItems.Count > 0 && this.msgListView.SelectedItems[0].Tag is Solimar.Licensing.Attribs.Sys_EventLogInfoAttribs.Sys_EventLogEntriesInfoAttribs)
                    selEventIdx = (int)(this.msgListView.SelectedItems[0].Tag as Solimar.Licensing.Attribs.Sys_EventLogInfoAttribs.Sys_EventLogEntriesInfoAttribs).index.TVal;
                ListViewItem newSelLvi = null;

                this.msgListView.BeginUpdate();
                this.msgListView.Items.Clear();
                System.Collections.Generic.List<ListViewItem> lviList = new List<ListViewItem>();
                SortedList<string, Solimar.Licensing.Common.Collections.Tuple<ToolStripItem, int, string>> strHash = new SortedList<string, Solimar.Licensing.Common.Collections.Tuple<ToolStripItem, int, string>>();
                ToolStripComboBox toolStripComboBox = null;

                EventLog_ComboSourceItem filterItem = null;
                string filterSource = string.Empty;
                #region foreach (ToolStripItem tsItem in this.msgToolStrip.Items)
                foreach (ToolStripItem tsItem in this.msgToolStrip.Items)
                {
                    if (tsItem is ToolStripButton)
                    {
                        tsItem.Text = ((Solimar.Licensing.Common.Collections.Tuple<string, System.Diagnostics.EventLogEntryType>)tsItem.Tag).Item1;
                        if ((tsItem as ToolStripButton).Checked == true)
                            strHash.Add(((Solimar.Licensing.Common.Collections.Tuple<string, System.Diagnostics.EventLogEntryType>)tsItem.Tag).Item2.ToString(), new Solimar.Licensing.Common.Collections.Tuple<ToolStripItem, int, string>(tsItem, 0, ((Solimar.Licensing.Common.Collections.Tuple<string, System.Diagnostics.EventLogEntryType>)tsItem.Tag).Item1));
                    }
                    else if (tsItem is ToolStripComboBox)
                    {
                        toolStripComboBox = tsItem as ToolStripComboBox;
                        filterItem = toolStripComboBox.SelectedItem as EventLog_ComboSourceItem;
                        filterSource = filterItem != null ? filterItem.source : string.Empty;
                        toolStripComboBox.Items.Clear();
                    }
                }
                #endregion

                System.Collections.Generic.SortedList<string, int> sourceList = new SortedList<string, int>();
                sourceList.Add(string.Empty, 0);

                #region foreach (System.Diagnostics.EventLogEntry eventLogEntry in GetAllEntries())
                IList<Solimar.Licensing.Attribs.Sys_EventLogInfoAttribs.Sys_EventLogEntriesInfoAttribs> eventLogEntryList = GetAllEntries();
//                
                if (eventLogEntryList != null)
                {
                    //foreach (Solimar.Licensing.Attribs.Sys_EventLogInfoAttribs.Sys_EventLogEntriesInfoAttribs eventLogEntry in eventLogEntryList)
                    foreach (Solimar.Licensing.Attribs.Sys_EventLogInfoAttribs.Sys_EventLogEntriesInfoAttribs eventLogEntry in eventLogEntryList)
                    {
                        //string key = eventLogEntry.entryType.ToString();
                        string key = ((System.Diagnostics.EventLogEntryType)eventLogEntry.entryType.TVal).ToString();
                        bool pastSourceFilter = true;
                        if (filterSource != string.Empty)
                            pastSourceFilter = (string.Compare(filterSource, eventLogEntry.source.TVal, true) == 0);

                        if (!sourceList.ContainsKey(eventLogEntry.source.TVal))
                            sourceList.Add(eventLogEntry.source.TVal, 0);
                        if (strHash.ContainsKey(key) == true)
                            sourceList[eventLogEntry.source.TVal]++;

                        if (strHash.ContainsKey(key) == true && pastSourceFilter == true)
                        {
                            ListViewItem lvi = new ListViewItem();
                            //lvi.Text = eventLogEntry.Index.ToString();
                            //lvi.Text = eventLogEntry.entryType.ToString();
                            lvi.Text = key;
                            lvi.Tag = eventLogEntry;
                            lvi.SubItems.Add(eventLogEntry.timeGenerated.TVal.ToString());
                            lvi.SubItems.Add(eventLogEntry.source.TVal);
                            lvi.SubItems.Add(eventLogEntry.instanceId.TVal.ToString());
                            //lvi.SubItems.Add(eventLogEntry.Category);
                            lvi.SubItems.Add(eventLogEntry.categoryNumber.TVal == 0 ? "None" : eventLogEntry.category.TVal);

                            lviList.Add(lvi);
                            //if(!sourceList.ContainsKey(eventLogEntry.Source))
                            //    sourceList.Add(eventLogEntry.Source, 0);
                            //sourceList[eventLogEntry.Source]++;

                            if (newSelLvi == null && selEventIdx.HasValue && selEventIdx == eventLogEntry.index.TVal)
                                newSelLvi = lvi;

                            //if (++idx > 100)    //For testing use top 500
                            //    break;
                            strHash[key].Item2++;
                        }
                    }
                }
                #endregion
                foreach (KeyValuePair<string, Solimar.Licensing.Common.Collections.Tuple<ToolStripItem, int, string>> kvPair in strHash)
                {
                    if (kvPair.Value.Item1 is ToolStripButton)
                        (kvPair.Value.Item1 as ToolStripButton).Text = string.Format("{0} {1}", kvPair.Value.Item2, kvPair.Value.Item3);
                }
                if (toolStripComboBox != null)
                {
                    EventLog_ComboSourceItem selectedItem = null;
                    List<EventLog_ComboSourceItem> tmpList = new List<EventLog_ComboSourceItem>();
                    foreach (KeyValuePair<string, int> kvPair in sourceList)
                    {
                        EventLog_ComboSourceItem entry = new EventLog_ComboSourceItem() { source = kvPair.Key, itemCount = kvPair.Value };
                        tmpList.Add(entry);
                        if (selectedItem == null && filterItem.CompareTo(entry) == 0)
                            selectedItem = entry;
                    }
                    toolStripComboBox.Items.AddRange(tmpList.ToArray());

                    toolStripComboBox.SelectedIndexChanged -= new EventHandler(RefreshUI);
                    toolStripComboBox.SelectedItem = selectedItem;
                    toolStripComboBox.SelectedIndexChanged += new EventHandler(RefreshUI);
                }

                this.msgListView.Items.AddRange(lviList.ToArray());
                tssItemLabel.Text = string.Format("{0} Items", lviList.Count);
                if (newSelLvi != null)
                {
                    newSelLvi.Selected = true;
                    newSelLvi.EnsureVisible();
                }
                else
                    LoadEventLogEntryData(null);

            }
            catch (Exception ex)
            {
                msgListView.NoItemsMessage = ex.Message;
            }
            finally
            {
                this.msgListView.EndUpdate();
                UseWaitCursor = false;
            }
        }
        private void msgListView_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (this.msgListView.SelectedItems.Count > 0)
                LoadEventLogEntryData(this.msgListView.SelectedItems[0].Tag as Solimar.Licensing.Attribs.Sys_EventLogInfoAttribs.Sys_EventLogEntriesInfoAttribs);
            else
                LoadEventLogEntryData(null);
        }
        public void LoadEventLogEntryData(Solimar.Licensing.Attribs.Sys_EventLogInfoAttribs.Sys_EventLogEntriesInfoAttribs _eventLogEntry)
        {
            this.eventLogEntryControl1.SetData(_eventLogEntry);
        }


        private class EventLog_ComboSourceItem : System.IComparable
        {
            public string source = string.Empty;
            public int itemCount = 0;

            public int CompareTo(Object _obj) { return CompareTo(_obj as EventLog_ComboSourceItem); }
            public int CompareTo(EventLog_ComboSourceItem _srcItem) { return string.Compare(this.source, _srcItem.source, true); }
            public override string ToString()
            {
                return (this.source == string.Empty)
                    ? " <All>"
                    : string.Format("{0} ({1})", this.source, this.itemCount);
            }
        }

        
    }
}
