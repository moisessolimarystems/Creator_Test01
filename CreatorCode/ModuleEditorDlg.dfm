object ModuleEditorForm: TModuleEditorForm
  Left = 169
  Top = 300
  Width = 808
  Height = 402
  Caption = 'Module Editor'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 14
  inline ModuleEditor1: TModuleEditor
    Left = 0
    Top = 0
    Width = 839
    Height = 397
    Align = alClient
    TabOrder = 2
    inherited ModuleFrame1: TModuleFrame
      Left = 9
      Width = 406
      Height = 354
      inherited ModuleList: TListView
        Width = 406
        Height = 354
        Align = alClient
        OnDblClick = ModuleFrame1ModuleListDblClick
      end
      inherited ModuleDetailQuery: TQuery
        Left = 120
        Top = 72
      end
    end
  end
  object OK: TBitBtn
    Left = 663
    Top = 370
    Width = 81
    Height = 27
    Caption = 'OK'
    ModalResult = 1
    TabOrder = 0
  end
  object Cancel: TBitBtn
    Left = 758
    Top = 370
    Width = 81
    Height = 27
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
end
