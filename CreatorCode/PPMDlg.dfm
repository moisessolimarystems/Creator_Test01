object DlgPPM: TDlgPPM
  Left = 339
  Top = 181
  Width = 237
  Height = 127
  Caption = 'Pages Per Minute'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 14
  object Bevel1: TBevel
    Left = 17
    Top = 10
    Width = 199
    Height = 51
    Shape = bsFrame
  end
  object Label1: TLabel
    Left = 37
    Top = 27
    Width = 86
    Height = 14
    Caption = 'Pages Per Minute:'
  end
  object PagesEditBox: TMaskEdit
    Left = 143
    Top = 23
    Width = 56
    Height = 22
    EditMask = '099;0; '
    MaxLength = 3
    TabOrder = 0
    Text = '0'
  end
  object CancelBtn: TButton
    Left = 113
    Top = 69
    Width = 81
    Height = 27
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object okBtn: TButton
    Left = 34
    Top = 69
    Width = 81
    Height = 27
    Caption = 'OK'
    ModalResult = 1
    TabOrder = 2
  end
end
