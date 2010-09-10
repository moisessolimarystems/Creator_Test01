using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SolimarLicenseDiagnosticDataViewer
{
    abstract public class Base_DisplayForm : Form
    {
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
    }

    abstract public class Base_DisplayForm<T> : Base_DisplayForm, ISetData<T>
    {
        //Implements ISetData<T>
        abstract public void SetData(T t);
    }
    abstract public class Base_DisplayForm<T1, T2> : Base_DisplayForm, ISetData<T1, T2>
    {
        //Implements ISetData<T1, T2>
        abstract public void SetData(T1 t1, T2 t2);
    }

    public interface ISetData<T>
    {
        void SetData(T t);
    }
    public interface ISetData<T1, T2>
    {
        void SetData(T1 t1, T2 t2);
    }
}
