//==============================================================================
// Filename:    KeyWizard.cpp
//==============================================================================


//==============================================================================
//  Class: KeyWizard
//  Wizard for the key creation process.
//==============================================================================
#include <vcl.h>
#pragma hdrstop

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "KeyWizard.h"
#include <mem.h>
//------------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SftTree_OCX"
#pragma link "ModuleDetail"
#pragma link "SolSearcherEnterpriseDetails"
#pragma resource "*.dfm"
TKeyWizardFrm *KeyWizardFrm;


//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------
//static member initialized
const int TKeyWizardFrm::convertProductToIndex[] = { 0/*SPD_NT*/, 3/*XImage*/, 2/*XImage NT*/, 4/*Indexer*/, 6/*ConnectivityServer*/, 0/*SPD_LEGACY*/, 7/*iConvert*/, 8/*SS Enterprise*/, 9/*SOLscript*/, 10/*SDX_DESIGNER_PRODUCT*/, 11/*Quantum Server*/, 12/*Rubika*/, 13/*SOLFUSION*/, 14/*SPDE*/};
const int TKeyWizardFrm::convertIndexToProduct[] = { SPD_PRODUCT, XIMAGENT_PRODUCT, XIMAGE_PRODUCT, INDEX_PLUGIN, CONNECT_PRODUCT, SPD_PRODUCT, ICONVERT_PRODUCT, SOLSEARCHER_ENTERPRISE_PRODUCT, SOLSCRIPT_PRODUCT, SDX_DESIGNER_PRODUCT, QUANTUM_PRODUCT, RUBIKA_PRODUCT, SOLFUSION_PRODUCT, SPDE_PRODUCT};


//==============================================================================
// Function:    TKeyWizardFrm()
// Purpose:     Constructor
//==============================================================================
__fastcall TKeyWizardFrm::TKeyWizardFrm(SKeyRecord* _key_record, TComponent* Owner)
   : TForm(Owner), key_record(_key_record), /*key(_key_record->pkey),*/ m_selectedProduct(-1)
{
   initializeWizard();
}

//==============================================================================
// Function:    InitialWizard() -
// Purpose:     Initializes the KeyWizard
// Parameters:  None
// Returns:     None
//==============================================================================
void TKeyWizardFrm::initializeWizard()
{
   edited_days = false;
   edited_days_value = DEFAULT_DAYS;
   edited_hours_value = DEFAULT_DAYS*(HOURS_PER_DAY);
   edited_version = false;
   edited_system_id = key_record->system_id;
   wizard_state = wsStart;
   active_panel = StartPanel;
   active_panel->BringToFront();
   setProductComboBox( static_cast<ProductId>(key_record->pkey->productId) );
   setKeyClassComboBox();
   setPasswordComboBox();
   WizardModuleFrame->initialize(MODE_3, key_record->pkey->productId);
   state.push(wsStart);

   eBONumber = eBONumberEditBox->Text;
}

//==============================================================================
// Function:    setProductComboBox()
// Purpose:     Set combobox and issue a OnChangeEvent to update GUI.
// Parameters:  ProductId - _product
// Returns:     None
//==============================================================================
void TKeyWizardFrm::setProductComboBox(ProductId _product)
{
   ProductComboBox->ItemIndex = convertProductToIndex[_product];
   ProductComboBoxChange(this);
}


/* Function:    setPasswordComboBox()
 * Purpose:     Initailize combobox with list of expiration hrs/days combinations
 *    Sets default value and triggers the on change event.
 * Parameters:  NONE
 * Returns:     None
 *----------------------------------------------------------------------------*/
void TKeyWizardFrm::setPasswordComboBox()
{
    try
    {
        for (int i = 0; i < KD_PASSWORD_MAX; i++) {
            passwordComboBox->Items->Add((key_record->pkey->getKDPasswordText(i)));
        }

        passwordComboBox->ItemIndex = 0;
        edited_days_value = key_record->pkey->getKDPasswordDays(passwordComboBox->ItemIndex);
        edited_hours_value = key_record->pkey->getKDPasswordHours(passwordComboBox->ItemIndex);
    }
    catch(EDBEngineError &e)
    {
        passwordComboBox->Enabled = false;
    }

    passwordComboBoxChange(this);
}

//==============================================================================
// Function:    SetProductComboBox()
// Purpose:     Set combobox and issue a OnChangeEvent to update GUI.
// Parameters:  ProductId - _product
// Returns:     None
//==============================================================================
void TKeyWizardFrm::setKeyClassComboBox()
{
   keyClassComboBox->ItemIndex = USB_KEY;
   keyClassComboBoxChange(this);
}

void TKeyWizardFrm::resetKeyWizardFrm()
{
   OutputUnits->Visible = false;
   OutputEdit->Visible = false;
   OutputLabel->Visible = false;
   ConcurrentUnits->Visible = false;
   ConcurrentEdit->Visible = false;
   ConcurrentLabel->Visible = false;
   NamedUnits->Visible = false;
   NamedEdit->Visible = false;
   NamedLabel->Visible = false;
   AppInstanceEdit->Enabled = false;
   AppInstanceUnits->Enabled = false;
}
                                                                   
//==============================================================================
// Function:    ProductComboBoxChange
// Purpose:     Each time the product is changed update ....
// Parameters:  TObject* - Sender
// Returns:     None
//==============================================================================
void __fastcall TKeyWizardFrm::ProductComboBoxChange(TObject *Sender)
{
   //get select... if select has not changed do nothing
   int units(OutputEdit->Text.ToInt());
   int product(getSelectedProduct());
   if (product == m_selectedProduct)
      return;

   // new product select reset all fields... including keyType
   m_selectedProduct = product;
   m_bModules = false;

   ProtectionKey *pProtectionKey;
   key_record->pkey->productId = m_selectedProduct;
   pProtectionKey = ProtectionKey::newKey(key_record->pkey);
   memset(reinterpret_cast<void *>(&(pProtectionKey->keyDataBlock)), 0 , 100);

   delete key_record->pkey;
   key_record->pkey = pProtectionKey;

   resetKeyWizardFrm();
   //based on the product set the avaible keyType
   if      (m_selectedProduct == SPD_PRODUCT       ||
            m_selectedProduct == CONNECT_PRODUCT   ||
            m_selectedProduct == QUANTUM_PRODUCT   ||
            m_selectedProduct == ICONVERT_PRODUCT  ||
            m_selectedProduct == SPDE_PRODUCT      ) {

      WizardModuleFrame->initialize(MODE_3, m_selectedProduct);
      key_type_flag = ktfSPD;
      m_bModules    = true;
      SelectModules->Visible = true;
      OutputUnits->Visible = true;
      OutputEdit->Visible = true;
      OutputLabel->Visible = true;

      if(m_selectedProduct == SPDE_PRODUCT)
      {
            ConcurrentUnits->Visible = true;
            ConcurrentEdit->Visible = true;
            ConcurrentLabel->Visible = true;
            NamedUnits->Visible = true;
            NamedEdit->Visible = true;
            NamedLabel->Visible = true;
      }

      if (m_selectedProduct == ICONVERT_PRODUCT)
         OutputLabel->Caption = "Max Number of LUs";
      else {
         if (units == 4095)
            OutputLabel->Caption = "Unlimited Pool";
         else if (units < 1)
            OutputLabel->Caption = "No Pool";
         else
            OutputLabel->Caption = "Limited Pool";
      }

      if (m_selectedProduct == 5) {
         SPD_LEGACY = true;
      }
      else {
         OutputUnits->Enabled = true;
         OutputEdit->Enabled = true;
         SPD_LEGACY = false;
      }
   }
   else if (m_selectedProduct == SOLSCRIPT_PRODUCT ||
            m_selectedProduct == SDX_DESIGNER_PRODUCT ||
            m_selectedProduct == SOLFUSION_PRODUCT  ||
            m_selectedProduct == RUBIKA_PRODUCT) {
      WizardModuleFrame->initialize(MODE_3, m_selectedProduct);
      key_type_flag = ktfSPD;
      m_bModules    = true;
      SelectModules->Visible = true;
      //has modules but no concept of outputs
      OutputUnits->Visible = false;
      OutputEdit->Visible = false;
      OutputLabel->Visible = false;
      SPD_LEGACY = false;
      //shares moduleDetail frame... disable members that do not apply
   }
   else if (m_selectedProduct == SOLSEARCHER_ENTERPRISE_PRODUCT) {
      WizardModuleFrame->initialize(MODE_3, m_selectedProduct);
      TSolSearcherDetails1->Initialize(MODE_3);
      key_type_flag = ktfSPD;
      m_bModules    = true;
      SelectModules->Visible = true;
      SPD_LEGACY = false;
   }
   else { //all other products
      key_type_flag = ktfProductWithoutModules;
      m_bModules    = false;
      SelectModules->Visible = false;
   }

   //check if key is already associated, if so allow only associated keytypes to be selected
   if(!EDIT_SYSTEMID)
      key_type_flag & ~ktfStandAlone;

   //apply customer number constrants
   if( key_record->pkey->customerNumber != 0x516 )
      key_type_flag = key_type_flag & ~ktfEmergency;
   if( key_record->pkey->customerNumber != 0x108 )
      key_type_flag = key_type_flag & ~ktfInventory;

   updateKeyTypeLookup();
}

//==============================================================================
// Function:    updateKeyTypeLookup()
// Purpose:
// Parameters:  None
// Returns:     None
//==============================================================================
void TKeyWizardFrm::updateKeyTypeLookup()
{
   //save current lookup value
   int tmp_index;
   AnsiString selected_string;

   if( keyTypeComboBox->ItemIndex != -1 )
      selected_string = keyTypeComboBox->Items->Strings[keyTypeComboBox->ItemIndex];

   //
   //ClearTypeComboBox
   keyTypeComboBox->Clear();
   //
   //add available selections
   if( key_type_flag & ktfAddon )
      keyTypeComboBox->Items->Add("Add-On");
   if( key_type_flag & ktfBase )
      keyTypeComboBox->Items->Add("Base");
   if( key_type_flag & ktfReplacement )
      keyTypeComboBox->Items->Add("Replacement");
   if( key_type_flag & ktfRelicense )
      keyTypeComboBox->Items->Add("Relicense");
   if( key_type_flag & ktfInventory )
      keyTypeComboBox->Items->Add("Inventory");
   if( key_type_flag & ktfBackup )
      keyTypeComboBox->Items->Add("Backup");
   if( key_type_flag & ktfEmergency )
      keyTypeComboBox->Items->Add("Emergency");
   if( key_type_flag & ktfCustom )
      keyTypeComboBox->Items->Add("Custom");
   if( key_type_flag & ktfLoan )
      keyTypeComboBox->Items->Add("Loan");
   if( key_type_flag & ktfDemo )
      keyTypeComboBox->Items->Add("Demo");
   if( key_type_flag & ktfRental )
      keyTypeComboBox->Items->Add("Rental");
   if( key_type_flag & ktfDevelopment )
      keyTypeComboBox->Items->Add("Test/Dev");

   //
   //check if previously selected item is still available, if it is not set to Base
   /* commenting out -- new wizard set base everytime product reset
   int new_index = getKeyTypeComboBoxIndex(keyTypeComboBox->Items, selected_string);
   if( -1 != new_index )
      keyTypeComboBox->ItemIndex = new_index;
   else */
      keyTypeComboBox->ItemIndex = getKeyTypeComboBoxIndex(keyTypeComboBox->Items,AnsiString("Base"));

   //
   //trigger OnChangeEvent
   keyTypeComboBoxChange(this);
}

//==============================================================================
// Function:    updateDays()
// Purpose:     Updates the days for selected values.
// Parameters:  None
// Returns:     None
//==============================================================================
void TKeyWizardFrm::updateDays()
{
   AnsiString selected_text(keyTypeComboBox->Items->Strings[keyTypeComboBox->ItemIndex]);
   DaysLabel->Caption = "Days";

   //
   // Inventory & Demo Keys get MAX_DAYS
   if( selected_text == "Inventory" || selected_text == "Demo" )
   {
         DayUpDown->Position = MAX_DAYS;
   }
   // Backup & Emergency Keys get EXPIRED_DAYS
   else if( selected_text == "Backup" || selected_text == "Emergency" )
   {
         DayUpDown->Position = EXPIRED_DAYS;
   }
   // Test/Dev Keys do not use this value, set to 0
   else if( selected_text == "Test/Dev" )
   {
         ui_days->Enabled = false;
         ui_days->Text = "0";
         DayUpDown->Enabled = false;
   }
   else
   {
         ui_days->Enabled = true;
         DayUpDown->Enabled = true;
         ui_days->Text = edited_days_value;
         DayUpDown->Position = edited_days_value;//DEFAULT_DAYS;
   }
}

//==============================================================================
// Function:    setDefaults()
// Purpose:
// Parameters:  None
// Returns:     None
//==============================================================================
void TKeyWizardFrm::setDefaults()
{
   //
   //Programmed Days
   edited_days_value = DEFAULT_DAYS;
   edited_days_value = DEFAULT_DAYS*(HOURS_PER_DAY);
   edited_days = false;

   //
   //Version Number
   edited_version = false;
}

//==============================================================================
// Function:    getKeyTypeComboBoxIndex()
// Purpose:
// Parameters:  None
// Returns:     None
//==============================================================================
int TKeyWizardFrm::getKeyTypeComboBoxIndex(Classes::TStrings* combobox, AnsiString search_string)
{
   int index(-1);
   for(int i=0; i<combobox->Count; i++)
   {
      if(combobox->Strings[i] == search_string)
      {
         index = i;
         break;
      }
   }
   return index;
}

//==============================================================================
// Function:    ui_daysChange()
// Purpose:
// Parameters:  TObject - *Sender
// Returns:     None
//==============================================================================
void __fastcall TKeyWizardFrm::ui_daysChange(TObject *Sender)
{
   int number_of_days;
   //if only '-' entered do not attempt conversion, treat as -1

   //check if ui_days is active if not, then control should not be processing input
   if(ui_days->Enabled == true)
   {
      if( ui_days->Text == '-' )
         number_of_days = 0;
      else
      {
         try
         {
            number_of_days = ui_days->Text.ToInt();
         }
         catch(...)
         {
            number_of_days = DayUpDown->Position;
         }
         //
         //set color
         if(number_of_days<0)
            ui_days->Font->Color = clRed;
         else
            ui_days->Font->Color = clWindowText;
      }

      edited_days_value = number_of_days;
      key_record->num_days = edited_days_value;
   }
}

//==============================================================================
// Function:    getSelectedVersion()
// Purpose:
// Parameters:  None
// Returns:     unsigned short
//==============================================================================
unsigned short TKeyWizardFrm::getSelectedVersion()
{
   AnsiString dbVersion = DBFormatVersion(ui_version->Text);
   return static_cast<unsigned short>(strtol(dbVersion.c_str(), &endptr, 16));
}

//==============================================================================
// Function:    getOutputUnits()
// Purpose:     depends on productId for the current protection key being set.
// Parameters:  None
// Returns:     unsigned short
//==============================================================================
unsigned short TKeyWizardFrm::getOutput()
{
   if(hasModuleLicenses())
   	return static_cast<unsigned short>(OutputUnits->Position);
   else
   	return 0;
}


//==============================================================================
// Function:    BackBtnClick()
// Purpose:     User is moving back through the wizard
// Parameters:  TObject - *Sender
// Returns:     None
//==============================================================================
void __fastcall TKeyWizardFrm::BackBtnClick(TObject *Sender)
{
   //steping back - previous page's controls should have already been validated,
   //so do not validate and enable NEXT button.
   NextBtn->Enabled = true;
   //hide current panel before going back to previous panel
   active_panel->Visible = false;

   //perform actions before state change
   int current_state = state.top();
   switch(current_state)
   {
      case wsSSESelection :
      case wsModuleSelection :
         break;
      case wsFinal :
         break;
      default:
         break;
   }

   //
   // perform actions after state change
   // pop current state... now state is equal to previous state
   state.pop();
   current_state = state.top();
   switch( current_state )
   {
      case wsStart:
         //buttons are set based on the state of selection controls
         BackBtn->Enabled = false;
         ProductComboBoxChange(this);
         active_panel = StartPanel;
         break;
      case wsSSESelection :
         active_panel = SolSearcherEnterprisePanel;
         break;
      case wsModuleSelection :
         active_panel = ModuleProgramPanel;
         break;
      //case wsKeySelection :
      //   ProgramBtn->Enabled = false;
      //   active_panel = KeySelectPanel;
      //   break;
      default:
         break;
   }

   active_panel->Visible = true;
}

//==============================================================================
// Function:    getSystemId()
// Purpose:
// Parameters:  None
// Returns:     int
//==============================================================================
int TKeyWizardFrm::getSystemId()
{
   int id(0);
   try
   {
      if(SystemEdit->Text!="GENERATED")
         id = SystemEdit->Text.ToInt();
   }
   catch(...){}//no special processing needed, returns default value

   return id;
}

//==============================================================================
// Function:    getSummary()
// Purpose:     Returns whatever values are in the summary or description field
//              for the key.
// Parameters:  None
// Returns:     String
//==============================================================================
String TKeyWizardFrm::getSummary()
{
   String summary(NULL);
   try
   {
       summary = KeySummary->Text;
   }
   catch(...){} // no special processing needed, returns default value

   return summary;
}


//==============================================================================
// Function:    geteBONumber()
// Purpose:     Returns the eBO number for the product.  This number is used for
//              Clientele and the order tracking for accounting.
// Parameters:  None
// Returns:     AnsiString
//==============================================================================
AnsiString TKeyWizardFrm::geteBONumber()
{
   AnsiString ebonum(0);
   try
   {
      ebonum = eBONumberEditBox->Text;
   }
   catch(...){} // no special processing needed, returns default value

   return ebonum;
}

//==============================================================================
// Function:    setStartPanelMembers()
// Purpose:
// Parameters:  None
// Returns:     None
//==============================================================================
void TKeyWizardFrm::setStartPanelMembers()
{
   unsigned short days;
   AnsiString summary;

   try{days = static_cast<unsigned short>(ui_days->Text.ToInt());}
   catch(...){/*unable to set protection key cells *ERROR OUT OF FUNCTION* */}

   KeyType keyType(getKeyType());
   //
   // SKeyRecord Members - database only
   key_record->system_id = getSystemId();
   key_record->eBOnumber = geteBONumber();//ebo number
   key_record->non_perm_ktf = static_cast<bool>(keyType&ktfAddon || keyType&ktfBackup || keyType&ktfDevelopment);

   //
   // ProtecitonKey Members -
   // key->customerNumbeizr should be set on keyWizard initialization
   // key->keyNumber should be set on keyWizard initialation
   ProtectionKey *pKey(key_record->pkey);
   pKey->keyType = key_record->getKeyTypeEnumValue(keyType);
   pKey->productId = getSelectedProduct();
   pKey->productVersion = getSelectedVersion();
   pKey->status = 0;             // all keys are programmed as trial keys
   pKey->primaryDescriptor   = PRIMARY_DESCRIPTOR;
   pKey->primaryPassword     = PRIMARY_PASSWORD;
   pKey->secondaryDescriptor = SECONDARY_DESCRIPTOR;

   //
   // Test/Development keys are different than the normal keys in that they have
   // set days/hours values without a grace period. We set these values, in
   // addition, we set the initial value in the summary so we'll know in the
   // future what the key was programmed for.
   if (pKey->keyType == KEYDevelopment)
   {
      //
      // Test/Development keys get preset values
      summary = key_record->summary;
      sprintf ( scratch, "%s  -  %s", summary, getSummary() );
      key_record->summary = scratch;
      pKey->setExpirationDays(edited_days_value);
      pKey->setCounterDaysByHours(edited_hours_value);
   }
   else
   {
      //
      // All other keys get user defined values
      key_record->summary = getSummary();    //KeySummary->Text;
      pKey->setExpirationDays(days);
      pKey->setCounterDays(days);
   }
}

//==============================================================================
// Function:    setModulePanelMembers()
// Purpose:
// Parameters:  None
// Returns:     None
//==============================================================================
void TKeyWizardFrm::setModulePanelMembers()
{
   //moduleCells are update to key as module grid changes
   //update all other GUI components to key

   //Set Application Instances for key
   key_record->setAppInstances(static_cast<unsigned short>(AppInstanceEdit->Text.ToInt()));

   if(key_record->pkey->productId == SPD_PRODUCT ||
           key_record->pkey->productId == ICONVERT_PRODUCT ||
           key_record->pkey->productId == SPDE_PRODUCT)
        ((SpdProtectionKey*)(key_record->pkey))->setOutputUnits(static_cast<unsigned short>(OutputEdit->Text.ToInt()));
   if(key_record->pkey->productId == SPDE_PRODUCT)
   {
        ((SpdeProtectionKey*)(key_record->pkey))->setOperatorSessionUnits(static_cast<unsigned short>(ConcurrentEdit->Text.ToInt()));
        ((SpdeProtectionKey*)(key_record->pkey))->setUserSessionUnits(static_cast<unsigned short>(NamedEdit->Text.ToInt()));
   }
}

//==============================================================================
// Fucntion:    NextBtnClick()
// Purpose:     User is moving forward through the wizard
// Parameters:  TObject - *Sender
// Returns:     None
//==============================================================================
void __fastcall TKeyWizardFrm::NextBtnClick(TObject *Sender)
{
   //User has clicked the next button
   KeyType keyTypeSelected;

   //switch on current state of wizard
   switch( state.top() )
   {
      //StartPanel
      case wsStart:
         //
         //set start panel to invisible
         active_panel->Visible = false;
         BackBtn->Enabled = true;

         //check to see if it is test/dev
         if(passwordComboBox->Enabled)
         {
            key_record->num_days = passwordComboBox->ItemIndex;
         }
         //set physical key members
         setStartPanelMembers();

         //set application instances for key
         //if remote key checked then initialize to 1, else 0
         if(RemoteKey_CHB->Checked)
         {
                AppInstanceEdit->Enabled = true;
                AppInstanceUnits->Enabled = true;
                AppInstanceUnits->Position = 1;
         }
         else
         {
                AppInstanceEdit->Enabled = false;
                AppInstanceUnits->Enabled = false;
                AppInstanceUnits->Position = 0;
         }
         //if ktfInventory - no parent key, no orders added - goto FinalPanel
         //else if ktfType needs parent and parent not set - goto KeySelectionPanel
         //else if no parent or parent already set but need orders - goto OrderPanel
         keyTypeSelected = getKeyType();
         if      (m_selectedProduct == SPD_PRODUCT       ||
                  m_selectedProduct == CONNECT_PRODUCT   ||
                  m_selectedProduct == QUANTUM_PRODUCT   ||
                  m_selectedProduct == ICONVERT_PRODUCT  ||
                  m_selectedProduct == SPDE_PRODUCT       ) {
            //Select Modules
            state.push( wsModuleSelection );
            active_panel = ModuleProgramPanel;

            ///if(MODULE_STATE_CHANGE) {
               WizardModuleFrame->initialize(MODE_3, m_selectedProduct);
               WizardModuleFrame->load(key_record, true);
               OutputUnits->Position = ((SpdProtectionKey*)(key_record->pkey))->getOutputUnits();
               if(m_selectedProduct == SPDE_PRODUCT)
               {
                   ConcurrentUnits->Position = ((SpdeProtectionKey*)(key_record->pkey))->getOperatorSessionUnits();
                   NamedUnits->Position = ((SpdeProtectionKey*)(key_record->pkey))->getUserSessionUnits();
               }
            ///}

            //enable NextBtn - user is not required to make any chages
            NextBtn->Enabled = true;
            active_panel->BringToFront();
            active_panel->Visible = true;
         }
         else if (m_selectedProduct == SOLSCRIPT_PRODUCT ||
                  m_selectedProduct == RUBIKA_PRODUCT  ||
                  m_selectedProduct == SOLFUSION_PRODUCT ||
                  m_selectedProduct == SDX_DESIGNER_PRODUCT ) {
            //Select Modules
            state.push( wsModuleSelection );
            active_panel = ModuleProgramPanel;

            ///if(MODULE_STATE_CHANGE) {
               WizardModuleFrame->initialize(MODE_3, m_selectedProduct);
               WizardModuleFrame->load(key_record, true);
               OutputUnits->Position = ((SpdProtectionKey*)(key_record->pkey))->getOutputUnits();
            ///}

            //enable NextBtn - user is not required to make any chages
            NextBtn->Enabled = true;
            active_panel->BringToFront();
            active_panel->Visible = true;
         }
         else if (m_selectedProduct == SOLSEARCHER_ENTERPRISE_PRODUCT) {
            //Select Units
            state.push( wsSSESelection );
            active_panel = SolSearcherEnterprisePanel;

            WizardModuleFrame->initialize(MODE_3, m_selectedProduct);
            WizardModuleFrame->load(key_record, true);
            TSolSearcherDetails1->Initialize(MODE_3);
            TSolSearcherDetails1->Load(key_record);

            //enable NextBtn - user is not required to make any chages
            NextBtn->Enabled = true;
            active_panel->BringToFront();
            active_panel->Visible = true;

         }
         else { //all other products
            state.push( wsFinal );
            NextBtn->Enabled = false;
            ProgramBtn->Enabled = true;
            active_panel = FinalPanel;
            active_panel->BringToFront();
            active_panel->Visible = true;
         }
         break;
      //OnClick Next - ModulePanel
      case wsModuleSelection:
      //case wsOrderSelection :

         active_panel->Visible = false;
         //set key to reflect changes made on Module Selection Panel
         setModulePanelMembers();

         state.push( wsFinal );
         NextBtn->Enabled = false;
         ProgramBtn->Enabled = true;
         active_panel = FinalPanel;
         active_panel->BringToFront();
         active_panel->Visible = true;

         break;
      case wsSSESelection:
         active_panel->Visible = false;

         //set key to reflect changes made on Module Selection Panel
         SetSSEMembers();

         state.push( wsModuleSelection );
         NextBtn->Enabled = true;
         ProgramBtn->Enabled = false;
         active_panel = ModuleProgramPanel;
         active_panel->BringToFront();
         active_panel->Visible = true;

         break;
      //OnClick Next - FinalPanel
      case wsFinal:
      default:             
         break;
   }
}

//==============================================================================
// Function:    getKeyType()
// Purpose:     Gets the item index from the keyTypeComboBox and returns the
//              key type associated with that index.
// Parameters:  None
// Returns:     KeyType
//==============================================================================
KeyType TKeyWizardFrm::getKeyType()
{
   //
   // Initialize variables
   KeyType selected(static_cast<KeyType>(0));//nothing selected
   AnsiString selected_string(keyTypeComboBox->Items->Strings[keyTypeComboBox->ItemIndex]);

   //
   // Check selected_string vs. selected
   if(selected_string == "Add-On")
      selected = static_cast<KeyType>(ktfAddon);
   else if( selected_string == "Base")
      selected = static_cast<KeyType>(ktfBase);
   else if( selected_string == "Replacement")
      selected = static_cast<KeyType>(ktfReplacement);
   else if( selected_string == "Relicense")
      selected = static_cast<KeyType>(ktfRelicense);
   else if( selected_string == "Inventory")
      selected = static_cast<KeyType>(ktfInventory);
   else if( selected_string == "Backup")
      selected = static_cast<KeyType>(ktfBackup);
   else if( selected_string == "Emergency")
      selected = static_cast<KeyType>(ktfEmergency);
   else if( selected_string == "Custom")
      selected = static_cast<KeyType>(ktfCustom);
   else if( selected_string == "Loan")
      selected = static_cast<KeyType>(ktfLoan);
   else if( selected_string == "Demo")
      selected = static_cast<KeyType>(ktfDemo);
   else if ( selected_string == "Test/Dev")
      selected = static_cast<KeyType>(ktfDevelopment);
   else if(selected_string == "Rental")
      selected = static_cast<KeyType>(ktfRental);

   return (selected);
}


//==============================================================================
// Function:    ProgramBtnClick
// Purpose:     The program button can be clicked during different stages of the
//              wizard.
// Parameters:  (TObject *Sender)
// Returns:     None
//==============================================================================
void __fastcall TKeyWizardFrm::ProgramBtnClick(TObject *Sender)
{
   //
   //perform actions before state change
   int current_state = state.top();

   switch(current_state)
   {
      case wsStart:
         setStartPanelMembers();
         //
         //set key module cells and outputUnits cell
         //keyMaster->initializeMaxModules(key_record);
         ModalResult = mrOk;
         break;
      case wsModuleSelection:
         //setStartPanelMembers has already been called
         setModulePanelMembers();
         ModalResult = mrOk;
         break;
      case wsSSESelection:
         SetSSEMembers();
         ModalResult = mrOk;
         break;
      case wsFinal:
         ModalResult = mrOk;
         break;
      default:
         break;
   }
}

//==============================================================================
// Function:    OutputEditChange()
// Purpose:     Returns the status of the pools based on the number in the
//              edit box.
// Parameters:  TObject *Sender
// Returns:     None
//==============================================================================
void __fastcall TKeyWizardFrm::OutputEditChange(TObject *Sender)
{

   try
   {
      int units(OutputEdit->Text.ToInt());
      int product_index = ProductComboBox->ItemIndex;

      if (product_index == 6)  // not iCONVERT
      {
         OutputLabel->Caption = "Max Number of LUs";
      }
      else
      {
         if(units == 4095)
             OutputLabel->Caption = "Unlimited Pool";
         else if(units < 1)
             OutputLabel->Caption = "No Pool";
         else
             OutputLabel->Caption = "Limited Pool";
      }
   }
   catch(...)
   {
      OutputEdit->Text = OutputUnits->Position;
   }
}

//==============================================================================
// Function:    SelectModulesClick()
// Purpose:     Event Handler for the SelectModules Event.
// Parameters:  TObject - *Sender
// Returns:     None
//==============================================================================
void __fastcall TKeyWizardFrm::SelectModulesClick(TObject *Sender)
{
    SpdProtectionKey* pSpdKey = static_cast<SpdProtectionKey*>(key_record->pkey);
    pSpdKey->setLicensesToZero();
    if (SelectModules->Checked) {
         //initializeMinModules uses productId and productVersion
         key_record->pkey->productId = getSelectedProduct();
         key_record->pkey->productVersion = getSelectedVersion();
         keyMaster->initializeMinModules(key_record);
    }
    else {
         //initialize uses productId and productVersion
         key_record->pkey->productId = getSelectedProduct();
         key_record->pkey->productVersion = getSelectedVersion();
         keyMaster->initializeMaxModules(key_record);
    }
}

//==============================================================================
// Function:    keyTypeComboBoxChange
// Purpose:
// Parameters:  TObject - *Sender
// Returns:     None
//==============================================================================
void __fastcall TKeyWizardFrm::keyTypeComboBoxChange(TObject *Sender)
{
   //global setting based on product should be set in ProductComboBoxChange
   int moduleState(msNone);
   unsigned short version;
   int majorVersion, minorVersion;
   KeyType keyType(getKeyType());
   ProgramBtn->Enabled = false;
   //reset wizard components that are change on key_type
   SelectModules->Checked = false;
   SelectModules->Enabled = false;

   //set controls based on product - product version
   ui_version->Enabled = !SPD_LEGACY;
   version = lookup->getCurrentVersion(m_selectedProduct, SPD_LEGACY);
   majorVersion = ((int)version) >> 12;
   //extract the last three bytes
   minorVersion = (int)((((version & 0x0F00) >> 8) * 10) + (((version & 0x00F0) >> 4) * 1));
   ui_version->Text = AnsiString::Format("%d.%d", OPENARRAY(TVarRec,(majorVersion,minorVersion)));
   //Format("%4X", OPENARRAY(TVarRec,(lookup->getCurrentVersion(m_selectedProduct, SPD_LEGACY))));

   /* Addon Key- purpose is to extend the functionality of an existing key.  Note:
    * If existing key is non-permanent (can not be made permanent) then and Add-on
    * is not needed.  The key in question should be given non-permanent upgrade
    * passwords.
    *-------------------------------------------------------------------------*/
   if      (keyType == ktfAddon) {
      //set items specific to Addon
      SystemEdit->Enabled = true;
      SystemEdit->Visible = true;
      SystemEdit->Text = edited_system_id;
      m_bNonPermanentKey = true;

      if (m_bModules) {
         moduleState = msMin;             // starts in minimized state
         SelectModules->Checked = true;   // able to change state
      }
   }
   else if (keyType == ktfRelicense) {
      // For module products only.
      SystemEdit->Enabled = true;
      SystemEdit->Visible = true;
      SystemEdit->Text = edited_system_id;
      m_bNonPermanentKey = false;
      showPasswordComboBox(false); //test/dev key time selection

      if (m_bModules) {
         moduleState = msMin;
         SelectModules->Checked = true;
         SelectModules->Enabled = true;
      }
   }
   else if (keyType == ktfBackup) {
      //backup is it's own system
      SystemEdit->Visible = true;
      SystemEdit->Enabled = false;
      SystemEdit->Text = "GENERATED";
      m_bNonPermanentKey = true;
      showPasswordComboBox(false);

      //override default product version
      if (!SPD_LEGACY) {
         ui_version->Enabled = false;
         ui_version->Text = "9.999";
      }

      if (m_bModules) {
         SelectModules->Checked = true;
         moduleState = msMin;
      }
      else { // XImange, SolSearcher, etc.
         moduleState = msClear;
      }
   }
   else if (keyType == ktfRental) {
      SystemEdit->Visible = true;
      SystemEdit->Enabled = false;
      SystemEdit->Text = "GENERATED";
      m_bNonPermanentKey = true;
      showPasswordComboBox(false);

      if (m_bModules) {
         SelectModules->Checked = true;
         SelectModules->Enabled = true;
         moduleState = msMin;
      }
      else { // XImange, SolSearcher, etc.
         moduleState = msClear;
      }
   }
   else if(keyType == ktfReplacement) {
      SystemEdit->Visible = true;
      SystemEdit->Enabled = true;
      SystemEdit->Text = edited_system_id;
      m_bNonPermanentKey = false;

      showPasswordComboBox(false);

      if (m_bModules) {
         //allow user to choice between programming modules specifically or setting to trial
         SelectModules->Enabled = true;
         SelectModulesClick(this);
      }
      else { // XImange, SolSearcher, etc.
         moduleState = msClear;
      }
   }
   else if (keyType == ktfInventory) {
      SystemEdit->Visible = true;
      SystemEdit->Enabled = false;
      SystemEdit->Text = "GENERATED";
      m_bNonPermanentKey = false;

      showPasswordComboBox(false);

      if (m_bModules) {
         moduleState = msMax;
         //have all the information needed to program!
         ProgramBtn->Enabled = true;
      }
      else {// XImange, SolSearcher, etc.
         moduleState = msClear;
      }
   }
      //supports all product types
   else if (keyType == ktfEmergency) {
      SystemEdit->Visible = true;
      SystemEdit->Enabled = false;
      SystemEdit->Text = "GENERATED";
      m_bNonPermanentKey = false;

      showPasswordComboBox(false);
      if (m_bModules) {
         moduleState = msMax;
      }
      else {// XImange, SolSearcher, etc.
         moduleState = msClear;
      }
   }
   else if (keyType == ktfCustom) {
         SystemEdit->Visible = true;
         SystemEdit->Enabled = false;
         SystemEdit->Text = "GENERATED";

         m_bNonPermanentKey = false;

         showPasswordComboBox(false);

         if (m_bModules) {
            SelectModules->Checked = true;
            moduleState = msMin;
            //Enable Wizard Buttons
            ProgramBtn->Enabled = true;
         }
         else { // XImange, SolSearcher, etc.
            moduleState = msClear;
         }
   }
   else if (keyType == ktfLoan) {
      SystemEdit->Visible = true;
      SystemEdit->Enabled = false;
      SystemEdit->Text = "GENERATED";
      m_bNonPermanentKey = true;

      showPasswordComboBox(false);
      if (m_bModules) {
         moduleState = msMin;
         SelectModules->Checked = true;
         SelectModules->Enabled = true;
      }
      else {// XImage, SolSearcher, etc.
            moduleState = msClear;
      }
   }
   else if( keyType == ktfDemo) {
      SystemEdit->Visible = true;
      SystemEdit->Enabled = false;
      SystemEdit->Text = "GENERATED";
      m_bNonPermanentKey = false;

      showPasswordComboBox(false);
      if (m_bModules) {
         moduleState = msMax;
      }
      else // XImange, SolSearcher, etc.
      {
         moduleState = msClear;
      }
   }
   else if (keyType == ktfBase) {
      SystemEdit->Visible = true;
      SystemEdit->Enabled = false;
      SystemEdit->Text = "GENERATED";
      m_bNonPermanentKey = false;

      showPasswordComboBox(false);
      if (m_bModules) {
//         moduleState = msMax;
         SelectModules->Enabled = true;
         SelectModulesClick(this);
      }
      else {// XImange, SolSearcher, etc.
         moduleState = msClear;
      }
   }
   else if (keyType == ktfDevelopment) {
      SystemEdit->Visible = true;
      SystemEdit->Enabled = false;
      SystemEdit->Text = "GENERATED";
      m_bNonPermanentKey = true;

      showPasswordComboBox(true);
      key_record->summary = key_record->pkey->getKDPasswordText(passwordComboBox->ItemIndex);
      edited_days_value = key_record->pkey->getKDPasswordDays(passwordComboBox->ItemIndex);
      edited_hours_value = key_record->pkey->getKDPasswordHours(passwordComboBox->ItemIndex);
      if (m_bModules) {
         SelectModules->Checked = true;
         SelectModules->Enabled = true;
         moduleState = msMin;
      }
      else {//XImange, SolSearcher, etc.
         moduleState = msClear;
      }
   }

   // update the key type flag pertaining to: Addon, Test/Dev, & Backup keys
   //key_record->non_perm_ktf = (keyTypeCheckBox->Checked == true) ? true : false;

   if (moduleState == msMax) {
         //initialize uses productId and productVersion
       key_record->pkey->productId = getSelectedProduct();
       key_record->pkey->productVersion = getSelectedVersion();
       keyMaster->initializeMaxModules(key_record);
   }
   else if (moduleState == msMin) {
         //initializeMinModules uses productId and productVersion
       key_record->pkey->productId = getSelectedProduct();
       key_record->pkey->productVersion = getSelectedVersion();
       keyMaster->initializeMinModules(key_record);
   }

   if (m_selectedProduct == SOLSEARCHER_ENTERPRISE_PRODUCT) {
      if (moduleState == msClear) {
         SSProtectionKey* pSSKey = static_cast<SSProtectionKey*>(key_record->pkey);
         pSSKey->setLicensesToZero();
      }
   }
   else if (m_selectedProduct == SPD_PRODUCT       ||
            m_selectedProduct == CONNECT_PRODUCT   ||
            m_selectedProduct == QUANTUM_PRODUCT   ||
            m_selectedProduct == ICONVERT_PRODUCT  ||
            m_selectedProduct == SOLSCRIPT_PRODUCT ||
            m_selectedProduct == SDX_DESIGNER_PRODUCT ||
            m_selectedProduct == SOLFUSION_PRODUCT ||
            m_selectedProduct == RUBIKA_PRODUCT) {
      if (moduleState == msClear) {
         SpdProtectionKey* spd_key = static_cast<SpdProtectionKey*>(key_record->pkey);
         spd_key->setOutputUnits(0);
         spd_key->setLicensesToZero();
      }
   }
   else if(m_selectedProduct == SPDE_PRODUCT) {       //case for SPDE product
      if (moduleState == msClear) {
         SpdeProtectionKey* spde_key = static_cast<SpdeProtectionKey*>(key_record->pkey);
         spde_key->setOutputUnits(0);
         spde_key->setOperatorSessionUnits(0);
         spde_key->setUserSessionUnits(0);
         spde_key->setLicensesToZero();
      }
   }
   else {}

   updateDays();
}


//==============================================================================
// Function:    showPasswordComboBox
// Purpose:     Shows or turns off the password combo box used for the Test/Dev keys
//              If the passwordComboBox is off, then the Days are on
//              It's always the opposite.
// Parameters:  TObject - *Sender
// Returns:     None
//==============================================================================
void TKeyWizardFrm::showPasswordComboBox(bool bTurnOn)
{
   //
   // Used for the Test/Dev Keys
   passwordComboBox->Visible = bTurnOn;
   passwordComboBox->Enabled = bTurnOn;
   passwordComboBoxLabel->Visible = bTurnOn;

   //
   // Used for ALL OTHER Keys
   DaysLabel->Visible = !bTurnOn;
   ui_days->Visible = !bTurnOn;
   DayUpDown->Visible = !bTurnOn;


}


//==============================================================================
// Function:    SystemEditChange
// Purpose:
// Parameters:  TObject - *Sender
// Returns:     None
//==============================================================================
void __fastcall TKeyWizardFrm::SystemEditChange(TObject *Sender)
{
   try
   {
      if(SystemEdit->Text != "GENERATED")
         edited_system_id = SystemEdit->Text.ToInt();
      //else do not set systemEdit
   }
   catch(...)
   {
      SystemEdit->Text = edited_system_id;
   }
}

//==============================================================================
// Function:     eBONumberEditBoxChange
// Purpose:      To update eBOnumber when the user enters the number value into
//               this parameter
// Parameters:   TObject - *Sender
// Returns:      None
//==============================================================================
void __fastcall TKeyWizardFrm::eBONumberEditBoxChange(TObject *Sender)
{
   eBONumber = eBONumberEditBox->Text;
}


//==============================================================================
// Function:     keyClassComboBoxChange
// Purpose:      This function will let the user select which type of key he/she
//               is programming. This is for internal use only.
// Parameters:   TObject - *Sender
// Returns:      None
//==============================================================================
void __fastcall TKeyWizardFrm::keyClassComboBoxChange(TObject *Sender)
{
   int class_index(keyClassComboBox->ItemIndex);
   key_record->key_class = (class_index == 0) ? PARALLEL_KEY : USB_KEY;
}

//==============================================================================
// Function:     passwordComboBoxChange
// Purpose:      This function updates the days & hours counter.
// Parameters:   TObject - *Sender
// Returns:      None
//==============================================================================
void __fastcall TKeyWizardFrm::passwordComboBoxChange(TObject *Sender)
{
   int password_index(passwordComboBox->ItemIndex);

   //
   // check if passwordComboBox is active if not, then control should not be pro
   if(passwordComboBox->Enabled == true)
   {
       edited_days_value = key_record->pkey->getKDPasswordDays(password_index);
       edited_hours_value = key_record->pkey->getKDPasswordHours(password_index);
       key_record->summary = key_record->pkey->getKDPasswordText(password_index);
   }
}

/*
 *----------------------------------------------------------------------------*/
void __fastcall TKeyWizardFrm::ui_versionChange(TObject *Sender)
{
   /*if (ui_version->Text.Length()==4){
      key_record->pkey->productVersion = getSelectedVersion();
      if(m_bModules)
         SelectModulesClick(NULL);
   }
   */
}


void __fastcall TKeyWizardFrm::ui_versionExit(TObject *Sender)
{
   if(!IsValidVersionFormat(ui_version->Text))
   {
       Application->MessageBox("Please enter a valid version.", "Invalid Entry", MB_OK );
       ui_version->SelectAll();
       ui_version->SetFocus();
   }
   else
   {
       key_record->pkey->productVersion = getSelectedVersion();
       if(m_bModules)
         SelectModulesClick(NULL);
   }
}
//---------------------------------------------------------------------------


//Expect strVersion to be valid X.X+ , minorVersion max of 2
AnsiString TKeyWizardFrm::DBFormatVersion(AnsiString strVersion)
{
    AnsiString majorVersion, minorVersion;
    int pos;
    pos = strVersion.AnsiPos(".");
    majorVersion = AnsiString(strVersion.SubString(0,pos-1).ToInt());
    minorVersion = AnsiString(strVersion.SubString(pos+1, strVersion.Length()-(majorVersion.Length())).ToInt());

    if(minorVersion.Length() == 1)
        minorVersion = "0" + minorVersion;
    minorVersion= minorVersion + "0";
    return majorVersion + minorVersion;
}

bool TKeyWizardFrm::IsValidVersionFormat(AnsiString versionString)
{
    AnsiString majorVersion, minorVersion, tempString;
    int count = 0, pos = 0, number = 0;
    //find position of period
    tempString = versionString;
    do
    {
        pos = tempString.AnsiPos(".");
        tempString = tempString.SubString(pos+1, tempString.Length()-pos);
        if(pos != 0)
            count++;
    }
    while(pos != 0);
    if(count != 1)
        return false;
    else
    {   //only one period ...split into major, minor
        pos = versionString.AnsiPos(".");
        majorVersion = versionString.SubString(0,pos-1);
        if((majorVersion.Length() > 1) ||
           (StrToIntDef(majorVersion, -1) < 0) ||
           (majorVersion.ToInt() < 1))
            return false;
        minorVersion = versionString.SubString(pos+1, versionString.Length()-(majorVersion.Length()));
        if((minorVersion.Length() > 2) || (StrToIntDef(minorVersion, -1) < 0))
            return false;
    }
    return true;
}
