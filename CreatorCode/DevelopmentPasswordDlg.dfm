object DlgDevelopmentPassword: TDlgDevelopmentPassword
  Left = 630
  Top = 309
  Width = 289
  Height = 156
  Caption = 'DlgDevelopmentPassword'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 14
  object PasswordSelection: TGroupBox
    Left = 9
    Top = 9
    Width = 259
    Height = 70
    Caption = 'Choose your password:'
    TabOrder = 0
    object passwordComboBox: TComboBox
      Left = 34
      Top = 26
      Width = 208
      Height = 22
      DropDownCount = 25
      ItemHeight = 14
      TabOrder = 0
      Text = 'passwordComboBox'
    end
  end
  object OKButton: TButton
    Left = 103
    Top = 86
    Width = 81
    Height = 27
    Caption = 'OK'
    TabOrder = 1
    OnClick = OKButtonClick
  end
  object CancelButton: TButton
    Left = 190
    Top = 86
    Width = 80
    Height = 27
    Cancel = True
    Caption = 'Cancel'
    TabOrder = 2
    OnClick = CancelButtonClick
  end
end
