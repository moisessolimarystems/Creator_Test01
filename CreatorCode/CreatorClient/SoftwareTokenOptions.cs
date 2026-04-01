using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Client.Creator.CreatorService;

namespace Client.Creator
{
    public partial class SoftwareTokenOptions : Shared.VisualComponents.DialogBaseForm
    {
        public SoftwareTokenOptions()
        {
            InitializeComponent();
            IList<SoftwareTokenTable> swTokens = null;
            //load property grid with product items
            Client.Creator.ServiceProxy.Service<ICreator>.Use((client) =>
            {
                swTokens = client.GetAllSoftwareTokens();
            });
            foreach (SoftwareTokenTable swt in swTokens)
            {
                ListViewItem lvItem = new ListViewItem();
                lvItem.Name = lvItem.Text = swt.TokenType;
                lvItem.Checked = swt.Status > 0;
                SoftwareTokenListView.Items.Add(lvItem);
            }
        }

        private void SoftwareTokenListView_ItemChecked(object sender, ItemCheckedEventArgs e)
        {
            ServiceProxy.Service<ICreator>.Use((client) =>
            {
                SoftwareTokenTable swt = client.GetSoftwareToken(e.Item.Text);
                swt.Status = (byte)((e.Item.Checked) ? 1 : 0);
                client.UpdateSoftwareTokenTable(swt);
            });
        }
    }
}
