object ksdSearchDlg: TksdSearchDlg
  Left = 533
  Top = 260
  Width = 374
  Height = 336
  Caption = 'Search'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object ksdPanel: TPanel
    Left = 0
    Top = 0
    Width = 366
    Height = 309
    Align = alClient
    TabOrder = 0
    object ksdKeyNumberLabel: TLabel
      Left = 24
      Top = 64
      Width = 102
      Height = 20
      Caption = 'Key Number:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object ksdVersionNumberLabel: TLabel
      Left = 24
      Top = 104
      Width = 134
      Height = 20
      Caption = 'Version Number:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object ksdLicenseLabel: TLabel
      Left = 24
      Top = 144
      Width = 68
      Height = 20
      Caption = 'License:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object ksdKeyTypeLabel: TLabel
      Left = 24
      Top = 185
      Width = 78
      Height = 20
      Caption = 'Key Type:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object ksdCustomerNumberLabel: TLabel
      Left = 24
      Top = 24
      Width = 150
      Height = 20
      Caption = 'Customer Number:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object ksdSearchButton: TButton
      Left = 184
      Top = 248
      Width = 75
      Height = 25
      Caption = 'Search'
      ModalResult = 1
      TabOrder = 0
    end
    object ksdCancelButton: TButton
      Left = 272
      Top = 248
      Width = 75
      Height = 25
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 6
    end
    object ksdKeyMaskEdit: TMaskEdit
      Left = 224
      Top = 63
      Width = 97
      Height = 21
      Enabled = False
      EditMask = 'aaa\-aaa;1;*'
      MaxLength = 7
      TabOrder = 2
      Text = '   -   '
      OnChange = ksdKeyMaskEditChange
    end
    object ksdVersionMaskEdit: TMaskEdit
      Left = 224
      Top = 104
      Width = 97
      Height = 21
      Enabled = False
      EditMask = 'a\.aaa;0;*'
      MaxLength = 5
      TabOrder = 3
      OnChange = ksdVersionMaskEditChange
    end
    object ksdLicenseComboBox: TComboBox
      Left = 224
      Top = 143
      Width = 113
      Height = 21
      ItemHeight = 13
      TabOrder = 4
      OnChange = ksdLicenseComboBoxChange
      Items.Strings = (
        'Initial'
        'Permanent'
        'Deactivated'
        'Extension 1'
        'Extension 2'
        'Extension 3'
        'Extension 4'
        'Extension 5'
        'Lost'
        'Returned')
    end
    object ksdKeyTypeComboBox: TComboBox
      Left = 224
      Top = 184
      Width = 113
      Height = 21
      ItemHeight = 13
      TabOrder = 5
      OnChange = ksdKeyTypeComboBoxChange
      Items.Strings = (
        'RESERVED'
        'Add-on'
        'Backup'
        'Base'
        'Custom'
        'Demo'
        'Emergency'
        'Inventory'
        'Loan'
        'None'
        'Relicense'
        'Rental'
        'Replacement'
        'Test/Dev')
    end
    object ksdCustomerMaskEdit: TMaskEdit
      Left = 224
      Top = 24
      Width = 97
      Height = 21
      Enabled = False
      EditMask = 'aaa;0;*'
      MaxLength = 3
      TabOrder = 1
      OnChange = ksdCustomerMaskEditChange
    end
  end
end
