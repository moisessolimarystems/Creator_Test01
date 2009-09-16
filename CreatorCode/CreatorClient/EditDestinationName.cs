using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Client.Creator.CreatorService;
using Client.Creator.ServiceProxy;

namespace Client.Creator
{
    public partial class EditDestinationName : Shared.VisualComponents.DialogBaseForm
    {
        private LicenseServerProperty _licInfo;
        private bool _modified;

        public bool Modified
        {
            get { return _modified; }
            set { _modified = value; }
        }

        //returns selected destid otherwise base destid
        public int SelectedDestinationID
        {
            get
            {
                int id = -1;
                Service<ICreator>.Use((client) =>
                {
                    DestinationNameTable destTable = client.GetDestinationID((int)_licInfo.CustID, SelectedDestinationName);
                    if (destTable != null)
                        id = destTable.DestID;
                });                
                return id;
            }
        }
        //returns selected destname otherwise base destname
        public string SelectedDestinationName
        {
            get 
            { 
                if(destNameListView.SelectedItems.Count > 0)
                    return destNameListView.SelectedItems[0].Text;
                return destNameListView.Items[0].Text;
            }
        }

        public EditDestinationName(LicenseServerProperty licInfo, ComboBox.ObjectCollection destNameCollection)
        {            
            InitializeComponent();
            _modified = false;
            _licInfo = licInfo;
            foreach (string item in destNameCollection)
            {
                if (!item.Equals("<New...>") && !item.Equals("<Edit...>"))
                {
                    ListViewItem lvItem = new ListViewItem(item);
                    lvItem.Name = item;
                    destNameListView.Items.Add(item);
                }
            }
            destNameListView.TopItem.Selected = true;
        }

        //need to disallow removal of any destination names being used.
        private void removeDestNameButton_Click(object sender, EventArgs e)
        {
            DialogResult result = MessageBox.Show(string.Format("Are you sure you want to remove '{0}'", SelectedDestinationName), "Remove Destination Name",
                                      MessageBoxButtons.YesNo, MessageBoxIcon.Question);
            if (result == DialogResult.Yes)
            {
                int licCount = 0;
                Service<ICreator>.Use((client) =>
                {
                    licCount = client.GetLicenseCountByDestName((uint)_licInfo.CustID, (uint)SelectedDestinationID);
                });
                if (licCount == 0)
                {
                    int removeIndex = destNameListView.SelectedIndices[0];
                    DestinationNameTable destTable;
                    Service<ICreator>.Use((client) =>
                    {
                        destTable = client.GetDestinationID((int)_licInfo.CustID, SelectedDestinationName);
                        client.DeleteDestinationName(destTable);
                    });
                    destNameListView.Items.RemoveAt(removeIndex);
                    if (removeIndex < destNameListView.Items.Count)
                        destNameListView.Items[removeIndex].Selected = true;
                    else
                        destNameListView.Items[destNameListView.Items.Count - 1].Selected = true;
                    _modified = true;
                }
                else
                {
                    MessageBox.Show(string.Format("Failed to remove destination name. '{0}' is currently in use.", SelectedDestinationName), "Remove Destination Name",
                                    MessageBoxButtons.OK, MessageBoxIcon.Error);
                }


            }
            //delete from database
        }

        private void renameDestNameButton_Click(object sender, EventArgs e)
        {
            //prompt user to confirm
            if (destNameListView.SelectedItems.Count > 0)
            {
                destNameListView.SelectedItems[0].BeginEdit();
            }
        }

        private void destNameListView_AfterLabelEdit(object sender, LabelEditEventArgs e)
        {

            if ((e.Label == null) || SelectedDestinationName.Equals(e.Label))
                e.CancelEdit = true;
            else
            {
                DialogResult result = MessageBox.Show(string.Format("Are you sure you want to rename '{0}' to '{1}'",SelectedDestinationName, e.Label), "Edit Destination Name",
                                                      MessageBoxButtons.YesNo, MessageBoxIcon.Question);
                if (result == DialogResult.Yes)
                {
                    //modify database
                    DestinationNameTable destTable;
                    Service<ICreator>.Use((client) =>
                    {
                        destTable = client.GetDestinationID((int)_licInfo.CustID, SelectedDestinationName);
                        destTable.DestName = e.Label;
                        client.UpdateDestinationName(destTable);
                        _modified = true;
                    });                     
                }
                else
                    e.CancelEdit = true;
            }
        }

        private void destNameListView_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {   //allow selected and non default destination names to be remove & renamed.
            if (e.IsSelected && e.ItemIndex > 0)
            {
                removeDestNameButton.Enabled = true;
                renameDestNameButton.Enabled = true;
            }
            else
            {
                removeDestNameButton.Enabled = false;
                renameDestNameButton.Enabled = false;
            }
        }
    }
}
