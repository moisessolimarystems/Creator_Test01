object dsSearchDlg: TdsSearchDlg
  Left = 524
  Top = 193
  Width = 327
  Height = 205
  Caption = 'dsSearchDlg'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 317
    Height = 179
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabOrder = 0
    object MinDate: TLabel
      Left = 10
      Top = 72
      Width = 150
      Height = 13
      Caption = 'Date Greater Than or Equal To:'
    end
    object MaxDate: TLabel
      Left = 81
      Top = 104
      Width = 79
      Height = 13
      Caption = 'Date Less Than:'
    end
    object dsMainLabel: TLabel
      Left = 72
      Top = 16
      Width = 164
      Height = 16
      Caption = 'Expiration Date Search:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label2: TLabel
      Left = 176
      Top = 48
      Width = 58
      Height = 13
      Caption = 'mm/dd/yyyy'
    end
    object Label3: TLabel
      Left = 112
      Top = 48
      Width = 48
      Height = 13
      Caption = 'FORMAT:'
    end
    object lessThanEditBox: TEdit
      Left = 176
      Top = 96
      Width = 129
      Height = 23
      AutoSize = False
      MaxLength = 10
      TabOrder = 1
    end
    object greaterThanEditBox: TEdit
      Left = 176
      Top = 64
      Width = 129
      Height = 23
      AutoSize = False
      MaxLength = 10
      TabOrder = 0
    end
    object dsSearchButton: TButton
      Left = 152
      Top = 136
      Width = 75
      Height = 25
      Caption = 'Search'
      ModalResult = 1
      TabOrder = 2
      OnClick = dsSearchButtonClick
    end
    object dsCancelButton: TButton
      Left = 232
      Top = 136
      Width = 75
      Height = 25
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 3
    end
  end
end
