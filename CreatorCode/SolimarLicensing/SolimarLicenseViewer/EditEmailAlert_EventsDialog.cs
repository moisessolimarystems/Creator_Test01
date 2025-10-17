using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace SolimarLicenseViewer
{
    public partial class EditEmailAlert_EventsDialog : Shared.VisualComponents.DialogBaseForm
    {
        public class DialogData
        {
            public DialogData(
                IList<int> _checkedEventIdList
                )
            {
                this.checkedEventIdList = _checkedEventIdList;
            }
            public IList<int> checkedEventIdList { get; private set; }
        }
        public EditEmailAlert_EventsDialog()
        {
            InitializeComponent();
            InitializeInternal();
        }
        private void InitializeInternal()
        {
            
            List<ListViewItem> lviList = new List<ListViewItem>();

            //foreach (var value in Enum.GetValues(typeof(Solimar.Licensing.KeyMessages.EventId)))
            //{
            //    int index = (int)value;
            //    ListViewItem lvi = new ListViewItem();
            //    lvi.Text = string.Format("{0}", (int)value);

            //    lvi.SubItems.Add(Solimar.Licensing.KeyMessages.GetDescription(index));
            //    lvi.Tag = index;
            //    lviList.Add(lvi);
            //}

            foreach (Solimar.Licensing.KeyMessages.EventId value in Enum.GetValues(typeof(Solimar.Licensing.KeyMessages.EventId)))
            {
                if (value != Solimar.Licensing.KeyMessages.EventId.ClientTimeoutProductAndAppInst)
                {
                    int index = (int)value;
                    ListViewItem lvi = new ListViewItem();
                    lvi.Text = Solimar.Licensing.KeyMessages.GetDescription(index);
                    lvi.Tag = index;
                    lviList.Add(lvi);
                }
            }
            this.eventsListView.Items.Clear();
            this.eventsListView.Items.AddRange(lviList.ToArray());

            foreach(ColumnHeader column in this.eventsListView.Columns)
                column.AutoResize(ColumnHeaderAutoResizeStyle.ColumnContent);
        }

        private const string TITLE = "Configure Events";
        private const string READONLY = " (Read Only)";
        private bool bReadOnly = false;
        private DialogData data = null;

        public void SetData(DialogData _data, bool _bReadOnly)
        {
            this.data = _data;
            this.bReadOnly = _bReadOnly;

            this.Text = string.Format("{0}{1}",
                TITLE,
                this.bReadOnly ? READONLY : ""
                );

            foreach (ListViewItem lvi in this.eventsListView.Items)
                lvi.Checked = this.data.checkedEventIdList.Contains((int)lvi.Tag);

            this.eventsCheckAllTSButton.Enabled = !this.bReadOnly;
            this.eventsUncheckAllTSButton.Enabled = !this.bReadOnly;
            this.eventsListView.Enabled = !this.bReadOnly;
        }
        public DialogData GetData()
        {
            List<int> checkedList = new List<int>();

            foreach (ListViewItem lvi in this.eventsListView.CheckedItems)
            {
                checkedList.Add((int)lvi.Tag);
                if ((Solimar.Licensing.KeyMessages.EventId)lvi.Tag == Solimar.Licensing.KeyMessages.EventId.ClientTimeout)
                    checkedList.Add((int)Solimar.Licensing.KeyMessages.EventId.ClientTimeoutProductAndAppInst);
            }

            return data = new DialogData(checkedList);
        }

        private void eventsCheckAllTSButton_Click(object sender, EventArgs e)
        {
            foreach (ListViewItem lvi in this.eventsListView.Items)
                lvi.Checked = true;
        }

        private void eventsUncheckAllTSButton_Click(object sender, EventArgs e)
        {
            foreach (ListViewItem lvi in this.eventsListView.Items)
                lvi.Checked = false;
        }

        private void eventsListView_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.A && e.Control)
            {
                foreach (ListViewItem lvi in this.eventsListView.Items)
                    lvi.Selected = true;
            }
        }

        private void EditEmailAlert_EventsDialog_Load(object sender, EventArgs e)
        {
            this.ActiveControl = this.eventsListView;
        }
    }
}
