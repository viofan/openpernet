# Microsoft Developer Studio Project File - Name="win32" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=win32 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "win32.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "win32.mak" CFG="win32 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "win32 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "win32 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "win32 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "__WIN32__" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib /nologo /subsystem:console /profile /debug /machine:I386 /out:"win32.exe"

!ELSEIF  "$(CFG)" == "win32 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "__WIN32__" /D "_AFX_NOFORCE_LIBS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"nafxcwd.lib" /pdbtype:sept
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "win32 - Win32 Release"
# Name "win32 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\rtsp_stack\AMRAudioRTPSink.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\AMRAudioSource.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\AudioRTPSink.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\BasicHashTable.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\BasicTaskScheduler.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\BasicTaskScheduler0.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\BasicUDPSink.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\BasicUsageEnvironment.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\BasicUsageEnvironment0.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\DelayQueue.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\DigestAuthentication.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\DvrWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\EncoderMediaSubsession.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\EncoderSource.cpp
# End Source File
# Begin Source File

SOURCE=..\CrashReport\ExceptionHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\FramedFilter.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\FramedSource.cpp
# End Source File
# Begin Source File

SOURCE=..\CrashReport\GetWinVer.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\GroupEId.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\Groupsock.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\GroupsockHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\HashTable.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\HikData.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\inet.c
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\Locale.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\mainSdk.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\Media.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\MediaSink.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\MediaSource.cpp
# End Source File
# Begin Source File

SOURCE=..\CrashReport\MiniVersion.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\MPEG4ESVideoRTPSink.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\MPEG4VideoStreamFramer.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\MPEGVideoStreamFramer.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\MPEGVideoStreamParser.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\MultiFramedRTPSink.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\NetAddress.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\NetInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\our_md5.c
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\our_md5hl.c
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\rsa_crypto.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\RTCP.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\rtcp_from_spec.c
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\RTPInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\RTPSink.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\RTPSource.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\RTSPCommon.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\RTSPEncoderSession.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\RTSPOverHTTPServer.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\RTSPSdk.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\RTSPServer.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\ServerMediaSession.cpp
# End Source File
# Begin Source File

SOURCE=..\CrashReport\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\strDup.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\StreamParser.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\UsageEnvironment.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\VideoRTPSink.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\xvid_interface.cpp
# End Source File
# Begin Source File

SOURCE=..\lib\libxvidcore.lib
# End Source File
# Begin Source File

SOURCE=..\lib\libeay32.lib
# End Source File
# Begin Source File

SOURCE=..\Hik\XClientDLL.lib
# End Source File
# Begin Source File

SOURCE=..\lib\dbghelp.lib
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
