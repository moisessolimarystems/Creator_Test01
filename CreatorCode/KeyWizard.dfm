object KeyWizardFrm: TKeyWizardFrm
  Left = 407
  Top = 343
  Width = 796
  Height = 536
  BorderWidth = 5
  Caption = 'KeyWizard'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 14
  object StartPanel: TPanel
    Left = 173
    Top = 0
    Width = 605
    Height = 455
    Align = alClient
    BevelOuter = bvNone
    BorderStyle = bsSingle
    TabOrder = 0
    object Label5: TLabel
      Left = 9
      Top = 69
      Width = 64
      Height = 14
      Caption = 'Product Type'
    end
    object Label6: TLabel
      Left = 9
      Top = 112
      Width = 46
      Height = 14
      Caption = 'Key Type'
    end
    object Label1: TLabel
      Left = 170
      Top = 69
      Width = 38
      Height = 14
      Caption = 'Version'
    end
    object DaysLabel: TLabel
      Left = 170
      Top = 112
      Width = 25
      Height = 14
      Caption = 'Days'
    end
    object SystemEditLabel: TLabel
      Left = 9
      Top = 237
      Width = 47
      Height = 14
      Caption = 'System Id'
    end
    object Label7: TLabel
      Left = 9
      Top = 283
      Width = 45
      Height = 14
      Caption = 'Summary'
    end
    object eBONumberLabel: TLabel
      Left = 170
      Top = 233
      Width = 61
      Height = 14
      Caption = 'eBO Number'
    end
    object keyClassLabel: TLabel
      Left = 9
      Top = 17
      Width = 49
      Height = 14
      Caption = 'Key Class'
    end
    object passwordComboBoxLabel: TLabel
      Left = 170
      Top = 112
      Width = 107
      Height = 14
      Caption = 'Exp Days/Counter Hrs'
    end
    object ProductComboBox: TComboBox
      Left = 8
      Top = 84
      Width = 157
      Height = 22
      ItemHeight = 14
      TabOrder = 0
      OnChange = ProductComboBoxChange
      Items.Strings = (
        'Platinum'
        'XImage NT'
        'XImage 3.1/95'
        'SOLsearcher'
        'Connectivity'
        'Legacy'
        'iConvert'
        'SOLsearcher Enterprise'
        'SOLscript'
        'SDX Designer'
        'Quantum Server'
        'PDF Utility'
        'SP/D Enterprise')
    end
    object keyTypeComboBox: TComboBox
      Left = 8
      Top = 128
      Width = 157
      Height = 22
      ItemHeight = 14
      TabOrder = 1
      OnChange = keyTypeComboBoxChange
    end
    object ui_version: TMaskEdit
      Left = 179
      Top = 84
      Width = 45
      Height = 22
      EditMask = 'A.AAA;0;_'
      MaxLength = 5
      TabOrder = 2
      Text = '510B'
      OnChange = ui_versionChange
    end
    object ui_days: TMaskEdit
      Left = 179
      Top = 128
      Width = 46
      Height = 22
      TabOrder = 3
      Text = '30'
      OnChange = ui_daysChange
    end
    object DayUpDown: TUpDown
      Left = 225
      Top = 128
      Width = 17
      Height = 22
      Associate = ui_days
      Min = -10
      Max = 90
      Position = 30
      TabOrder = 4
      Wrap = False
    end
    object SelectModules: TCheckBox
      Left = 276
      Top = 67
      Width = 311
      Height = 18
      Caption = 'Specify modules to be programed.'
      TabOrder = 5
      Visible = False
      OnClick = SelectModulesClick
    end
    object SystemEdit: TEdit
      Left = 8
      Top = 252
      Width = 126
      Height = 22
      TabOrder = 6
      Text = 'SystemEdit'
      OnChange = SystemEditChange
    end
    object KeySummary: TMemo
      Left = 8
      Top = 306
      Width = 480
      Height = 66
      MaxLength = 254
      TabOrder = 7
    end
    object eBONumberEditBox: TEdit
      Left = 170
      Top = 250
      Width = 81
      Height = 22
      TabOrder = 8
      Text = '0'
      OnChange = eBONumberEditBoxChange
    end
    object keyClassComboBox: TComboBox
      Left = 9
      Top = 34
      Width = 156
      Height = 22
      ItemHeight = 14
      TabOrder = 9
      OnChange = keyClassComboBoxChange
      Items.Strings = (
        'Parallel'
        'USB')
    end
    object passwordComboBox: TComboBox
      Left = 179
      Top = 128
      Width = 182
      Height = 22
      DropDownCount = 25
      ItemHeight = 14
      TabOrder = 10
      OnChange = passwordComboBoxChange
    end
  end
  object FinalPanel: TPanel
    Left = 173
    Top = 0
    Width = 605
    Height = 455
    Align = alClient
    BevelOuter = bvNone
    BorderWidth = 5
    BorderStyle = bsSingle
    TabOrder = 3
    Visible = False
  end
  object SolSearcherEnterprisePanel: TPanel
    Left = 173
    Top = 0
    Width = 605
    Height = 455
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 5
    inline TSolSearcherDetails1: TSolSearcherDetails
      Left = 0
      Top = 26
      Width = 664
      Height = 467
      Color = clBackground
      ParentColor = False
      TabOrder = 0
      inherited DetailSQL: TQuery
        Left = 46
        Top = 235
      end
      inherited PropertyPopup: TPopupMenu
        Left = 18
        Top = 235
      end
    end
  end
  object ModuleProgramPanel: TPanel
    Left = 173
    Top = 0
    Width = 605
    Height = 455
    Align = alClient
    BorderWidth = 5
    BorderStyle = bsSingle
    TabOrder = 4
    Visible = False
    object OutputLabel: TLabel
      Left = 307
      Top = 366
      Width = 109
      Height = 14
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Unlimited Pool'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    inline WizardModuleFrame: TModuleFrame
      Left = 11
      Top = 19
      Width = 475
      Height = 331
      TabOrder = 0
      inherited ModuleList: TListView
        Width = 475
        Height = 331
        Align = alClient
      end
    end
    object OutputEdit: TMaskEdit
      Left = 431
      Top = 363
      Width = 39
      Height = 22
      TabOrder = 1
      Text = '4095'
      OnChange = OutputEditChange
    end
    object OutputUnits: TUpDown
      Left = 470
      Top = 363
      Width = 17
      Height = 22
      Associate = OutputEdit
      Min = 0
      Max = 4095
      Position = 4095
      TabOrder = 2
      Thousands = False
      Wrap = False
    end
  end
  object ButtonPanel: TPanel
    Left = 0
    Top = 455
    Width = 778
    Height = 44
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    DesignSize = (
      778
      44)
    object Bevel1: TBevel
      Left = 0
      Top = 0
      Width = 778
      Height = 10
      Align = alTop
      Shape = bsTopLine
    end
    object NextBtn: TButton
      Left = 512
      Top = 17
      Width = 80
      Height = 27
      Anchors = [akRight, akBottom]
      Caption = '&Next >'
      TabOrder = 0
      OnClick = NextBtnClick
    end
    object BackBtn: TButton
      Left = 431
      Top = 17
      Width = 81
      Height = 27
      Anchors = [akRight, akBottom]
      Caption = '< &Back'
      Enabled = False
      TabOrder = 1
      OnClick = BackBtnClick
    end
    object CancelBtn: TButton
      Left = 694
      Top = 17
      Width = 81
      Height = 27
      Anchors = [akRight, akBottom]
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 2
    end
    object ProgramBtn: TButton
      Left = 596
      Top = 17
      Width = 79
      Height = 27
      Anchors = [akRight, akBottom]
      Caption = '&Program'
      Enabled = False
      TabOrder = 3
      OnClick = ProgramBtnClick
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 173
    Height = 455
    Align = alLeft
    BevelOuter = bvLowered
    BorderWidth = 5
    BorderStyle = bsSingle
    Color = clAppWorkSpace
    TabOrder = 2
  end
  object Query1: TQuery
    DatabaseName = 'BackOffice'
    SQL.Strings = (
      
        'select l.order_no, l.order_ext, l.line_no, l.part_no As '#39'ordered' +
        ' part'#39', l.description, l.ordered, l.shipped, l.status, w.part_no' +
        ', w.fixed, w.qty, inv.description As '#39'subdescription'#39' from ord_l' +
        'ist As l INNER JOIN what_part As w INNER JOIN inventory As inv O' +
        'N w.part_no = inv.part_no ON ( l.part_no = w.asm_no ) where (w.p' +
        'art_no Like '#39'00 03 501%'#39' or w.asm_no = '#39'00 03 303 00 0000'#39') AND ' +
        'inv.location = '#39'STRM'#39' AND (')
    Left = 16
    Top = 96
  end
  object DataSource1: TDataSource
    DataSet = Query1
    Left = 48
    Top = 96
  end
  object backoffice: TDatabase
    DatabaseName = 'BackOffice'
    LoginPrompt = False
    Params.Strings = (
      'USERNAME=sa'
      'PASSWORD=sa')
    SessionName = 'Default'
    Left = 16
    Top = 64
  end
  object OrderListQuery: TQuery
    DatabaseName = 'BackOffice'
    SQL.Strings = (
      'select * from KeyOrders where key_id = :key_id')
    Left = 16
    Top = 136
    ParamData = <
      item
        DataType = ftInteger
        Name = 'key_id'
        ParamType = ptUnknown
        Value = '0'
      end>
  end
  object ImageList1: TImageList
    Left = 49
    Top = 136
    Bitmap = {
      494C010103000400040010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000001000000001001000000000000008
      000000000000000000000000000000000000FF7F000000000000000000000000
      000000000000000000000000000000000000FF7F000000000000000000000000
      0000000000000000000000000000E07FE07FFF7F000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000E07F0042E07F0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000FF7FFF7FFF7FFF7FFF7FFF7F
      FF7FFF7FFF7FFF7FFF7FFF7FFF7F000000000000FF7FFF7FFF7FFF7FFF7FFF7F
      FF7FFF7FFF7FFF7FFF7FE07FE07FE07F10420000FF7FFF7FFF7FFF7FFF7FFF7F
      FF7FFF7FFF7FFF7FFF7FFF7FFF7F000000000000000000000000000000000000
      0000000000000000000000000000000000000000FF7FFF7FFF7FFF7FFF7FFF7F
      FF7FFF7FFF7FFF7FFF7FFF7FFF7F000000000000FF7F1863FF7FFF7F00000000
      000000000000000000000042E07F104200000000FF7F0000FF7F0000FF7F0000
      FF7F0000FF7F0000FF7F0000FF7F000000000000000000000000000000000000
      00000000000000000000000000000000000000001863000018630000FF7F0000
      0000FF7F186300000000FF7FFF7F00000000000018631863186300001863FF7F
      1863FF7F1863FF7F186300001042000000000000FF7F00000000FF7FFF7F0000
      FF7F0000FF7F000000000000FF7F000000000000000000000000000000000000
      0000000000000000000000000000000000000000186300000000000018630000
      18630000186300001863000018630000000000001863186300001042FF7F0000
      0000000000000000FF7F00001863000000000000FF7F000000000000FF7F0000
      00000000FF7F0000FF7F0000FF7F000000000000000000000000000000000000
      0000000000000000000000000000000000000000186300001863000018630000
      18630000186300001863000018630000000000001863186300001042FF7F0000
      FF7FFF7F0000FF7FFF7F00001863000000000000FF7F0000FF7F0000FF7F0000
      00000000FF7F0000FF7F0000FF7F000000000000000000000000000000000000
      0000000000000000000000000000000000000000186300001863000018630000
      18630000186300001863000018630000000000001863104200001042FF7F0000
      0000000000001863FF7F00001863000000000000FF7F000000000000FF7F0000
      FF7F0000FF7F000000000000FF7F000000000000000000000000000000000000
      0000000000000000000000000000000000000000186300000000000018630000
      00001863186300000000186318630000000000001042186300001042FF7FFF7F
      FF7FFF7F0000FF7FFF7F00001042000000000000FF7FFF7FFF7FFF7FFF7FFF7F
      FF7FFF7FFF7FFF7FFF7FFF7FFF7F000000000000000000000000000000000000
      0000000000000000000000000000000000000000186310421042104210421042
      10421042186310421042104210420000000000001042104200001042FF7F0000
      0000000000001863000000001042000000000000186318631863186318631863
      1863007C1863FF7FFF7F1863FF7F000000000000000000000000000000000000
      0000000000000000000000000000000000000000104210421042104210421042
      FF7F1042104210421042104210420000000000001F001042104200001042FF7F
      FF7FFF7FFF7F0000000010421F00000000000000007C007C007C007C007C007C
      007C007C007CFF7F186318631863000000000000000000000000000000000000
      0000000000000000000000000000000000000000FF7FFF7FFF7FFF7FFF7FFF7F
      FF7FFF7F007C007C0040004000400000000000001F001F0010421F0000000000
      000000000000000010421F001F00000000000000007C007C007C007C007C007C
      007C007C007C18630000000000000000FF7F0000000000000000000000000000
      0000000000000000000000000000000000000000FF7FFF7FFF7FFF7FFF7FFF7F
      FF7FFF7F007C007C0000000000000000FF7F00001F001F001F001F001F001F00
      1F001F001F001F000000000000000000FF7F00001863FF7F1863FF7F1863FF7F
      1863007C1863FF7F0000FF7FFF7F0000FF7F0000000000000000000000000000
      0000000000000000000000000000000000000000007C007C007C007C007C007C
      FF7F007C007C007C0000FF7FFF7F0000FF7F00001F001F001F001F001F001F00
      1F001F001F001F000000FF7FFF7F0000FF7F0000FF7F186318631863FF7F1863
      FF7F1863FF7F18630000FF7F0000FF7FFF7F0000000000000000000000000000
      0000000000000000000000000000000000000000007C007C007C007C007C007C
      007C007C007C007C0000FF7F0000FF7FFF7F00001F001F001F001F001F001F00
      1F001F001F001F000000FF7F0000FF7FFF7F0000000000000000000000000000
      000000000000000000000000FF7FFF7FFF7F0000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000FF7FFF7FFF7F0000000000000000000000000000
      000000000000000000000000FF7FFF7FFF7FFF7FFF7FFF7FFF7FFF7FFF7FFF7F
      FF7FFF7FFF7FFF7FFF7FFF7FFF7FFF7FFF7F0000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000100000000100010000000000800000000000000000000000
      000000000000000000000000FFFFFF0000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000}
  end
  object cQuery: TQuery
    DatabaseName = 'CustDB'
    SQL.Strings = (
      'SELECT * FROM sPasswordLookup')
    Left = 18
    Top = 178
  end
  object PasswordDataSource: TDataSource
    DataSet = cQuery
    Left = 50
    Top = 178
  end
end
