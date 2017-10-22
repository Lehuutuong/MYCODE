# Microsoft Developer Studio Project File - Name="AntiKeylogger" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=AntiKeylogger - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AntiKeylogger.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AntiKeylogger.mak" CFG="AntiKeylogger - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AntiKeylogger - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "AntiKeylogger - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AntiKeylogger - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_UNINSTALL_NOTIFY" /D "_SELF_UNINSTALL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Sfc.lib Crypt32.lib Rpcrt4.lib Version.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "AntiKeylogger - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_UNINSTALL_NOTIFY" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Sfc.lib Crypt32.lib Rpcrt4.lib Version.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "AntiKeylogger - Win32 Release"
# Name "AntiKeylogger - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AdvancedOptionPage.cpp
# End Source File
# Begin Source File

SOURCE=.\AntiKeylogger.cpp
# End Source File
# Begin Source File

SOURCE=.\AntiKeyloggerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Comm.cpp
# End Source File
# Begin Source File

SOURCE=.\DetectPage.cpp
# End Source File
# Begin Source File

SOURCE=.\FileInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Global.cpp
# End Source File
# Begin Source File

SOURCE=".\hash&crypt.cpp"
# End Source File
# Begin Source File

SOURCE=.\hook.cpp
# End Source File
# Begin Source File

SOURCE=.\Init.cpp
# End Source File
# Begin Source File

SOURCE=.\logo.cpp
# End Source File
# Begin Source File

SOURCE=.\NotifyDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\password.cpp
# End Source File
# Begin Source File

SOURCE=.\PrefsStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\ThreadDataList.cpp
# End Source File
# Begin Source File

SOURCE=.\Trans.cpp
# End Source File
# Begin Source File

SOURCE=.\TrayDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\uninstall.cpp
# End Source File
# Begin Source File

SOURCE=.\UserOptionPage.cpp
# End Source File
# Begin Source File

SOURCE=.\WhitePage.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AdvancedOptionPage.h
# End Source File
# Begin Source File

SOURCE=.\AntiKeylogger.h
# End Source File
# Begin Source File

SOURCE=.\AntiKeyloggerDlg.h
# End Source File
# Begin Source File

SOURCE=.\Comm.h
# End Source File
# Begin Source File

SOURCE=.\Constdef.h
# End Source File
# Begin Source File

SOURCE=.\DetectPage.h
# End Source File
# Begin Source File

SOURCE=.\FileInfo.h
# End Source File
# Begin Source File

SOURCE=.\Global.h
# End Source File
# Begin Source File

SOURCE=".\hash&crypt.h"
# End Source File
# Begin Source File

SOURCE=.\hook.h
# End Source File
# Begin Source File

SOURCE=.\Init.h
# End Source File
# Begin Source File

SOURCE=.\logo.h
# End Source File
# Begin Source File

SOURCE=.\NotifyDialog.h
# End Source File
# Begin Source File

SOURCE=.\ntdll.h
# End Source File
# Begin Source File

SOURCE=.\ntstatus.h
# End Source File
# Begin Source File

SOURCE=.\password.h
# End Source File
# Begin Source File

SOURCE=.\PrefsStatic.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ThreadDataList.h
# End Source File
# Begin Source File

SOURCE=.\Trans.h
# End Source File
# Begin Source File

SOURCE=.\TrayDialog.h
# End Source File
# Begin Source File

SOURCE=.\uninstall.h
# End Source File
# Begin Source File

SOURCE=.\UserOptionPage.h
# End Source File
# Begin Source File

SOURCE=.\WhitePage.h
# End Source File
# Begin Source File

SOURCE=.\Wintrust.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\AntiKeylogger.ico
# End Source File
# Begin Source File

SOURCE=.\AntiKeylogger.rc
# End Source File
# Begin Source File

SOURCE=.\res\AntiKeylogger.rc2
# End Source File
# Begin Source File

SOURCE=.\res\bin1.bin
# End Source File
# Begin Source File

SOURCE=.\res\Data.bin
# End Source File
# Begin Source File

SOURCE=.\res\FACE03.ICO
# End Source File
# Begin Source File

SOURCE=.\res\FACE04.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Logo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Notify.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\res\SHKeyPro.sys
# End Source File
# End Target
# End Project
