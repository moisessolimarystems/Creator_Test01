object ModuleEditorForm: TModuleEditorForm
  Left = 139
  Top = 153
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
  TextHeight = 13
  inline ModuleEditor1: TModuleEditor
    Width = 800
    Height = 375
    Align = alClient
    TabOrder = 2
    inherited ModuleFrame1: TModuleFrame
      Width = 377
      Height = 329
      inherited ModuleList: TListView
        Height = 329
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
    Left = 616
    Top = 344
    Width = 75
    Height = 25
    Caption = 'OK'
    ModalResult = 1
    TabOrder = 0
  end
  object Cancel: TBitBtn
    Left = 704
    Top = 344
    Width = 75
    Height = 25
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
end
