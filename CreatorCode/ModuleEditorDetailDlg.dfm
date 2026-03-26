object ModuleEditorDetailsDlg: TModuleEditorDetailsDlg
  Left = 420
  Top = 189
  Width = 418
  Height = 452
  Caption = 'Module Editor Details'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object OK: TButton
    Left = 228
    Top = 392
    Width = 75
    Height = 25
    Caption = 'OK'
    ModalResult = 1
    TabOrder = 0
  end
  object Cancel: TButton
    Left = 320
    Top = 392
    Width = 75
    Height = 25
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 393
    Height = 369
    Caption = 'Module Details'
    TabOrder = 2
    object ModIDLabel: TLabel
      Left = 40
      Top = 48
      Width = 52
      Height = 13
      Caption = 'Module ID:'
    end
    object ModNameLabel: TLabel
      Left = 40
      Top = 72
      Width = 69
      Height = 13
      Caption = 'Module Name:'
    end
    object ModTypeLabel: TLabel
      Left = 40
      Top = 96
      Width = 65
      Height = 13
      Caption = 'Module Type:'
    end
    object ModEngineerLabel: TLabel
      Left = 40
      Top = 120
      Width = 83
      Height = 13
      Caption = 'Module Engineer:'
    end
    object ModVersionAddedLabel: TLabel
      Left = 40
      Top = 144
      Width = 75
      Height = 13
      Caption = 'Version Added: '
    end
    object ModVersionRemovedLabel: TLabel
      Left = 40
      Top = 168
      Width = 87
      Height = 13
      Caption = 'Version Removed:'
    end
    object ModIDNumberLabel: TLabel
      Left = 136
      Top = 32
      Width = 3
      Height = 13
    end
    object ModulePartNumberLabel: TLabel
      Left = 40
      Top = 192
      Width = 62
      Height = 13
      Caption = 'Part Number:'
    end
    object ModMaxLabel: TLabel
      Left = 40
      Top = 216
      Width = 61
      Height = 13
      Caption = 'Module Max:'
    end
    object ProductIDLabel: TLabel
      Left = 40
      Top = 24
      Width = 54
      Height = 13
      Caption = 'Product ID:'
    end
    object ModNameEdit: TEdit
      Left = 136
      Top = 64
      Width = 201
      Height = 21
      TabOrder = 0
    end
    object ModVersionAddedEdit: TEdit
      Left = 136
      Top = 136
      Width = 201
      Height = 21
      CharCase = ecUpperCase
      TabOrder = 1
    end
    object ModTypeCombo: TComboBox
      Left = 136
      Top = 88
      Width = 201
      Height = 21
      ItemHeight = 13
      Sorted = True
      TabOrder = 2
      Items.Strings = (
        'Emulation'
        'Input'
        'Output'
        'Undefined')
    end
    object ModEngineerCombo: TComboBox
      Left = 136
      Top = 112
      Width = 201
      Height = 21
      ItemHeight = 13
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
      Left = 136
      Top = 160
      Width = 201
      Height = 21
      CharCase = ecUpperCase
      TabOrder = 4
    end
    object ModPartNumberEdit: TEdit
      Left = 136
      Top = 184
      Width = 201
      Height = 21
      TabOrder = 5
    end
    object ModMaxEdit: TEdit
      Left = 136
      Top = 208
      Width = 201
      Height = 21
      TabOrder = 6
    end
    object ModSpdDefaultCheckBox: TCheckBox
      Left = 136
      Top = 240
      Width = 97
      Height = 17
      Caption = 'Spd Default'
      TabOrder = 7
    end
    object ModConnectivityDefaultCheckBox: TCheckBox
      Left = 136
      Top = 264
      Width = 137
      Height = 17
      Caption = 'Connectivity Default'
      TabOrder = 8
      OnClick = ModConnectivityDefaultCheckBoxClick
    end
    object ModConnectivityModuleCheckBox: TCheckBox
      Left = 136
      Top = 288
      Width = 129
      Height = 17
      Caption = 'Connectivity Module'
      TabOrder = 9
    end
    object ModiConvertDefaultCheckBox: TCheckBox
      Left = 136
      Top = 312
      Width = 121
      Height = 17
      Caption = 'iConvert Default'
      TabOrder = 10
      OnClick = ModiConvertDefaultCheckBoxClick
    end
    object ModiConvertModuleCheckBox: TCheckBox
      Left = 136
      Top = 336
      Width = 153
      Height = 17
      Caption = 'iConvert Module'
      TabOrder = 11
    end
  end
  object UpdateButton: TButton
    Left = 136
    Top = 392
    Width = 75
    Height = 25
    Caption = 'Update'
    TabOrder = 3
    OnClick = UpdateButtonClick
  end
end
