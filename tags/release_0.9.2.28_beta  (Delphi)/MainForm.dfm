object MainForm: TMainForm
  Left = 0
  Top = 0
  BorderStyle = bsNone
  Caption = 'Mini Launch Bar'
  ClientHeight = 71
  ClientWidth = 341
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object trayIconPopupMenu: TPopupMenu
    OnPopup = trayIconPopupMenuPopup
    Left = 16
    Top = 8
    object trayIconPopupMenuHideShow: TMenuItem
      Caption = 'Hide / Show'
      OnClick = trayIconPopupMenuHideShowClick
    end
    object N1: TMenuItem
      Caption = '-'
      Enabled = False
    end
    object trayIconPopupMenuClose: TMenuItem
      Caption = 'Close'
      OnClick = trayIconPopupMenuCloseClick
    end
  end
end
