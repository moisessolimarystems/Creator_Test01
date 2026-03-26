object DlgPPM: TDlgPPM
  Left = 339
  Top = 181
  Width = 219
  Height = 126
  Caption = 'Pages Per Minute'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Bevel1: TBevel
    Left = 16
    Top = 9
    Width = 185
    Height = 48
    Shape = bsFrame
  end
  object Label1: TLabel
    Left = 34
    Top = 25
    Width = 87
    Height = 13
    Caption = 'Pages Per Minute:'
  end
  object PagesEditBox: TMaskEdit
    Left = 133
    Top = 21
    Width = 52
    Height = 21
    EditMask = '099;0; '
    MaxLength = 3
    TabOrder = 0
    Text = '0'
  end
  object CancelBtn: TButton
    Left = 105
    Top = 64
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object okBtn: TButton
    Left = 32
    Top = 64
    Width = 75
    Height = 25
    Caption = 'OK'
    ModalResult = 1
    TabOrder = 2
  end
end
