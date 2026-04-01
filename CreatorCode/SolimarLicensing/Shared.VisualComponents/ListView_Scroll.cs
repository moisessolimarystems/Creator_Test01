using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
//using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Shared.VisualComponents
{
    public partial class ListView_Scroll : NoFlickerListView
    {
        public ListView_Scroll()
        {
            InitializeComponent();
        }

        public delegate void ScrollHandler(object sender, ListViewScrollEventArgs e);

        public event ScrollHandler Scroll;

        public void OnScroll(ListViewScrollEventArgs e)
        {
            if (Scroll != null)
                Scroll(this, e);
        }

        [System.Runtime.InteropServices.DllImport("user32.dll")]
        private static extern IntPtr SendMessage(IntPtr hWnd, uint Msg, IntPtr wParam, IntPtr lParam);

        public void ScrollTo(ListViewScrollEventArgs e)
        {
            if (m_handlingWndProc == false)
            {
//System.Diagnostics.Trace.WriteLine(string.Format("HANDLED - this: 0x{2:x}, m.Msg: 0x{0:x}, mWParam: 0x{1:x}", e.Msg, e.WParam, this.Handle));
                SendMessage(this.Handle, e.Msg, e.WParam, IntPtr.Zero);
            }
        }

        private const int WM_HSCROLL = 0x114;
        private const int WM_VSCROLL = 0x115;
        private const int WM_KEYDOWN = 0x100;
        private const int WM_KEYUP = 0x101; 
        private const int WM_MOUSEWHEEL = 0x020a;


        private bool m_handlingWndProc = false;
        protected override void WndProc(ref Message m)
        {
            base.WndProc(ref m);
            if (m.Msg == WM_VSCROLL || m.Msg == WM_MOUSEWHEEL || m.Msg == WM_KEYUP || m.Msg == WM_KEYDOWN || m.Msg == WM_HSCROLL)
            {
                if (m_handlingWndProc == false)
                {
                    m_handlingWndProc = true;
                    //System.Diagnostics.Trace.WriteLine("m.Msg: " + m.Msg + ", m.LParam: " + m.LParam + ", mWParam: " + m.WParam);
                    ListViewScrollEventArgs e = new ListViewScrollEventArgs();
                    e.Msg = (uint)m.Msg;
                    e.WParam = m.WParam;
                    e.Orientation = (m.Msg == WM_HSCROLL) ? ScrollOrientation.HorizontalScroll : ScrollOrientation.VerticalScroll;
                    OnScroll(e);
                    m_handlingWndProc = false;
                }
            }
        }
    }



    public class ListViewScrollEventArgs : EventArgs
    {
        private ScrollOrientation orientation;
        private IntPtr wParam;
        private uint msg;
        public IntPtr WParam
        {
            get { return wParam; }
            set { wParam = value; }
        }

        public ScrollOrientation Orientation
        {
            get { return orientation; }
            set { orientation = value; }
        }
        public uint Msg
        {
            get { return msg; }
            set { msg = value; }
        }
    }
}
