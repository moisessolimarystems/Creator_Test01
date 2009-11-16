using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Client.Creator
{
    public partial class ReportDlg : Shared.VisualComponents.DialogBaseForm
    {
        private int m_ConditionCount;
        private bool m_Validated;

        public ReportDlg()
        {
            InitializeComponent();
            m_ConditionCount = 1;
            m_Validated = false;
        }

        private void ReportDlg_InitDialog(object sender, Shared.VisualComponents.InitDialogEventArgs e)
        {
            ReportDlgData d = e.Data as ReportDlgData;
            if (d != null)
            {
                //each condition name corresponds to a different operator list
                //on selection change for name, repopulate operator combobox
                //need to determine type of conditionname
                //ways to determine condition name 
                //1) separate into different types of names .. stringname, numbername, datename
                //2) fill combobox with different name type objects to allow getting of type
                //conditionNameComboBox.DataSource = Enum.GetValues(typeof(CreatorService.ConditionName));
                //conditionNameComboBox.Items.AddRange(System.Enum.GetNames(typeof(CreatorWCF.StringName)));
                //conditionOperatorComboBox.DataSource = Enum.GetValues(typeof(CreatorService.ConditionOperator));
                foreach (string key in ReportProperty._filterNames.Keys)
                {
                    conditionNameComboBox.Items.Add(key);
                }
                //conditionNameComboBox.DataSource = ReportProperty._filterNames.Keys;
                foreach (string key in ReportProperty._filterOperators.Keys)
                {
                    conditionOperatorComboBox.Items.Add(key);
                }
                //conditionOperatorComboBox.DataSource = ReportProperty._filterOperators.Keys;
                if (d.Report.Conditions.Count > 0)
                {
                    conditionNameComboBox.SelectedIndex = (int)d.Report.Conditions[0].Name;
                    conditionOperatorComboBox.SelectedIndex = (int)d.Report.Conditions[0].Operator;
                    conditionValueTextBox.Text = d.Report.Conditions[0].Value;
                }
                else
                {
                    conditionNameComboBox.SelectedIndex = 0;
                    conditionOperatorComboBox.SelectedIndex = 0;
                }
            }    
        }

        private void ReportDlg_FinishDialog(object sender, Shared.VisualComponents.FinishDialogEventArgs e)
        {
            if (e.Result != DialogResult.OK)
                return;

            ReportDlgData d = e.Data as ReportDlgData;
            if (d == null)
                e.Data = d = new ReportDlgData();
            else
                d.Report.Conditions.Clear();
            for (int conditionIndex = 0; conditionIndex < m_ConditionCount; conditionIndex++)
            {    
                CreatorService.Condition newCondition = new CreatorService.Condition();
                newCondition.Name = ReportProperty._filterNames[conditionNameComboBox.SelectedItem.ToString()];
                newCondition.Operator = ReportProperty._filterOperators[conditionOperatorComboBox.SelectedItem.ToString()];
                newCondition.Value = conditionValueTextBox.Text;
                d.Report.Conditions.Add(newCondition);
            }
        }

        private void conditionNameComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            //change operator values to corresponding type
        }

        //creates generic name combobox, operator
        //create generic value edit box
        //create generic add, minus button
        private void conditionPlusButton_Click(object sender, EventArgs e)
        {

        }

        private void ReportDlg_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (DialogResult == DialogResult.OK)
                if (!m_Validated)
                    e.Cancel = true;
        }

        private void btnOk_Click(object sender, EventArgs e)
        {
            m_Validated = true;
        }
    }

    #region ReportDlgData class
    public class ReportDlgData : Shared.VisualComponents.DialogData
    {
        private ReportProperty _report;

        #region Constructors

        public ReportDlgData()
        {
            _report = new ReportProperty();            
        }

        public ReportDlgData(ReportProperty newReport)
        {
            _report = new ReportProperty();
            _report.ID = newReport.ID;
            _report.Conditions = newReport.Conditions;            
        }
        #endregion

        #region Properties

        public ReportProperty Report
        {
            get { return _report; }
            set { _report = value; }
        }
        #endregion
    }
    #endregion
}
