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
        private int m_ConditionIndex;
        private bool m_Validated;
        private ReportDlgData _reportData;
        private const int MAXIMUM_CONDITIONS = 9;

        public ReportDlg()
        {
            InitializeComponent();
            m_ConditionCount = m_ConditionIndex = 0;
            //initialize selected item in matchComboBox to be "all"
            matchComboBox.SelectedIndex = 0;
            m_Validated = false;
            _reportData = null;
        }

        private void ReportDlg_InitDialog(object sender, Shared.VisualComponents.InitDialogEventArgs e)
        {
            _reportData = e.Data as ReportDlgData;
            if (_reportData != null)
            {
                this.Text = string.Format("{0} - {1}", _reportData.Report.Type.ToString(),_reportData.Report.ID);
                matchComboBox.SelectedIndex = (_reportData.Report.MatchAll) ? 0 : 1;
                if (_reportData.Report.Conditions.Count == 0)
                {
                    createConditionInput(new CreatorService.Condition());
                }
                else
                {
                    foreach (CreatorService.Condition userCondition in _reportData.Report.Conditions)
                    {
                        //translate from db to user value
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
                    newCondition.Name = _reportData.Report.GetFilterName(cb.SelectedItem.ToString()); 
                    ReportProperty.ConditionNameType cnt = _reportData.Report.GetConditionNameType(newCondition.Name);
                    //Eval Operator
                    controlIndex = groupBox1.Controls.IndexOfKey(string.Format("evalComboBox{0}", index));
                    cb = (ComboBox)groupBox1.Controls[controlIndex];
                    int evalIndex = cb.SelectedIndex;
                    newCondition.Operator = _reportData.Report.GetFilterOperator(newCondition.Name, cb.SelectedItem.ToString());                                        
                    if(cnt== ReportProperty.ConditionNameType.Number || cnt == ReportProperty.ConditionNameType.String)
                    {
                        controlIndex = groupBox1.Controls.IndexOfKey(string.Format("valueTextBox{0}", index));
                        tb = (TextBox)groupBox1.Controls[controlIndex];
                        newCondition.Value = tb.Text;
                    }
                    else if(cnt == ReportProperty.ConditionNameType.Defined)
                    {
                        controlIndex = groupBox1.Controls.IndexOfKey(string.Format("definedValueComboBox{0}", index));
                        cb = (ComboBox)groupBox1.Controls[controlIndex];
                        newCondition.Value = cb.SelectedItem.ToString();
                    }
                    else if (cnt == ReportProperty.ConditionNameType.Bool)
                    { 
                        controlIndex = groupBox1.Controls.IndexOfKey(string.Format("evalComboBox{0}", index));
                        newCondition.Value = (evalIndex == 0) ? bool.TrueString : bool.FalseString;
                    }
                    else
                    {   //date type
                        controlIndex = groupBox1.Controls.IndexOfKey(string.Format("valueTextBox{0}", index));
                        tb = (TextBox)groupBox1.Controls[controlIndex];
                        newCondition.Value = tb.Text;
                        if(newCondition.Operator == Client.Creator.CreatorService.ConditionOperator.IsInTheLast ||
                           newCondition.Operator == Client.Creator.CreatorService.ConditionOperator.IsInTheNext)
                        {
                            controlIndex = groupBox1.Controls.IndexOfKey(string.Format("valueTypeComboBox{0}", index));
                            cb = (ComboBox)groupBox1.Controls[controlIndex];
                            newCondition.ValueType = cb.SelectedItem.ToString();
                        }
                    }

                    //    tb = (TextBox)groupBox1.Controls[controlIndex];                                    
                    //newCondition.Value = tb.Text;
                    d.Report.Conditions.Add(newCondition);
                }
            }
            d.Report.MatchAll = (matchComboBox.SelectedIndex == 0);
        }

        private void conditionNameComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            //change operator values to corresponding type
            //set operator combobox with 
            ComboBox selectedComboBox = (ComboBox)sender;
            string conditionIndex = selectedComboBox.Name.Substring(selectedComboBox.Name.Length - 1, 1);
            int controlIndex = groupBox1.Controls.IndexOfKey(string.Format("evalComboBox{0}", conditionIndex));
            if(controlIndex >= 0)
            {
                ComboBox evalComboBox = (ComboBox)groupBox1.Controls[controlIndex];
                evalComboBox.Items.Clear();
                Client.Creator.CreatorService.ConditionName cn = _reportData.Report.GetFilterName(selectedComboBox.SelectedItem.ToString());//(Client.Creator.CreatorService.ConditionName)Enum.Parse(typeof(Client.Creator.CreatorService.ConditionName), selectedComboBox.SelectedItem.ToString());
                foreach (string key in _reportData.Report.GetOperatorKeys(cn))
                {
                    evalComboBox.Items.Add(key);
                }
                if (evalComboBox.Items.Count > 0)
                {
                    evalComboBox.SelectedIndex = 0;
                    ReportProperty.ConditionNameType cnt = _reportData.Report.GetConditionNameType(cn);
                    if (cnt == ReportProperty.ConditionNameType.Defined)
                    {
                        controlIndex = groupBox1.Controls.IndexOfKey(string.Format("definedValueComboBox{0}", conditionIndex));
                        ComboBox cb = (ComboBox)groupBox1.Controls[controlIndex];
                        cb.SelectedIndex = 0;
                    }
                    else
                    {
                        controlIndex = groupBox1.Controls.IndexOfKey(string.Format("valueTextBox{0}", conditionIndex));
                        TextBox tb = (TextBox)groupBox1.Controls[controlIndex];
                        if (cnt == ReportProperty.ConditionNameType.Date)
                            tb.Text = DateTime.Today.ToShortDateString();
                        else
                            tb.Text = "";
                    }
                    //set initial values for each value control
                }
            }
        }

        private void conditionOperatorComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            ComboBox selectedComboBox = (ComboBox)sender;
            string conditionIndex = selectedComboBox.Name.Substring(selectedComboBox.Name.Length - 1, 1);        
            int conditionValueIndex = groupBox1.Controls.IndexOfKey(string.Format("valueTextBox{0}", conditionIndex));
            int conditionValueTypeIndex = groupBox1.Controls.IndexOfKey(string.Format("valueTypeComboBox{0}", conditionIndex));
            int conditionValueComboBoxIndex = groupBox1.Controls.IndexOfKey(string.Format("definedValueComboBox{0}", conditionIndex));
            int conditionNameIndex = groupBox1.Controls.IndexOfKey(string.Format("conditionComboBox{0}", conditionIndex));   
            ComboBox cb = (ComboBox)groupBox1.Controls[conditionNameIndex];
            Client.Creator.CreatorService.ConditionName cn = _reportData.Report.GetFilterName(cb.SelectedItem.ToString());
            ReportProperty.ConditionNameType cnt = _reportData.Report.GetConditionNameType(cn);
            TextBox valueTB = (TextBox)groupBox1.Controls[conditionValueIndex];
            valueTB.Visible = false;
            ComboBox valueTypeCB = (ComboBox)groupBox1.Controls[conditionValueTypeIndex];
            valueTypeCB.Visible = false;
            ComboBox valueCB = (ComboBox)groupBox1.Controls[conditionValueComboBoxIndex];
            valueCB.Visible = false;
            if (cnt == ReportProperty.ConditionNameType.String || cnt == ReportProperty.ConditionNameType.Number)
            {
                valueTB.Visible = true;
                valueTB.Size = conditionValueComboBox.Size;                
                //valueTB.Text = "";
            }
            else if (cnt == ReportProperty.ConditionNameType.Defined)
            {
                valueCB.Visible = true;
                ComboBox definedCB = (ComboBox)valueCB;
                int currentIndex = definedCB.SelectedIndex;
                definedCB.Items.Clear();
                definedCB.Items.AddRange(_reportData.Report.GetDefinedValues(cn));
                definedCB.SelectedIndex = (currentIndex < 0) ? 0 : currentIndex;
            }
            else if(cnt == ReportProperty.ConditionNameType.Date)
            { //date
                valueTB.Visible = true;
                string selectedOperator = selectedComboBox.SelectedItem.ToString();
                Client.Creator.CreatorService.ConditionOperator co = _reportData.Report.GetFilterOperator(cn, selectedOperator);
                if (co == Client.Creator.CreatorService.ConditionOperator.IsInTheLast ||
                    co == Client.Creator.CreatorService.ConditionOperator.IsInTheNext)
                {
                    valueTypeCB.Visible = true;
                    valueTB.Size = conditionValueTextBox.Size;
                    valueTypeCB.Items.AddRange(Enum.GetNames(typeof(ReportProperty.ExpirationValueType)));
                    if(valueTypeCB.SelectedItem == null) valueTypeCB.SelectedIndex = 0;
                    int userValue;
                    if (!Int32.TryParse(valueTB.Text, out userValue)) valueTB.Text = "1";
                }
                else
                {
                    valueTB.Size = conditionValueComboBox.Size;
                    DateTime userDateValue;
                    if (!DateTime.TryParse(valueTB.Text, out userDateValue))
                        valueTB.Text = DateTime.Today.ToShortDateString();
                }
               // valueTB.Size = (valueTypeCB.Visible) ? conditionValueTextBox.Size : conditionValueComboBox.Size;
            }
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

        //populate all possible controls
        //select will populate the controls with correct entries and show/hide necessary controls
        private void createConditionInput(CreatorService.Condition userCondition)
        {
            //increase form size
            //increase group size
            matchComboBox.Visible = (m_ConditionCount > 0);
            matchSingleLabel.Visible = (m_ConditionCount == 0);
            matchMultipleLabel.Visible = (m_ConditionCount > 0);
            if (m_ConditionCount > 0)
            {
                groupBox1.Size = new Size(groupBox1.Size.Width, groupBox1.Height + 25);
                this.Size = new Size(this.Size.Width, this.Height + 25);
            }

            ComboBox conditionComboBox = new ComboBox();
            conditionComboBox.Name = string.Format("conditionComboBox{0}", m_ConditionIndex);
            conditionComboBox.Location = new Point(conditionNameComboBox.Location.X, conditionNameComboBox.Location.Y + (m_ConditionCount * 25));
            conditionComboBox.Size = conditionNameComboBox.Size;
            conditionComboBox.DropDownStyle = ComboBoxStyle.DropDownList;
            conditionComboBox.SelectedIndexChanged += new EventHandler(conditionNameComboBox_SelectedIndexChanged);
            foreach (string key in _reportData.Report.GetNameKeys())
            {
                conditionComboBox.Items.Add(key);
            }
            //conditionComboBox has _filter Keys 

            groupBox1.Controls.Add(conditionComboBox);
            ComboBox evalComboBox = new ComboBox();
            evalComboBox.Name = string.Format("evalComboBox{0}", m_ConditionIndex);
            evalComboBox.Location = new Point(conditionOperatorComboBox.Location.X, conditionOperatorComboBox.Location.Y + (m_ConditionCount * 25));
            evalComboBox.Size = conditionOperatorComboBox.Size;
            evalComboBox.DropDownStyle = ComboBoxStyle.DropDownList;
            evalComboBox.SelectedIndexChanged += new EventHandler(conditionOperatorComboBox_SelectedIndexChanged);
            groupBox1.Controls.Add(evalComboBox);

            TextBox valueTextBox = new TextBox();
            valueTextBox.Name = string.Format("valueTextBox{0}", m_ConditionIndex);
            valueTextBox.Location = new Point(conditionValueTextBox.Location.X, conditionValueTextBox.Location.Y + (m_ConditionCount * 25));
            valueTextBox.Size = conditionValueTextBox.Size; 
            groupBox1.Controls.Add(valueTextBox);

            ComboBox valueTypeComboBox = new ComboBox();
            valueTypeComboBox.Name = string.Format("valueTypeComboBox{0}", m_ConditionIndex);
            valueTypeComboBox.Location = new Point(conditionValueTypeComboBox.Location.X, conditionValueTypeComboBox.Location.Y + (m_ConditionCount * 25));
            valueTypeComboBox.Size = conditionValueTypeComboBox.Size;
            valueTypeComboBox.DropDownStyle = ComboBoxStyle.DropDownList;        
            groupBox1.Controls.Add(valueTypeComboBox);

            ComboBox definedValueComboBox = new ComboBox();
            definedValueComboBox.Name = string.Format("definedValueComboBox{0}", m_ConditionIndex);
            definedValueComboBox.Location = new Point(conditionValueComboBox.Location.X, conditionValueComboBox.Location.Y + (m_ConditionCount * 25));
            definedValueComboBox.Size = conditionValueComboBox.Size; 
            definedValueComboBox.DropDownStyle = ComboBoxStyle.DropDownList;
            groupBox1.Controls.Add(definedValueComboBox);
       
            Button minusButton = new Button();
            minusButton.Name = string.Format("minusButton{0}", m_ConditionIndex);
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
            plusButton.Name = string.Format("plusButton{0}", m_ConditionIndex);
            plusButton.Click += new EventHandler(conditionPlusButton_Click);
            plusButton.Text = "+";
            plusButton.Location = new Point(conditionPlusButton.Location.X, conditionPlusButton.Location.Y + (m_ConditionCount * 25));
            plusButton.Size = conditionPlusButton.Size;
            groupBox1.Controls.Add(plusButton);

            conditionComboBox.SelectedItem = _reportData.Report.GetFilterKey(userCondition);
            if (conditionComboBox.SelectedItem == null) conditionComboBox.SelectedItem = "Customer";
            evalComboBox.SelectedItem = _reportData.Report.GetOperatorKey(userCondition);  
            //3 value controls do not have their information updated, need to retrieve from groupbox control list
            //3 types
            //string,number, date textbox
            //defined 
            //date with special op
            valueTextBox.Text = userCondition.Value;
            valueTypeComboBox.SelectedItem = userCondition.ValueType;
            definedValueComboBox.SelectedItem = userCondition.Value;
            valueTypeComboBox.SelectedItem = userCondition.ValueType;
            m_ConditionCount++;
            m_ConditionIndex++;
        }

        private void removeConditionInput(string conditionIndex, int yPos)
        {
            groupBox1.Size = new Size(groupBox1.Size.Width, groupBox1.Height - 25);
            this.Size = new Size(this.Size.Width, this.Height - 25);
            //need to rearrange all other lines
            groupBox1.Controls.RemoveByKey(string.Format("conditionComboBox{0}", conditionIndex));
            groupBox1.Controls.RemoveByKey(string.Format("evalComboBox{0}", conditionIndex));
            groupBox1.Controls.RemoveByKey(string.Format("valueTextBox{0}", conditionIndex));
            groupBox1.Controls.RemoveByKey(string.Format("valueTypeComboBoxBox{0}", conditionIndex));
            groupBox1.Controls.RemoveByKey(string.Format("definedValueComboBox{0}", conditionIndex));
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
            matchComboBox.Visible = (m_ConditionCount > 1);
            matchSingleLabel.Visible = (m_ConditionCount == 1);
            matchMultipleLabel.Visible = (m_ConditionCount > 1);
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

        private void matchCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            btnOk.Enabled = matchCheckBox.Checked;
            foreach (Control control in groupBox1.Controls)
            {
                control.Enabled = matchCheckBox.Checked;
            }
        }

        private void conditionValueTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            //validate string, date, number value.
        }


    }

    #region ReportDlgData class
    public class ReportDlgData : Shared.VisualComponents.DialogData
    {
        private ReportProperty _report;
        private CommunicationLink _commLink;

        #region Constructors
        public ReportDlgData()
        {
            _commLink = new CommunicationLink();
            _report = new ReportProperty(_commLink);
        }

        public ReportDlgData(CommunicationLink commLink)
        {
            _report = new ReportProperty(commLink);            
        }

        public ReportDlgData(ReportProperty newReport, CommunicationLink commLink)
        {
            _report = new ReportProperty(commLink);
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

        public CommunicationLink CommLink
        {
            get { return _commLink; }
        }
        #endregion
    }
    #endregion
}
