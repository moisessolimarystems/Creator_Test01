using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Drawing;
using System.Windows.Forms;
using System.Collections;

namespace Client.Creator
{
    public class ListViewMgr
    {
        #region ListView Win32 Defines
        [StructLayout(LayoutKind.Sequential)]
        public struct HDITEM
        {
            public Int32 mask;
            public Int32 cxy;
            [MarshalAs(UnmanagedType.LPTStr)]
            public String pszText;
            public IntPtr hbm;
            public Int32 cchTextMax;
            public Int32 fmt;
            public Int32 lParam;
            public Int32 iImage;
            public Int32 iOrder;
        };

        // Parameters for ListView-Headers
        public const Int32 HDI_FORMAT = 0x0004;
        public const Int32 HDF_LEFT = 0x0000;
        public const Int32 HDF_STRING = 0x4000;
        public const Int32 HDF_SORTUP = 0x0400;
        public const Int32 HDF_SORTDOWN = 0x0200;
        public const Int32 HDM_GETITEM = 0x1200 + 11;  // HDM_FIRST + 11
        public const Int32 HDM_SETITEM = 0x1200 + 12;  // HDM_FIRST + 12
        public const Int32 LVM_GETHEADER = 0x1000 + 31;  // LVM_FIRST + 31
        public const Int32 LVM_FIRST = 0x1000;
        public const Int32 LVM_SETSELECTEDCOLUMN = (LVM_FIRST + 140);
        public const Int32 LVM_SETCOLUMNWIDTH = LVM_FIRST + 30;

        [DllImport("user32.dll")]
        private static extern IntPtr SendMessage(IntPtr hWnd, uint Msg, IntPtr wParam, IntPtr lParam);

        [DllImport("user32.dll", EntryPoint = "SendMessage")]
        private static extern IntPtr SendMessageITEM(IntPtr Handle, Int32 msg, IntPtr wParam, ref HDITEM lParam);
        #endregion

        #region Private Members

        private ListViewColumnSorter _lvwColumnSorter;

        #endregion

        #region Properties
        #endregion

        #region ListViewColumn Methods
        public ListViewMgr()
        {
            _lvwColumnSorter = new ListViewColumnSorter();
        }

        public void SetListViewColumnSorter(ListView lv)
        {
            lv.ListViewItemSorter = _lvwColumnSorter;
        }

        /// <summary>
        /// Initializes the ListViewColumnSorter with options based upon currently selected column. 
        /// <param name="columnIndex"> index of the column to sort </param>
        /// </summary>
        public void SetSortIndexColumn(IntPtr handle, int columnIndex)
        {
            // Determine if clicked column is already the column that is being sorted.
            if (columnIndex == _lvwColumnSorter.SortColumn)
            {
                // Reverse the current sort direction for this column.
                if (_lvwColumnSorter.Order == SortOrder.Ascending)
                {
                    _lvwColumnSorter.Order = SortOrder.Descending;
                }
                else
                {
                    _lvwColumnSorter.Order = SortOrder.Ascending;
                }
            }
            else
            {
                SetSortIcons(handle, _lvwColumnSorter.SortColumn, columnIndex);
                // Set the column number that is to be sorted; default to ascending.
                _lvwColumnSorter.SortColumn = columnIndex;
                _lvwColumnSorter.Order = SortOrder.Ascending;
            }
            SetSortIcons(handle, _lvwColumnSorter.SortColumn, columnIndex);
            SetSelectedColumnColor(handle, columnIndex);
        }

        //sizes depending on what the current comparer's order
        public void SetSortIcons(IntPtr handle, int previouslySortedColumn, int newSortColumn)
        {
            IntPtr hHeader = SendMessage(handle, LVM_GETHEADER, IntPtr.Zero, IntPtr.Zero);
            IntPtr newColumn = new IntPtr(newSortColumn);
            IntPtr prevColumn = new IntPtr(previouslySortedColumn);
            HDITEM hdItem;
            IntPtr rtn;

            // Only update the previous item if it existed and if it was a different one.
            if (previouslySortedColumn != -1 && previouslySortedColumn != newSortColumn)
            {
                // Clear icon from the previous column.
                hdItem = new HDITEM();
                hdItem.mask = HDI_FORMAT;
                rtn = SendMessageITEM(hHeader, HDM_GETITEM, prevColumn, ref hdItem);
                hdItem.fmt &= ~HDF_SORTDOWN & ~HDF_SORTUP;
                rtn = SendMessageITEM(hHeader, HDM_SETITEM, prevColumn, ref hdItem);
            }

            // Set icon on the new column.
            hdItem = new HDITEM();
            hdItem.mask = HDI_FORMAT;
            rtn = SendMessageITEM(hHeader, HDM_GETITEM, newColumn, ref hdItem);
            if (_lvwColumnSorter.Order == SortOrder.Ascending || _lvwColumnSorter.Order == SortOrder.None)
            {
                hdItem.fmt &= ~HDF_SORTDOWN;
                hdItem.fmt |= HDF_SORTUP;
                hdItem.fmt |= HDF_LEFT;
            }
            else
            {
                hdItem.fmt &= ~HDF_SORTUP;
                hdItem.fmt |= HDF_SORTDOWN;
                hdItem.fmt |= HDF_LEFT;
            }
            rtn = SendMessageITEM(hHeader, HDM_SETITEM, newColumn, ref hdItem);
        }

        public void SetSelectedColumnColor(IntPtr handle, int newSortColumn)
        {
            IntPtr newColumn = new IntPtr(newSortColumn);
            SendMessage(handle, LVM_SETSELECTEDCOLUMN, newColumn, IntPtr.Zero);
        }

        /// <summary>
        /// AutoResize each ListView column based upon the header size. 
        /// Need to call after columns are added for proper resizing.
        /// </summary>
        /// 
        public void AutoResizeColumns(ListView lv)
        {
            Size currentTextSize = new Size();
            Size largestTextSize = new Size();
            foreach (ColumnHeader column in lv.Columns)
            {
                largestTextSize.Width = column.Width;
                foreach (ListViewItem lvItem in lv.Items)
                {
                    currentTextSize = TextRenderer.MeasureText(lvItem.SubItems[column.Index].Text, lv.Font);
                    if (currentTextSize.Width > largestTextSize.Width)
                        largestTextSize.Width = currentTextSize.Width;
                }
                if (largestTextSize.Width > column.Width)
                    column.AutoResize(ColumnHeaderAutoResizeStyle.ColumnContent);
                else
                    column.AutoResize(ColumnHeaderAutoResizeStyle.HeaderSize);

                if (lv.Columns.IndexOf(column) != lv.Columns.Count - 1)
                {
                    //Pad 3 spaces to account for sort up/down arrow
                    column.Width += TextRenderer.MeasureText(" ", lv.Font).Width * 3;
                }
            }
        }

        public void ResetListViewColumnSorter(ListView lv)
        {
            if (lv.Items.Count > 0)
            {
                _lvwColumnSorter.ResetProperties();
                SetSortIndexColumn(lv.Handle, 0);
            }
        }
        #endregion

    }

    class ListViewColumnSorter : IComparer
    {
        #region Private Variables
        /// <summary>
        /// Specifies the column to be sorted
        /// </summary>
        private int ColumnToSort;
        /// <summary>
        /// Specifies the order in which to sort (i.e. 'Ascending').
        /// </summary>
        private SortOrder OrderOfSort;
        /// <summary>
        /// Case insensitive comparer object
        /// </summary>
        private CaseInsensitiveComparer ObjectCompare;
        /// <summary>
        /// Specifies whether or not to group sort
        /// </summary>
        private bool GroupToSort;
        /// <summary>
        /// Declare a Hashtable array in which to store the groups.
        /// </summary>
        private Hashtable[] groupTables;


        #endregion

        #region Constructor
        /// <summary>
        /// Class constructor.  Initializes various elements
        /// </summary>
        public ListViewColumnSorter()
        {
            // Initialize the column to '0'
            ColumnToSort = 0;

            // Initialize the sort order to 'none'
            OrderOfSort = SortOrder.None;

            // Initialize the CaseInsensitiveComparer object
            ObjectCompare = new CaseInsensitiveComparer();
        }
        #endregion

        #region Comparison Methods
        /// <summary>
        /// This method is inherited from the IComparer interface.  It compares the two objects passed using a case insensitive comparison.
        /// </summary>
        /// <param name="x">First object to be compared</param>
        /// <param name="y">Second object to be compared</param>
        /// <returns>The result of the comparison. "0" if equal, negative if 'x' is less than 'y' and positive if 'x' is greater than 'y'</returns>
        public int Compare(object x, object y)
        {
            int compareResult;
            ListViewItem listviewX, listviewY;
          
            // Cast the objects to be compared to ListViewItem objects
            //if (GroupToSort)
            //{
            //    compareResult = String.Compare(((ListViewGroup)x).Header, ((ListViewGroup)y).Header);               
            //}
            //else
            //{
                listviewX = (ListViewItem)x;
                listviewY = (ListViewItem)y;
                // Compare the two items           
                compareResult = ObjectCompare.Compare(listviewX.SubItems[ColumnToSort].Text, listviewY.SubItems[ColumnToSort].Text);
            //}
            // Calculate correct return value based on object comparison
            if (OrderOfSort == SortOrder.Ascending)
            {
                // Ascending sort is selected, return normal result of compare operation
                return compareResult;
            }
            else if (OrderOfSort == SortOrder.Descending)
            {
                // Descending sort is selected, return negative result of compare operation
                return (-compareResult);
            }
            else
            {
                // Return '0' to indicate they are equal
                return 0;
            }
        }



        public void ResetProperties()
        {
            SortColumn = 0;
            Order = SortOrder.None;
        }

        #endregion

        #region Properties
        /// <summary>
        /// Gets or sets the number of the column to which to apply the sorting operation (Defaults to '0').
        /// </summary>
        public int SortColumn
        {
            get { return ColumnToSort; }
            set { ColumnToSort = value; }
        }

        /// <summary>
        /// Gets or sets the order of sorting to apply (for example, 'Ascending' or 'Descending').
        /// </summary>
        public SortOrder Order
        {
            get { return OrderOfSort; }
            set { OrderOfSort = value; }
        }

        public bool GroupSort
        {
            get { return GroupToSort; }
            set { GroupToSort = value; }
        }
        #endregion
    }
}
