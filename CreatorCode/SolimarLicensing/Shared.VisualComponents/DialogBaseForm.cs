using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Shared.VisualComponents
{
    #region Dialog class

    public partial class DialogBaseForm : Form
    {
        private DialogData data = null;

        #region Constructors & finalizers

        public DialogBaseForm()
        {
            InitializeComponent();
        }
		#endregion

		#region Properties

		/// <summary>
		/// Gets object passed to Execute method as data for this dialog.
		/// </summary>
		protected DialogData Data
		{
			get{ return data; }
		}
		#endregion

		#region Events

		/// <summary>
		/// Allows to perform any initialization before dialog is showed.
		/// </summary>
		[Category("Dialog"),
		Description("Occurs before the dialog is showed.")]
		public event InitDialogEventHandler InitDialog;

		/// <summary>
		/// Fired after the dialog is closed.
		/// </summary>
		[Category("Dialog"), 
		Description("Occurs after the dialog is closed.")]
		public event FinishDialogEventHandler FinishDialog;

		#endregion

		#region Methods

		/// <summary>
		/// Shows the dialog with no owner and no data.
		/// </summary>
		/// <returns>One of the DialogResult values.</returns>
		public new DialogResult ShowDialog()
		{
			return this.ShowDialog(null, null);
		}

		/// <summary>
		/// Shows the dialog with the specified owner and no data.
		/// </summary>
		/// <param name="owner">Any object that implements IWin32Window that represents the top-level window that will own the dialog.</param>
		/// <returns>One of the DialogResult values.</returns>
		public new DialogResult ShowDialog(IWin32Window owner)
		{
			return this.ShowDialog(owner, null);
		}

		/// <summary>
		/// Shows the dialog with no owner and specified data.
		/// </summary>
		/// <param name="data">Any DialogData descendant that contains data for the dialog.</param>
		/// <returns>One of the DialogResult values.</returns>
		public DialogResult ShowDialog(DialogData data)
		{
			return this.ShowDialog(null, data);
		}

		/// <summary>
		/// Shows the dialog with the specified owner and data.
		/// </summary>
		/// <param name="owner">Any object that implements IWin32Window that represents the top-level window that will own the dialog.</param>
		/// <param name="data">Any DialogData descendant that contains data for the dialog.</param>
		/// <returns>One of the DialogResult values.</returns>
		public DialogResult ShowDialog(IWin32Window owner, DialogData data)
		{			
			this.data = data;
			OnInitDialog(new InitDialogEventArgs(this.data));

			DialogResult result = base.ShowDialog(owner);

			OnFinishDialog(new FinishDialogEventArgs(this.data, result));
			return result;
		}

		/// <summary>
		/// Raises the InitDialog event.
		/// </summary>
		/// <param name="e">The InitDialogEventArgs that contains event data.</param>
		protected virtual void OnInitDialog(InitDialogEventArgs e)
		{
			if(InitDialog != null)
				InitDialog(this, e);
		}

		/// <summary>
		/// Raises the FinishDialog event.
		/// </summary>
		/// <param name="e">The FinishDialogEventArgs that contains event data.</param>
		protected virtual void OnFinishDialog(FinishDialogEventArgs e)
		{
			if(FinishDialog != null)
				FinishDialog(this, e);
		}
		#endregion
    }

    #endregion
    #region DialogData class

    /// <summary>
    /// DialogData is the base class for classes containing dialog data.
    /// </summary>
    public class DialogData
    {
        /// <summary>
        /// The value of Empty is a read-only instance of DialogData 
        /// equivalent to the result of calling the DialogData constructor.
        /// </summary>
        public static readonly DialogData Empty = new DialogData();

        /// <summary>
        /// Initializes new instance of a DialogData object.
        /// </summary>
        public DialogData()
        {
        }
    }
    #endregion

    #region InitDialogEventArgs class

    /// <summary>
    /// Provides data for an InitDialog event.
    /// </summary>
    public class InitDialogEventArgs
    {
        private DialogData data = null;

        /// <summary>
        /// Initializes a new instance of the InitDialogEventArgs class 
        /// with the Data property set to null.
        /// </summary>
        internal InitDialogEventArgs()
            : base()
        {
        }

        /// <summary>
        /// Initializes a new instance of the InitDialogEventArgs class 
        /// with the Data property set to the given value.
        /// </summary>
        internal InitDialogEventArgs(DialogData data)
            : base()
        {
            this.data = data;
        }

        /// <summary>
        /// Gets data object associated with the event.
        /// </summary>
        public DialogData Data
        {
            get { return data; }
        }
    }
    #endregion

    #region FinishDialogEventArgs class

    /// <summary>
    /// Provides data for FinishDialog event.
    /// </summary>
    public class FinishDialogEventArgs
    {
        private DialogData data = null;
        private DialogResult result;

        /// <summary>
        /// Initializes a new instance of the FinishDialogEventArgs class 
        /// with the Data property set to null and Result property set to the given value.
        /// </summary>
        internal FinishDialogEventArgs(DialogResult r)
        {
            result = r;
        }

        /// <summary>
        /// Initializes a new instance of the FinishDialogEventArgs class 
        /// with the Data and Result properties set to the given values.
        /// </summary>
        internal FinishDialogEventArgs(DialogData data, DialogResult r)
        {
            this.data = data;
            result = r;
        }

        /// <summary>
        /// Gets data object associated with the event.
        /// </summary>
        public DialogData Data
        {
            get { return data; }
            set { data = value; }
        }

        /// <summary>
        /// Gets a return value of a dialog box.
        /// </summary>
        public DialogResult Result
        {
            get { return result; }
        }
    }
    #endregion

    #region Public delegates

    /// <summary>
    /// Represents the method that will handle InitDialog event.
    /// </summary>
    public delegate void InitDialogEventHandler(object sender, InitDialogEventArgs e);

    /// <summary>
    /// Represents the method that will handle FinishDialog event.
    /// </summary>
    public delegate void FinishDialogEventHandler(object sender, FinishDialogEventArgs e);

    #endregion

}