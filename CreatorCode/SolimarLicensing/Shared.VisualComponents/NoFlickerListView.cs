using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace Shared.VisualComponents
{
	public partial class NoFlickerListView : ListView
	{
        [System.Runtime.InteropServices.StructLayout(System.Runtime.InteropServices.LayoutKind.Sequential)]
        public struct HDITEM
        {
            public Int32 mask;
            public Int32 cxy;
            [System.Runtime.InteropServices.MarshalAs(System.Runtime.InteropServices.UnmanagedType.LPTStr)]
            public String pszText;
            public IntPtr hbm;
            public Int32 cchTextMax;
            public Int32 fmt;
            public Int32 lParam;
            public Int32 iImage;
            public Int32 iOrder;
        };

        #region HDI
        public enum HDI
        {
            HDI_FORMAT = 0x0004,
        }
        #endregion

        #region HDF
        public enum HDF
        {
            HDF_LEFT = 0x0000,
            HDF_STRING = 0x4000,
            HDF_SORTUP = 0x0400,
            HDF_SORTDOWN = 0x0200,
        }
        #endregion

        #region HDM
        public enum HDM
        {
            HDM_FIRST = 0x1200,
            HDM_GETITEM = (HDM_FIRST + 11),
            HDM_SETITEM = (HDM_FIRST + 12),
        }
        #endregion

        #region LVS_EX
        public enum LVS_EX
        {
            LVS_EX_GRIDLINES = 0x00000001,
            LVS_EX_SUBITEMIMAGES = 0x00000002,
            LVS_EX_CHECKBOXES = 0x00000004,
            LVS_EX_TRACKSELECT = 0x00000008,
            LVS_EX_HEADERDRAGDROP = 0x00000010,
            LVS_EX_FULLROWSELECT = 0x00000020,
            LVS_EX_ONECLICKACTIVATE = 0x00000040,
            LVS_EX_TWOCLICKACTIVATE = 0x00000080,
            LVS_EX_FLATSB = 0x00000100,
            LVS_EX_REGIONAL = 0x00000200,
            LVS_EX_INFOTIP = 0x00000400,
            LVS_EX_UNDERLINEHOT = 0x00000800,
            LVS_EX_UNDERLINECOLD = 0x00001000,
            LVS_EX_MULTIWORKAREAS = 0x00002000,
            LVS_EX_LABELTIP = 0x00004000,
            LVS_EX_BORDERSELECT = 0x00008000,
            LVS_EX_DOUBLEBUFFER = 0x00010000,
            LVS_EX_HIDELABELS = 0x00020000,
            LVS_EX_SINGLEROW = 0x00040000,
            LVS_EX_SNAPTOGRID = 0x00080000,
            LVS_EX_SIMPLESELECT = 0x00100000
        }
        #endregion

        #region LVM
        public enum LVM
        {
            LVM_FIRST = 0x1000,
            LVM_SETEXTENDEDLISTVIEWSTYLE = (LVM_FIRST + 54),
            LVM_GETEXTENDEDLISTVIEWSTYLE = (LVM_FIRST + 55),
            LVM_GETHEADER = (LVM_FIRST + 31),
            LVM_SETSELECTEDCOLUMN = (LVM_FIRST + 140),
            LVM_SETCOLUMNWIDTH = (LVM_FIRST + 30),
        }
        #endregion

        [System.Runtime.InteropServices.DllImport("user32.dll", CharSet = System.Runtime.InteropServices.CharSet.Auto)]
        public static extern int SendMessage(IntPtr handle, int messg, int wparam, int lparam);

        [System.Runtime.InteropServices.DllImport("user32.dll", EntryPoint = "SendMessage")]
        private static extern IntPtr SendMessageITEM(IntPtr Handle, Int32 msg, IntPtr wParam, ref HDITEM lParam);


		public NoFlickerListView()
		{
			InitializeComponent();
			DoubleBuffered = true;
		}
        private bool _gridLines = false;
        /// 
        /// Handle GridLines on our own because base.GridLines has to be switched on
        /// and off depending on the amount of items in the ListView.
        /// 
        [DefaultValue(false)]
        public new bool GridLines
        {
            get { return _gridLines; }
            set { _gridLines = value; Invalidate(); }
        }

        private string _noItemsMessage = "There are no items to show in this view";
        /// 
        /// To be able to localize the message it must not be hardcoded
        /// 
        [DefaultValue("There are no items to show in this view")]
        public string NoItemsMessage
        {
            get { return _noItemsMessage; }
            set { _noItemsMessage = value; Invalidate(); }
        }
        public void Reset_NoItemsMessage()
        {
            NoItemsMessage = "There are no items to show in this view";
        }

        private int m_ColumnToSort;
        private SortOrder m_OrderOfSort;
        public void SetSortColumn(int _columnIndex, SortOrder _sortOrder)
        {
            SetSortIcons(_columnIndex, m_ColumnToSort, _sortOrder);
            SetSelectedColumnColor(_columnIndex);
            m_ColumnToSort = _columnIndex;
            m_OrderOfSort = _sortOrder;
        }

        private void SetSortIcons(int _newColumnIndex, int _oldColumnIndex, SortOrder _orderOfSort)
        {
            IntPtr hHeader = (IntPtr)SendMessage(this.Handle, (int)LVM.LVM_GETHEADER, 0, 0);
            IntPtr newColumnPtr = new IntPtr(_newColumnIndex);
            IntPtr prevColumnPtr = new IntPtr(_oldColumnIndex);
            HDITEM hdItem;
            IntPtr rtn;

            // Only update the previous item if it existed and if it was a different one.
            if (_oldColumnIndex != -1 && _oldColumnIndex != _newColumnIndex)
            {
                // Clear icon from the previous column.
                hdItem = new HDITEM();
                hdItem.mask = (int)HDI.HDI_FORMAT;
                rtn = SendMessageITEM(hHeader, (int)HDM.HDM_GETITEM, prevColumnPtr, ref hdItem);
                hdItem.fmt &= (int)~HDF.HDF_SORTDOWN & (int)~HDF.HDF_SORTUP;
                rtn = SendMessageITEM(hHeader, (int)HDM.HDM_SETITEM, prevColumnPtr, ref hdItem);
            }

            // Set icon on the new column.
            hdItem = new HDITEM();
            hdItem.mask = (int)HDI.HDI_FORMAT;
            rtn = SendMessageITEM(hHeader, (int)HDM.HDM_GETITEM, newColumnPtr, ref hdItem);
            if (_orderOfSort == SortOrder.Ascending)
            {
                hdItem.fmt &= (int)~HDF.HDF_SORTDOWN;
                hdItem.fmt |= (int)HDF.HDF_SORTUP;
                hdItem.fmt |= (int)HDF.HDF_LEFT;
            }
            else if (_orderOfSort == SortOrder.Descending)
            {
                hdItem.fmt &= (int)~HDF.HDF_SORTUP;
                hdItem.fmt |= (int)HDF.HDF_SORTDOWN;
                hdItem.fmt |= (int)HDF.HDF_LEFT;
            }
            else if (_orderOfSort == SortOrder.None)
            {
                hdItem.fmt &= (int)~HDF.HDF_SORTUP;
                hdItem.fmt &= (int)~HDF.HDF_SORTDOWN;
            }
            rtn = SendMessageITEM(hHeader, (int)HDM.HDM_SETITEM, newColumnPtr, ref hdItem);
        }
        private void SetSelectedColumnColor(int _newSortColumn)
        {
            SendMessage(this.Handle, (int)LVM.LVM_SETSELECTEDCOLUMN, _newSortColumn, 0);
        }

        //const int WM_ERASEBKGND = 0x14;
        const int WM_PAINT = 0x0F;
        protected override void WndProc(ref Message m)
        {
            base.WndProc(ref m);

            //if (m.Msg == WM_ERASEBKGND) -> Doesn't appear if using DoubleBuffered
            if(m.Msg == WM_PAINT)
            {
                #region Handle drawing of "no items" message
                if (Items.Count == 0 && Columns.Count > 0)
                {
                    if (this.GridLines)
                    {
                        base.GridLines = false;
                    }

                    int w = 0;
                    foreach (ColumnHeader h in this.Columns)
                        w += h.Width;

                    StringFormat sf = new StringFormat();
                    sf.Alignment = StringAlignment.Center;

                    Rectangle rc = new Rectangle(0, (int)(this.Font.Height * 2), w, this.Height);

                    using (Graphics g = this.CreateGraphics())
                    {
                        g.FillRectangle(SystemBrushes.Window, 0, 0, this.Width, this.Height);
                        g.DrawString(NoItemsMessage, this.Font, SystemBrushes.ControlText, rc, sf);
                    }
                }
                else
                {
                    base.GridLines = this.GridLines;
                }
                #endregion
            }
        }
	}
}
