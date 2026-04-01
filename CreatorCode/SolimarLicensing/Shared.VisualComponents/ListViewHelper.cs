using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace Shared.VisualComponents
{
    public class ListViewHelper
    {
        // Display either the full column header or entire data, which ever is bigger
        static public void ResizeListViewHeadersToMaxOfDataAndHeader(ListView _listView)
        {
            if (_listView != null)
            {
                _listView.AutoResizeColumns(ColumnHeaderAutoResizeStyle.HeaderSize);
                List<int> headerMinSize = new List<int>();
                for (int colIdx = 0; colIdx < _listView.Columns.Count; colIdx++)
                    headerMinSize.Add(_listView.Columns[colIdx].Width);
                _listView.AutoResizeColumns(ColumnHeaderAutoResizeStyle.ColumnContent);
                for (int colIdx = 0; colIdx < _listView.Columns.Count; colIdx++)
                {
                    if (headerMinSize[colIdx] > _listView.Columns[colIdx].Width)
                        _listView.Columns[colIdx].Width = headerMinSize[colIdx];
                }
            }
        }

        static public void ListView_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Handled == false && sender is ListView)
            {
                ListView lView = sender as ListView;
                if (e.KeyCode == Keys.A && e.Control)
                {
                    try
                    {
                        lView.BeginUpdate();
                        foreach (ListViewItem lvi in lView.Items)
                            lvi.Selected = true;
                        e.Handled = true;
                    }
                    catch (Exception)
                    {
                    }
                    finally
                    {
                        lView.EndUpdate();
                    }
                }
            }
        }
    }
}
