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
        private int m_CondtionIndex;
        private bool m_Validated;
        private ReportDlgData _reportData;
        private CommunicationLink _commLink;
        private const int MAXIMUM_CONDITIONS = 9;

        public ReportDlg(CommunicationLink commLink)
        {
            InitializeComponent();
            m_ConditionCount = m_CondtionIndex = 0;
            m_Validated = false;
            _commLink = commLink;
            _reportData = null;
        }

        private void ReportDlg_InitDialog(object sender, Shared.VisualComponents.InitDialogEventArgs e)
        {
            _reportData = e.Data as ReportDlgData;
            if (_reportData != null)
            {
                this.Text = _reportData.Report.ID;
                if (_reportData.Report.Conditions.Count == 0)
                {
                    createConditionInput(new CreatorService.Condition());
                }
                else
                {
                    foreach (CreatorService.Condition userCondition in _reportData.Report.Conditions)
                    {
                        createConditionInput(userCondition);
                    }
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
            
            int controlIndex;
            ComboBox cb;
            TextBox tb;
            foreach(Control control in groupBox1.Controls)
            {
                if (control.Name.Contains("conditionComboBox") && control.Name != conditionNameComboBox.Name)
                {
                    //Condition Name
                    cb = (ComboBox)control;
                    string index = control.Name.Substring(control.Name.Length - 1, 1);
                    CreatorService.Condition newCondition = new CreatorService.Condition();
                    if (_reportData.Report.Type == ReportProperty.ReportType.LicenseServer)
                        newCondition.Name = ReportProperty._filterLSNames[cb.SelectedItem.ToString()];
                    else
                        newCondition.Name = ReportProperty._filterPLNames[cb.SelectedItem.ToString()];
                    //Eval Operator
                    controlIndex = groupBox1.Controls.IndexOfKey(string.Format("evalComboBox{0}", index));
                    cb = (ComboBox)groupBox1.Controls[controlIndex];
                    newCondition.Operator = ReportProperty._filterOperators[cb.SelectedItem.ToString()];
                    //Value TextBox
                    controlIndex = groupBox1.Controls.IndexOfKey(string.Format("valueTextBox{0}", index));
                    tb = (TextBox)groupBox1.Controls[controlIndex];                                    
                    newCondition.Value = tb.Text;                    
                    d.Report.Conditions.Add(newCondition);
                }
            }
            
        }

        private void conditionNameComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            //change operator values to corresponding type
        }

        private void conditionPlusButton_Click(object sender, EventArgs e)
        {
            //create
            if (m_ConditionCount <= MAXIMUM_CONDITIONS)
                createConditionInput(new CreatorService.Condition());
            else
                MessageBox.Show("Maximum number of conditions has been reached!", "Condition Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        private void conditionMinusButton_Click(object sender, EventArgs e)
        {
            //remove 
            Button selectedButton = (Button)sender;
            string conditionIndex = selectedButton.Name.Substring(selectedButton.Name.Length-1, 1);
            removeConditionInput(conditionIndex, selectedButton.Location.Y);
        }

        private void createConditionInput(CreatorService.Condition userCondition)
        {
            //increase form size
            //increase group size
            if (m_ConditionCount > 0)
            {
                groupBox1.Size = new Size(groupBox1.Size.Width, groupBox1.Height + 25);
                this.Size = new Size(this.Size.Width, this.Height + 25);
            }

            ComboBox conditionComboBox = new ComboBox();
            conditionComboBox.Name = string.Format("conditionComboBox{0}", m_CondtionIndex);
            conditionComboBox.Location = new Point(conditionNameComboBox.Location.X, conditionNameComboBox.Location.Y + (m_ConditionCount * 25));
            conditionComboBox.Size = conditionNameComboBox.Size;
            conditionComboBox.DropDownStyle = ComboBoxStyle.DropDownList;
            if (_reportData.Report.Type == ReportProperty.ReportType.LicenseServer)
            {
                foreach (string key in ReportProperty._filterLSNames.Keys)
                {
                    conditionComboBox.Items.Add(key);
                }
            }
            else
            {
                foreach (string key in ReportProperty._filterPLNames.Keys)
                {
                    conditionComboBox.Items.Add(key);
                }
            }
            //conditionComboBox has _filter Keys 
            conditionComboBox.SelectedItem = _reportData.Report.GetFilterKey(userCondition);
            groupBox1.Controls.Add(conditionComboBox);

            ComboBox evalComboBox = new ComboBox();
            evalComboBox.Name = string.Format("evalComboBox{0}", m_CondtionIndex);
            evalComboBox.Location = new Point(conditionOperatorComboBox.Location.X, conditionOperatorComboBox.Location.Y + (m_ConditionCount * 25));
            evalComboBox.Size = conditionOperatorComboBox.Size;
            evalComboBox.DropDownStyle = ComboBoxStyle.DropDownList;
            foreach (string key in ReportProperty._filterOperators.Keys)
            {
                evalComboBox.Items.Add(key);
            }
            evalComboBox.SelectedIndex = (int)userCondition.Operator;
            groupBox1.Controls.Add(evalComboBox);

            TextBox valueTextBox = new TextBox();
            valueTextBox.Name = string.Format("valueTextBox{0}", m_CondtionIndex);
            valueTextBox.Location = new Point(conditionValueTextBox.Location.X, conditionValueTextBox.Location.Y + (m_ConditionCount * 25));
            valueTextBox.Size = conditionValueTextBox.Size;
            valueTextBox.Text = userCondition.Value;
            groupBox1.Controls.Add(valueTextBox);

            Button minusButton = new Button();
            minusButton.Name = string.Format("minusButton{0}", m_CondtionIndex);
            minusButton.Text = "-";
            minusButton.Click += new EventHandler(conditionMinusButton_Click);
            minusButton.Location = new Point(conditionMinusButton.Location.X, conditionMinusButton.Location.Y + (m_ConditionCount * 25));
            minusButton.Size = conditionMinusButton.Size;
            groupBox1.Controls.Add(minusButton);
            foreach (Control control in groupBox1.Controls)
            {
                if (control.Location == conditionMinusButton.Location && control.Name != conditionMinusButton.Name)
                {
                    control.Enabled = (m_ConditionCount > 0);
                    break;
                }
            }  

            Button plusButton = new Button();
            plusButton.Name = string.Format("plusButton{0}", m_CondtionIndex);
            plusButton.Click += new EventHandler(conditionPlusButton_Click);
            plusButton.Text = "+";
            plusButton.Location = new Point(conditionPlusButton.Location.X, conditionPlusButton.Location.Y + (m_ConditionCount * 25));
            plusButton.Size = conditionPlusButton.Size;
            groupBox1.Controls.Add(plusButton);            
            m_ConditionCount++;
            m_CondtionIndex++;
        }

        private void removeConditionInput(string conditionIndex, int yPos)
        {
            groupBox1.Size = new Size(groupBox1.Size.Width, groupBox1.Height - 25);
            this.Size = new Size(this.Size.Width, this.Height - 25);
            //need to rearrange all other lines
            groupBox1.Controls.RemoveByKey(string.Format("conditionComboBox{0}", conditionIndex));
            groupBox1.Controls.RemoveByKey(string.Format("evalComboBox{0}", conditionIndex));
            groupBox1.Controls.RemoveByKey(string.Format("valueTextBox{0}", conditionIndex));
            groupBox1.Controls.RemoveByKey(string.Format("minusButton{0}", conditionIndex));
            groupBox1.Controls.RemoveByKey(string.Format("plusButton{0}", conditionIndex));

            int index;
            foreach (Control control in groupBox1.Controls)
            {                
                if(int.TryParse(control.Name.Substring(control.Name.Length - 1, 1),out index))
                    if (index > int.Parse(conditionIndex))                   
                        control.Location = new Point(control.Location.X, control.Location.Y - 25);
            }
            m_ConditionCount--;
            foreach (Control control in groupBox1.Controls)
            {
                if (control.Location == conditionMinusButton.Location && control.Name != conditionMinusButton.Name)
                {
                    control.Enabled = (m_ConditionCount > 1);
                    break;
                }
            }  
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
