object SolSearcherDetails: TSolSearcherDetails
  Left = 0
  Top = 0
  Width = 771
  Height = 330
  Color = clBackground
  ParentColor = False
  TabOrder = 0
  object SolSearcherPropertyList: TListView
    Left = 16
    Top = 24
    Width = 217
    Height = 257
    Columns = <
      item
        Caption = 'Property'
        Width = 150
      end
      item
        Caption = 'Licenses'
        Width = 60
      end>
    FlatScrollBars = True
    PopupMenu = PropertyPopup
    TabOrder = 0
    ViewStyle = vsReport
  end
  object DetailSQL: TQuery
    DatabaseName = 'CustDB'
    SQL.Strings = (
      'select * from sModuleDetail')
    Left = 64
    Top = 48
  end
  object PropertyPopup: TPopupMenu
    Left = 28
    Top = 45
    object GeneratePasswordMenuItem: TMenuItem
      Caption = 'Generate Password'
      OnClick = GeneratePasswordMenuItemClick
    end
    object SetValueMenuItem: TMenuItem
      Caption = 'Set Value'
      OnClick = SetValueMenuItemClick
    end
  end
end
