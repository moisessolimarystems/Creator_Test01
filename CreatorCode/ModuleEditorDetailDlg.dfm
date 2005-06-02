object ModuleEditorDetailsDlg: TModuleEditorDetailsDlg
  Left = 155
  Top = 27
  Width = 449
  Height = 493
  Caption = 'Module Editor Details'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 14
  object OK: TButton
    Left = 246
    Top = 430
    Width = 80
    Height = 27
    Caption = 'OK'
    ModalResult = 1
    TabOrder = 0
  end
  object Cancel: TButton
    Left = 345
    Top = 430
    Width = 80
    Height = 27
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object GroupBox1: TGroupBox
    Left = 9
    Top = 9
    Width = 423
    Height = 408
    Caption = 'Module Details'
    TabOrder = 2
    object ModIDLabel: TLabel
      Left = 43
      Top = 52
      Width = 49
      Height = 14
      Caption = 'Module ID:'
    end
    object ModNameLabel: TLabel
      Left = 43
      Top = 78
      Width = 67
      Height = 14
      Caption = 'Module Name:'
    end
    object ModTypeLabel: TLabel
      Left = 43
      Top = 103
      Width = 64
      Height = 14
      Caption = 'Module Type:'
    end
    object ModEngineerLabel: TLabel
      Left = 43
      Top = 129
      Width = 82
      Height = 14
      Caption = 'Module Engineer:'
    end
    object ModVersionAddedLabel: TLabel
      Left = 43
      Top = 155
      Width = 79
      Height = 14
      Caption = 'Version Added: '
    end
    object ModVersionRemovedLabel: TLabel
      Left = 43
      Top = 181
      Width = 89
      Height = 14
      Caption = 'Version Removed:'
    end
    object ModIDNumberLabel: TLabel
      Left = 146
      Top = 34
      Width = 3
      Height = 14
    end
    object ModulePartNumberLabel: TLabel
      Left = 43
      Top = 207
      Width = 62
      Height = 14
      Caption = 'Part Number:'
    end
    object ModMaxLabel: TLabel
      Left = 43
      Top = 233
      Width = 60
      Height = 14
      Caption = 'Module Max:'
    end
    object ProductIDLabel: TLabel
      Left = 43
      Top = 26
      Width = 52
      Height = 14
      Caption = 'Product ID:'
    end
    object ModNameEdit: TEdit
      Left = 146
      Top = 69
      Width = 217
      Height = 22
      TabOrder = 0
    end
    object ModVersionAddedEdit: TEdit
      Left = 146
      Top = 146
      Width = 217
      Height = 22
      CharCase = ecUpperCase
      TabOrder = 1
    end
    object ModTypeCombo: TComboBox
      Left = 146
      Top = 95
      Width = 217
      Height = 22
      ItemHeight = 14
      Sorted = True
      TabOrder = 2
      Items.Strings = (
        'Emulation'
        'Input'
        'Output'
        'Undefined')
    end
    object ModEngineerCombo: TComboBox
      Left = 146
      Top = 121
      Width = 217
      Height = 22
      ItemHeight = 14
      Sorted = True
      TabOrder = 3
      Items.Strings = (
        'Alex Ferguson'
        'Bert Jackson'
        'Bill DesRoches'
        'Drew Sprague'
        'Dylan Miller'
        'Jeff Lan'
        'N/A'
        'Steve Bailey')
    end
    object ModVersionRemovedEdit: TEdit
      Left = 146
      Top = 172
      Width = 217
      Height = 22
      CharCase = ecUpperCase
      TabOrder = 4
    end
    object ModPartNumberEdit: TEdit
      Left = 146
      Top = 198
      Width = 217
      Height = 22
      TabOrder = 5
    end
    object ModMaxEdit: TEdit
      Left = 146
      Top = 224
      Width = 217
      Height = 22
      TabOrder = 6
    end
    object ModSpdDefaultCheckBox: TCheckBox
      Left = 146
      Top = 258
      Width = 105
      Height = 19
      Caption = 'Spd Default'
      TabOrder = 7
    end
    object ModConnectivityDefaultCheckBox: TCheckBox
      Left = 146
      Top = 284
      Width = 148
      Height = 19
      Caption = 'Connectivity Default'
      TabOrder = 8
      OnClick = ModConnectivityDefaultCheckBoxClick
    end
    object ModConnectivityModuleCheckBox: TCheckBox
      Left = 146
      Top = 310
      Width = 139
      Height = 18
      Caption = 'Connectivity Module'
      TabOrder = 9
    end
    object ModiConvertDefaultCheckBox: TCheckBox
      Left = 146
      Top = 336
      Width = 131
      Height = 18
      Caption = 'iConvert Default'
      TabOrder = 10
      OnClick = ModiConvertDefaultCheckBoxClick
    end
    object ModiConvertModuleCheckBox: TCheckBox
      Left = 146
      Top = 362
      Width = 165
      Height = 18
      Caption = 'iConvert Module'
      TabOrder = 11
    end
  end
  object UpdateButton: TButton
    Left = 146
    Top = 430
    Width = 81
    Height = 27
    Caption = 'Update'
    TabOrder = 3
    OnClick = UpdateButtonClick
  end
end
