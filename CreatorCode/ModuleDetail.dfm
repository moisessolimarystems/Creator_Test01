object ModuleFrame: TModuleFrame
  Left = 0
  Top = 0
  Width = 391
  Height = 337
  TabOrder = 0
  object ModuleList: TListView
    Left = 0
    Top = 0
    Width = 377
    Height = 313
    Columns = <>
    ReadOnly = True
    RowSelect = True
    PopupMenu = ModulePopup
    SortType = stBoth
    TabOrder = 0
    ViewStyle = vsReport
    OnColumnClick = ModuleListColumnClick
    OnCompare = ModuleListCompare
    OnKeyPress = ModuleListKeyPress
    OnMouseDown = ModuleListMouseDown
  end
  object ModuleDetailQuery: TQuery
    DatabaseName = 'CustDB'
    SQL.Strings = (
      'select * from sModuleDetail')
    Left = 64
    Top = 64
  end
  object ModulePopup: TPopupMenu
    AutoPopup = False
    Left = 136
    Top = 40
    object mmIncrementModule: TMenuItem
      Caption = 'Increment Module'
      OnClick = mmIncrementModuleClick
    end
    object mmSetModule: TMenuItem
      Caption = 'Set Module'
      OnClick = mmSetModuleClick
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object mmViewAll: TMenuItem
      Caption = 'Show &All'
      OnClick = mmViewAllClick
    end
    object mmPagesPerMinute: TMenuItem
      Caption = 'Pages Per Minute'
      OnClick = mmPagesPerMinuteClick
    end
  end
end
