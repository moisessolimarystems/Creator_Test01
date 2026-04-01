using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SolimarLicenseDiagnosticDataViewer
{
    public class Base_DisplayForm : Form
    {
        public Base_DisplayForm()
        {
            InitializeComponent();
        }
        private const int CP_NOCLOSE_BUTTON = 0x200;
        protected override CreateParams CreateParams
        {
            get
            {
                CreateParams cp = base.CreateParams;
                cp.ClassStyle |= CP_NOCLOSE_BUTTON;	// Disable Close button
                return cp;
            }
        }

        private void InitializeComponent()
        {
            this.SuspendLayout();
            // 
            // Base_DisplayForm
            // 
            this.ClientSize = new System.Drawing.Size(284, 264);
            this.Name = "Base_DisplayForm";
            this.ShowIcon = false;
            this.ResumeLayout(false);

        }
    }

    //public class Base_DisplayForm<T> : Base_DisplayForm, ISetData<T>
    //{
    //    public Base_DisplayForm()
    //        : base()
    //    {
    //    }
    //    //Implements ISetData<T>
    //    virtual public void SetData(T t) { }
    //}

    //public class Base_DisplayForm<T1, T2> : Base_DisplayForm, ISetData<T1, T2>
    //{
    //    public Base_DisplayForm()
    //    {
    //    }
    //    //Implements ISetData<T1, T2>
    //    virtual public void SetData(T1 t1, T2 t2) { }
    //}

    //public interface ISetData<T>
    //{
    //    void SetData(T t);
    //}
    //public interface ISetData<T1, T2>
    //{
    //    void SetData(T1 t1, T2 t2);
    //}
}
