; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CNotifyDialog
LastTemplate=CPropertyPage
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "AntiKeylogger.h"

ClassCount=7
Class1=CAntiKeyloggerApp
Class2=CAntiKeyloggerDlg

ResourceCount=7
Resource2=IDD_WHITE
Resource1=IDR_MAINFRAME
Class3=CNotifyDialog
Resource3=IDD_DETECT
Resource4=IDD_OPTION_USER
Resource5=IDD_NOTIFY
Resource6=IDD_ANTIKEYLOGGER_DIALOG
Class4=CDetectPage
Class5=CWhitePage
Class6=CUserOptionPage
Class7=CAdvancedOptionPage
Resource7=IDD_OPTION_ADVANCED

[CLS:CAntiKeyloggerApp]
Type=0
HeaderFile=AntiKeylogger.h
ImplementationFile=AntiKeylogger.cpp
Filter=N

[CLS:CAntiKeyloggerDlg]
Type=0
HeaderFile=AntiKeyloggerDlg.h
ImplementationFile=AntiKeyloggerDlg.cpp
Filter=W
BaseClass=CTrayDialog
VirtualFilter=dWC
LastObject=CAntiKeyloggerDlg



[DLG:IDD_ANTIKEYLOGGER_DIALOG]
Type=1
Class=CAntiKeyloggerDlg
ControlCount=4
Control1=IDC_STOP,button,1342242816
Control2=IDC_TREE_CTRL,SysTreeView32,1350631463
Control3=IDC_PAGE_FRAME,static,1073741832
Control4=IDC_CAPTION_BAR,static,1342308352

[DLG:IDD_NOTIFY]
Type=1
Class=CNotifyDialog
ControlCount=1
Control1=IDC_NOTIFY,static,1342308608

[CLS:CNotifyDialog]
Type=0
HeaderFile=NotifyDialog.h
ImplementationFile=NotifyDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=CNotifyDialog
VirtualFilter=dWC

[DLG:IDD_DETECT]
Type=1
Class=CDetectPage
ControlCount=1
Control1=IDC_DETECT_LIST,SysListView32,1350680589

[DLG:IDD_WHITE]
Type=1
Class=CWhitePage
ControlCount=3
Control1=IDC_WHITE_LIST,SysListView32,1350680589
Control2=IDC_ADD,button,1342242816
Control3=IDC_REMOVE,button,1342242816

[DLG:IDD_OPTION_USER]
Type=1
Class=CUserOptionPage
ControlCount=4
Control1=IDC_CHECK00,button,1342242819
Control2=IDC_CHECK01,button,1342242819
Control3=IDC_CHECK02,button,1342242819
Control4=IDC_CHECK4,button,1342242819

[DLG:IDD_OPTION_ADVANCED]
Type=1
Class=CAdvancedOptionPage
ControlCount=11
Control1=IDC_CHECK10,button,1342242819
Control2=IDC_CHECK11,button,1342251011
Control3=IDC_CHECK12,button,1342242819
Control4=IDC_CHECK13,button,1342242819
Control5=IDC_CHECK14,button,1342251011
Control6=IDC_CHECK15,button,1342242819
Control7=IDC_CHECK16,button,1342251011
Control8=IDC_CHECK17,button,1342242819
Control9=IDC_CHECK18,button,1342251011
Control10=IDC_CHECK19,button,1342242819
Control11=IDC_CHECK1A,button,1342242819

[CLS:CDetectPage]
Type=0
HeaderFile=DetectPage.h
ImplementationFile=DetectPage.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=IDC_DETECT_LIST
VirtualFilter=idWC

[CLS:CWhitePage]
Type=0
HeaderFile=WhitePage.h
ImplementationFile=WhitePage.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=CWhitePage
VirtualFilter=idWC

[CLS:CUserOptionPage]
Type=0
HeaderFile=UserOptionPage.h
ImplementationFile=UserOptionPage.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=CUserOptionPage

[CLS:CAdvancedOptionPage]
Type=0
HeaderFile=AdvancedOptionPage.h
ImplementationFile=AdvancedOptionPage.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=CAdvancedOptionPage

