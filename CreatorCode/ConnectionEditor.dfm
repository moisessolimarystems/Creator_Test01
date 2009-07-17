object ConnectionEditorDlg: TConnectionEditorDlg
  Left = 469
  Top = 466
  BorderStyle = bsDialog
  Caption = 'Connection Editor'
  ClientHeight = 134
  ClientWidth = 313
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Bevel1: TBevel
    Left = 8
    Top = 8
    Width = 297
    Height = 81
    Shape = bsFrame
  end
  object Label1: TLabel
    Left = 32
    Top = 42
    Width = 52
    Height = 13
    Caption = 'Key Server'
  end
  object OKBtn: TButton
    Left = 151
    Top = 100
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
    OnClick = OKBtnClick
  end
  object CancelBtn: TButton
    Left = 231
    Top = 100
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object KeyServerNameEB: TEdit
    Left = 104
    Top = 38
    Width = 177
    Height = 21
    TabOrder = 2
  end
end
