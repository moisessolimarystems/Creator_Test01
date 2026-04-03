using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Shared.VisualComponents
{
    /// <summary>Represents base class for message dialogs</summary>
    /// <example>
    /// using (Shared.VisualComponents.BaseMessageDialog messageDialog = new Shared.VisualComponents.BaseMessageDialog())
    /// {
    ///		messageDialog.SetData(new Shared.VisualComponents.MessageBoxData());
    ///		messageDialog.SetMessage("Looking for Server: \"{0}\"", serverName);
    ///		messageDialog.Show(this);
    ///		messageDialog.Update();
    ///		...
    ///		messageDialog.SetMessage("More stuff...");
    ///		messageDialog.Update();
    ///		...
    ///		messageDialog.SetMessage("Still more stuff...");
    ///		messageDialog.Update();
    /// }
    /// </example>
    public partial class BaseMessageDialog : Form
    {
        public BaseMessageDialog()
        {
            InitializeComponent();
            InternalInitialize();
        }

        private void InternalInitialize()
        {
        }

        /// <remarks>Have not implemented cancel button</remarks>
        public void SetData(MessageBoxData _data)
        {
            m_data = _data;
            if (m_data != null)
            {
                cancelButton.Visible = m_data.bShowCancelButton;
            }
        }

        /// <summary>Dispalys the formatted message to the message dialog.</summary>
        /// <param name="_message">A composite format string.</param>
        /// <param name="_args">Optional - An System.Object array containing zero or more objects to format.</param>
        public void SetMessage(string _message, params object[] _args)
        {
            //Solimar.SSE.Logging.Log(Solimar.SSE.Logging.ELoggingFlags.Info, _message, _args);
            messageLabel.Text = string.Format(_message, _args);
        }

        protected MessageBoxData m_data;
    }
    public class MessageBoxData
    {
        public bool bShowCancelButton = false;
    }
}
