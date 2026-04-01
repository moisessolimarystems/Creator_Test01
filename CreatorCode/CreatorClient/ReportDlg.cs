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
        public ReportDlg()
        {
            InitializeComponent();
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
                conditionNameComboBox.DataSource = Enum.GetValues(typeof(CreatorService.Name));
                //conditionNameComboBox.Items.AddRange(System.Enum.GetNames(typeof(CreatorWCF.StringName)));
                conditionOperatorComboBox.DataSource = Enum.GetValues(typeof(CreatorService.Operator));
                if (d.Report.Conditions.Count > 0)
                {
                    conditionNameComboBox.SelectedIndex = (int)d.Report.Conditions[0].ConditionName;
                    conditionOperatorComboBox.SelectedIndex = (int)d.Report.Conditions[0].ConditionOperator;
                    conditionValueTextBox.Text = d.Report.Conditions[0].ConditionValue;
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

            CreatorService.Condition newCondition = new CreatorService.Condition();
            newCondition.ConditionName = (CreatorService.Name)conditionNameComboBox.SelectedItem;
            newCondition.ConditionOperator = (CreatorService.Operator)conditionOperatorComboBox.SelectedItem;            
            newCondition.ConditionValue = conditionValueTextBox.Text;   
            d.Report.Conditions.Add(newCondition);
        }

        private void conditionNameComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            //change operator values to corresponding type
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
            _report.Name = newReport.Name;
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
