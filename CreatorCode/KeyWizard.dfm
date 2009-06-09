object KeyWizardFrm: TKeyWizardFrm
  Left = 514
  Top = 175
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
  TextHeight = 13
  object FinalPanel: TPanel
    Left = 161
    Top = 0
    Width = 617
    Height = 453
    Align = alClient
    BevelOuter = bvNone
    BorderWidth = 5
    BorderStyle = bsSingle
    TabOrder = 3
    Visible = False
  end
  object SolSearcherEnterprisePanel: TPanel
    Left = 161
    Top = 0
    Width = 617
    Height = 453
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 5
    inline TSolSearcherDetails1: TSolSearcherDetails
      Left = 0
      Top = 8
      Width = 617
      Height = 458
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
    Left = 161
    Top = 0
    Width = 617
    Height = 453
    Align = alClient
    BorderWidth = 5
    BorderStyle = bsSingle
    TabOrder = 4
    Visible = False
    object OutputLabel: TLabel
      Left = 285
      Top = 340
      Width = 101
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Unlimited Pool'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object ConcurrentLabel: TLabel
      Left = 285
      Top = 365
      Width = 101
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Concurrent Operators'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object NamedLabel: TLabel
      Left = 285
      Top = 390
      Width = 101
      Height = 13
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Named Users'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object AppInstanceLabel: TLabel
      Left = 8
      Top = 344
      Width = 86
      Height = 13
      Caption = 'Remote Instances'
    end
    inline WizardModuleFrame: TModuleFrame
      Left = 10
      Top = 18
      Width = 441
      Height = 307
      TabOrder = 0
      inherited ModuleList: TListView
        Width = 441
        Height = 307
        Align = alClient
      end
    end
    object OutputEdit: TMaskEdit
      Left = 400
      Top = 337
      Width = 36
      Height = 21
      TabOrder = 1
      Text = '4095'
      OnChange = OutputEditChange
    end
    object OutputUnits: TUpDown
      Left = 436
      Top = 337
      Width = 16
      Height = 21
      Associate = OutputEdit
      Min = 0
      Max = 4095
      Position = 4095
      TabOrder = 2
      Thousands = False
      Wrap = False
    end
    object ConcurrentEdit: TMaskEdit
      Left = 400
      Top = 362
      Width = 36
      Height = 21
      TabOrder = 3
      Text = '255'
    end
    object NamedEdit: TMaskEdit
      Left = 400
      Top = 387
      Width = 36
      Height = 21
      TabOrder = 4
      Text = '255'
    end
    object ConcurrentUnits: TUpDown
      Left = 436
      Top = 362
      Width = 15
      Height = 21
      Associate = ConcurrentEdit
      Min = 0
      Max = 255
      Position = 255
      TabOrder = 5
      Thousands = False
      Wrap = False
    end
    object NamedUnits: TUpDown
      Left = 436
      Top = 387
      Width = 15
      Height = 21
      Associate = NamedEdit
      Min = 0
      Max = 255
      Position = 255
      TabOrder = 6
      Thousands = False
      Wrap = False
    end
    object AppInstanceUnits: TUpDown
      Left = 156
      Top = 340
      Width = 16
      Height = 21
      Associate = AppInstanceEdit
      Min = 0
      Max = 99
      Position = 0
      TabOrder = 7
      Thousands = False
      Wrap = False
    end
    object AppInstanceEdit: TMaskEdit
      Left = 120
      Top = 340
      Width = 36
      Height = 21
      TabOrder = 8
      Text = '0'
    end
  end
  object StartPanel: TPanel
    Left = 161
    Top = 0
    Width = 617
    Height = 453
    Align = alClient
    BevelOuter = bvNone
    BorderStyle = bsSingle
    TabOrder = 0
    object Label5: TLabel
      Left = 8
      Top = 64
      Width = 64
      Height = 13
      Caption = 'Product Type'
    end
    object Label6: TLabel
      Left = 8
      Top = 104
      Width = 45
      Height = 13
      Caption = 'Key Type'
    end
    object Label1: TLabel
      Left = 158
      Top = 64
      Width = 35
      Height = 13
      Caption = 'Version'
    end
    object DaysLabel: TLabel
      Left = 158
      Top = 104
      Width = 24
      Height = 13
      Caption = 'Days'
    end
    object SystemEditLabel: TLabel
      Left = 8
      Top = 220
      Width = 46
      Height = 13
      Caption = 'System Id'
    end
    object Label7: TLabel
      Left = 8
      Top = 263
      Width = 43
      Height = 13
      Caption = 'Summary'
    end
    object eBONumberLabel: TLabel
      Left = 158
      Top = 216
      Width = 61
      Height = 13
      Caption = 'eBO Number'
    end
    object keyClassLabel: TLabel
      Left = 8
      Top = 16
      Width = 46
      Height = 13
      Caption = 'Key Class'
    end
    object passwordComboBoxLabel: TLabel
      Left = 158
      Top = 104
      Width = 106
      Height = 13
      Caption = 'Exp Days/Counter Hrs'
    end
    object ProductComboBox: TComboBox
      Left = 7
      Top = 78
      Width = 146
      Height = 21
      ItemHeight = 13
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
        'Rubika'
        'SOLfusion'
        'SP/D Enterprise')
    end
    object keyTypeComboBox: TComboBox
      Left = 7
      Top = 119
      Width = 146
      Height = 21
      ItemHeight = 13
      TabOrder = 1
      OnChange = keyTypeComboBoxChange
    end
    object ui_version: TMaskEdit
      Left = 166
      Top = 78
      Width = 42
      Height = 21
      TabOrder = 2
      OnChange = ui_versionChange
      OnExit = ui_versionExit
    end
    object ui_days: TMaskEdit
      Left = 166
      Top = 119
      Width = 43
      Height = 21
      TabOrder = 3
      Text = '30'
      OnChange = ui_daysChange
    end
    object DayUpDown: TUpDown
      Left = 209
      Top = 119
      Width = 16
      Height = 21
      Associate = ui_days
      Min = -10
      Max = 90
      Position = 30
      TabOrder = 4
      Wrap = False
    end
    object SelectModules: TCheckBox
      Left = 272
      Top = 32
      Width = 289
      Height = 17
      Caption = 'Specify modules to be programed.'
      TabOrder = 5
      Visible = False
      OnClick = SelectModulesClick
    end
    object SystemEdit: TEdit
      Left = 7
      Top = 234
      Width = 117
      Height = 21
      TabOrder = 6
      Text = 'SystemEdit'
      OnChange = SystemEditChange
    end
    object KeySummary: TMemo
      Left = 7
      Top = 284
      Width = 446
      Height = 61
      MaxLength = 254
      TabOrder = 7
    end
    object eBONumberEditBox: TEdit
      Left = 158
      Top = 232
      Width = 75
      Height = 21
      TabOrder = 8
      Text = '0'
      OnChange = eBONumberEditBoxChange
    end
    object keyClassComboBox: TComboBox
      Left = 8
      Top = 32
      Width = 145
      Height = 21
      ItemHeight = 13
      TabOrder = 9
      OnChange = keyClassComboBoxChange
      Items.Strings = (
        'Parallel'
        'USB')
    end
    object passwordComboBox: TComboBox
      Left = 166
      Top = 119
      Width = 169
      Height = 21
      DropDownCount = 25
      ItemHeight = 13
      TabOrder = 10
      OnChange = passwordComboBoxChange
    end
    object RemoteKey_CHB: TCheckBox
      Left = 272
      Top = 48
      Width = 249
      Height = 17
      Caption = 'Remote Key'
      TabOrder = 11
    end
  end
  object ButtonPanel: TPanel
    Left = 0
    Top = 453
    Width = 778
    Height = 39
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    DesignSize = (
      778
      39)
    object Bevel1: TBevel
      Left = 0
      Top = 0
      Width = 778
      Height = 9
      Align = alTop
      Shape = bsTopLine
    end
    object NextBtn: TButton
      Left = 475
      Top = 16
      Width = 75
      Height = 25
      Anchors = [akRight, akBottom]
      Caption = '&Next >'
      TabOrder = 0
      OnClick = NextBtnClick
    end
    object BackBtn: TButton
      Left = 400
      Top = 16
      Width = 75
      Height = 25
      Anchors = [akRight, akBottom]
      Caption = '< &Back'
      Enabled = False
      TabOrder = 1
      OnClick = BackBtnClick
    end
    object CancelBtn: TButton
      Left = 644
      Top = 16
      Width = 76
      Height = 25
      Anchors = [akRight, akBottom]
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 2
    end
    object ProgramBtn: TButton
      Left = 553
      Top = 16
      Width = 74
      Height = 25
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
    Width = 161
    Height = 453
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
      0000000000003600000028000000400000001000000001002000000000000010
      000000000000000000000000000000000000FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF0000FFFF00FFFFFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000FFFF000084840000FFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0000FFFF0000FFFF0000FFFF008484840000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00000000000000000000000000FFFFFF00C6C6C600FFFF
      FF00FFFFFF000000000000000000000000000000000000000000000000000000
      00000084840000FFFF00848484000000000000000000FFFFFF0000000000FFFF
      FF0000000000FFFFFF0000000000FFFFFF0000000000FFFFFF0000000000FFFF
      FF0000000000FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000C6C6C60000000000C6C6
      C60000000000FFFFFF000000000000000000FFFFFF00C6C6C600000000000000
      0000FFFFFF00FFFFFF00000000000000000000000000C6C6C600C6C6C600C6C6
      C60000000000C6C6C600FFFFFF00C6C6C600FFFFFF00C6C6C600FFFFFF00C6C6
      C6000000000084848400000000000000000000000000FFFFFF00000000000000
      0000FFFFFF00FFFFFF0000000000FFFFFF0000000000FFFFFF00000000000000
      000000000000FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000C6C6C600000000000000
      000000000000C6C6C60000000000C6C6C60000000000C6C6C60000000000C6C6
      C60000000000C6C6C600000000000000000000000000C6C6C600C6C6C6000000
      000084848400FFFFFF000000000000000000000000000000000000000000FFFF
      FF0000000000C6C6C600000000000000000000000000FFFFFF00000000000000
      000000000000FFFFFF00000000000000000000000000FFFFFF0000000000FFFF
      FF0000000000FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000C6C6C60000000000C6C6
      C60000000000C6C6C60000000000C6C6C60000000000C6C6C60000000000C6C6
      C60000000000C6C6C600000000000000000000000000C6C6C600C6C6C6000000
      000084848400FFFFFF0000000000FFFFFF00FFFFFF0000000000FFFFFF00FFFF
      FF0000000000C6C6C600000000000000000000000000FFFFFF0000000000FFFF
      FF0000000000FFFFFF00000000000000000000000000FFFFFF0000000000FFFF
      FF0000000000FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000C6C6C60000000000C6C6
      C60000000000C6C6C60000000000C6C6C60000000000C6C6C60000000000C6C6
      C60000000000C6C6C600000000000000000000000000C6C6C600848484000000
      000084848400FFFFFF0000000000000000000000000000000000C6C6C600FFFF
      FF0000000000C6C6C600000000000000000000000000FFFFFF00000000000000
      000000000000FFFFFF0000000000FFFFFF0000000000FFFFFF00000000000000
      000000000000FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000C6C6C600000000000000
      000000000000C6C6C6000000000000000000C6C6C600C6C6C600000000000000
      0000C6C6C600C6C6C60000000000000000000000000084848400C6C6C6000000
      000084848400FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000FFFFFF00FFFF
      FF000000000084848400000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000C6C6C600848484008484
      84008484840084848400848484008484840084848400C6C6C600848484008484
      8400848484008484840000000000000000000000000084848400848484000000
      000084848400FFFFFF0000000000000000000000000000000000C6C6C6000000
      00000000000084848400000000000000000000000000C6C6C600C6C6C600C6C6
      C600C6C6C600C6C6C600C6C6C600C6C6C6000000FF00C6C6C600FFFFFF00FFFF
      FF00C6C6C600FFFFFF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000084848400848484008484
      8400848484008484840084848400FFFFFF008484840084848400848484008484
      84008484840084848400000000000000000000000000FF000000848484008484
      84000000000084848400FFFFFF00FFFFFF00FFFFFF00FFFFFF00000000000000
      000084848400FF0000000000000000000000000000000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF00FFFFFF00C6C6
      C600C6C6C600C6C6C60000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000FF000000FF000000
      84000000840000008400000000000000000000000000FF000000FF0000008484
      8400FF0000000000000000000000000000000000000000000000000000008484
      8400FF000000FF0000000000000000000000000000000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF00C6C6C6000000
      0000000000000000000000000000FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000FF000000FF000000
      0000000000000000000000000000FFFFFF0000000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF0000000000
      0000000000000000000000000000FFFFFF0000000000C6C6C600FFFFFF00C6C6
      C600FFFFFF00C6C6C600FFFFFF00C6C6C6000000FF00C6C6C600FFFFFF000000
      0000FFFFFF00FFFFFF0000000000FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000FF000000FF000000
      FF000000FF000000FF000000FF00FFFFFF000000FF000000FF000000FF000000
      0000FFFFFF00FFFFFF0000000000FFFFFF0000000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF0000000000
      0000FFFFFF00FFFFFF0000000000FFFFFF0000000000FFFFFF00C6C6C600C6C6
      C600C6C6C600FFFFFF00C6C6C600FFFFFF00C6C6C600FFFFFF00C6C6C6000000
      0000FFFFFF0000000000FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      0000FFFFFF0000000000FFFFFF00FFFFFF0000000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF0000000000
      0000FFFFFF0000000000FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
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
