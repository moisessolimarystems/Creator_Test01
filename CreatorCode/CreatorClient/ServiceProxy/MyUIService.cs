using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;
using System.Reflection;
using System.Windows.Forms;
using System.Windows.Forms.Design;
using System.Globalization;

namespace Client.Creator
{
    //Allows Propertygrid to display custom error messages instead of default error dialog.
    internal class PropertyGridUIService : IUIService
    {
        private PropertyGrid propertyGrid;
        private IDictionary styles;

        internal PropertyGridUIService(PropertyGrid grid)
        {
            this.propertyGrid = grid;
            this.styles = new Hashtable();
        }

        public bool CanShowComponentEditor(object component)
        { // return value doesn't appear to matter
            return false;
        }

        public IWin32Window GetDialogOwnerWindow()
        {
            return this.propertyGrid;
        }

        public void SetUIDirty()
        {
            throw new NotImplementedException();
        }

        public bool ShowComponentEditor(object component, IWin32Window parent)
        {
            throw new NotImplementedException();
        }

        public DialogResult ShowDialog(Form form)
        {
            if (form != null)
            {
                if (string.Compare(form.GetType().FullName, "System.Windows.Forms.PropertyGridInternal.GridErrorDlg", true, CultureInfo.InvariantCulture) == 0)
                { // show custom error message dialog
                    FieldInfo detailsTextBoxField = form.GetType().GetField("details", BindingFlags.NonPublic | BindingFlags.Instance);
                    TextBox detailsTextBox = detailsTextBoxField.GetValue(form) as TextBox;

                    return MessageBox.Show(this.propertyGrid,
                        string.Format(CultureInfo.InvariantCulture, "{0}{1}{1}{2}", detailsTextBox.Text, Environment.NewLine, "Select Cancel to revert to the previously valid value."),
                        this.propertyGrid.SelectedGridItem.PropertyDescriptor.DisplayName.Trim(),
                        MessageBoxButtons.OKCancel,
                        MessageBoxIcon.Warning);
                }
                else
                { // show collection editor
                    return form.ShowDialog(this.propertyGrid);
                }
            }

            return DialogResult.Cancel;
        }

        public void ShowError(Exception ex, string message)
        {
            throw new NotImplementedException();
        }

        public void ShowError(Exception ex)
        {
            throw new NotImplementedException();
        }

        public void ShowError(string message)
        {
            throw new NotImplementedException();
        }

        public DialogResult ShowMessage(string message, string caption, MessageBoxButtons buttons)
        {
            throw new NotImplementedException();
        }

        public void ShowMessage(string message, string caption)
        {
            throw new NotImplementedException();
        }

        public void ShowMessage(string message)
        {
            throw new NotImplementedException();
        }

        public bool ShowToolWindow(Guid toolWindow)
        {
            throw new NotImplementedException();
        }

        public IDictionary Styles
        {
            get
            {
                return this.styles;
            }
        }
    }

    internal class PropertyGridServiceProvider : IServiceProvider
    {
        private PropertyGridUIService service;

        internal PropertyGridServiceProvider(PropertyGrid grid)
        {
            this.service = new PropertyGridUIService(grid);
        }

        public object GetService(Type serviceType)
        {
            if (serviceType == typeof(IUIService))
            {
                return this.service;
            }
            return null;
        }
    }
}
