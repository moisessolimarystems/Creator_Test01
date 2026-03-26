object DlgDevelopmentPassword: TDlgDevelopmentPassword
  Left = 567
  Top = 460
  Width = 265
  Height = 141
  Caption = 'DlgDevelopmentPassword'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object PasswordSelection: TGroupBox
    Left = 8
    Top = 8
    Width = 241
    Height = 65
    Caption = 'Choose your password:'
    TabOrder = 0
    object passwordComboBox: TComboBox
      Left = 32
      Top = 24
      Width = 193
      Height = 21
      DropDownCount = 25
      ItemHeight = 13
      TabOrder = 0
      Text = 'passwordComboBox'
    end
  end
  object OKButton: TButton
    Left = 96
    Top = 80
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 1
    OnClick = OKButtonClick
  end
  object CancelButton: TButton
    Left = 176
    Top = 80
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    TabOrder = 2
    OnClick = CancelButtonClick
  end
end
