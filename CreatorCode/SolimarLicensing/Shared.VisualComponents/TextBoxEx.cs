using System;
using System.Collections.Generic;
using System.Text;

namespace Shared.VisualComponents
{
   public class TextBoxEx : System.Windows.Forms.TextBox
   {
      [System.Runtime.InteropServices.DllImport("user32.dll", CharSet = System.Runtime.InteropServices.CharSet.Auto)]
      private static extern Int32 SendMessage(IntPtr hWnd, int msg, int wParam, [System.Runtime.InteropServices.MarshalAs(System.Runtime.InteropServices.UnmanagedType.LPWStr)] string lParam);

      [System.Runtime.InteropServices.DllImport("user32.dll", EntryPoint = "SendMessage")]
      private static extern int SendMessage(IntPtr hWnd, int msg, [System.Runtime.InteropServices.MarshalAs(System.Runtime.InteropServices.UnmanagedType.LPWStr)] StringBuilder lpString, int nMaxCount);

      // This is need for when the Banner is set in the UI Designer.
      private string banner = string.Empty;

      [System.ComponentModel.Browsable(true)]
      public string Banner
      {
         get { return GetBanner(); }
         set { SetBanner(value); }
      }

      public void SetBanner(string _msg)
      {
         SetBanner(_msg, true);
      }
      public void SetBanner(string _msg, bool _bStillDisplayOnFocus)
      {
         if (this.IsHandleCreated)
         {
            SendMessage(
                this.Handle,
                EM_SETCUEBANNER,
                _bStillDisplayOnFocus ? 1 : 0,
                _msg);
         }
         this.banner = _msg;
      }
      public string GetBanner()
      {
         StringBuilder builder = new StringBuilder(4096);
         if (this.IsHandleCreated)
            SendMessage(this.Handle, EM_GETCUEBANNER, builder, 4096);
         else
            builder.Append(this.banner);
         return builder.ToString();
      }
      private const int EM_SETCUEBANNER = 0x1501;
      private const int EM_GETCUEBANNER = 0x1502;

      protected override void OnHandleCreated(EventArgs e)
      {
         base.OnHandleCreated(e);
         if (!string.IsNullOrEmpty(this.banner))
            SetBanner(this.banner);
      }
   }
}
