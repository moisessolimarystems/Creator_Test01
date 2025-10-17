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
            if (_listView != null && Application.RenderWithVisualStyles)
            {
                //Will resize to the larger of headers or content
                _listView.AutoResizeColumns(ColumnHeaderAutoResizeStyle.HeaderSize);
            }
            else if (_listView != null)
            {
                //Only do this if the OS has the directional carrot after the column words
                //Such as XP or Server OS's.  Should be able to look at visual style render.
                int padWidth = TextRenderer.MeasureText(" ", _listView.Font).Width * 4;
                //System.Diagnostics.Trace.WriteLine(string.Format("PadWith: {0}", padWidth));
                List<int> colContentSizeList = new List<int>();
                _listView.AutoResizeColumns(ColumnHeaderAutoResizeStyle.ColumnContent);
                foreach (ColumnHeader cHeader in _listView.Columns)
                    colContentSizeList.Add(cHeader.Width);

                _listView.AutoResizeColumns(ColumnHeaderAutoResizeStyle.HeaderSize);
                for (int idx = 0; idx < _listView.Columns.Count; idx++)
                    _listView.Columns[idx].Width = Math.Max(colContentSizeList[idx], _listView.Columns[idx].Width + padWidth);
            }
            return;
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

        static public string GetCopyTextForListView(ListView _listView)
        {
           StringBuilder copyStrBuilder = new StringBuilder();
           foreach (ListViewItem lvi in _listView.SelectedItems)
           {
              StringBuilder lineStrBuilder = new StringBuilder();
              foreach (ListViewItem.ListViewSubItem subItem in lvi.SubItems)
              {
                 if (lineStrBuilder.Length != 0)
                    lineStrBuilder.Append(" ");
                 lineStrBuilder.AppendFormat("\"{0}\"", subItem.Text);
              }
              if (copyStrBuilder.Length != 0)
                 copyStrBuilder.Append("\r\n");
              copyStrBuilder.Append(lineStrBuilder.ToString());
           }
           return copyStrBuilder.ToString();
        }
    }
}
