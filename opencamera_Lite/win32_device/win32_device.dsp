# Microsoft Developer Studio Project File - Name="win32_device" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=win32_device - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "win32_device.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "win32_device.mak" CFG="win32_device - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "win32_device - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "win32_device - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "win32_device - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "win32_device - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ  /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "__WIN32__" /YX /FD /GZ  /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  wsock32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "win32_device - Win32 Release"
# Name "win32_device - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\rtsp_stack\AMRAudioRTPSink.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\AMRAudioRTPSink.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\AMRAudioSource.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\AMRAudioSource.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\AudioRTPSink.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\AudioRTPSink.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\Base64.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\Base64.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\BasicHashTable.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\BasicHashTable.h
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

SOURCE=..\rtsp_stack\BasicUDPSink.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\BasicUsageEnvironment.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\BasicUsageEnvironment.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\BasicUsageEnvironment0.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\BasicUsageEnvironment0.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\BasicUsageEnvironment_version.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\bn.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\Boolean.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\crypto.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\Debug.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\DelayQueue.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\DelayQueue.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\DigestAuthentication.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\DigestAuthentication.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\e_os.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\EncoderMediaSession.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\EncoderMediaSubsession.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\EncoderMediaSubsession.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\EncoderSource.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\EncoderSource.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\err.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\FramedFilter.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\FramedFilter.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\FramedSource.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\FramedSource.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\GroupEId.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\GroupEId.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\Groupsock.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\Groupsock.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\groupsock_version.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\GroupsockHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\GroupsockHelper.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\H264VideoRTPSink.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\H264VideoRTPSink.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\H264VideoRTPSource.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\H264VideoRTPSource.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\H264VideoStreamFramer.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\H264VideoStreamFramer.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\HandlerSet.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\HashTable.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\HashTable.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\httpHash.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\inet.c
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\IOHandlers.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\IOHandlers.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\liveMedia.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\liveMedia_version.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\Locale.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\Locale.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\mainSdk.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\Media.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\Media.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\MediaSession.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\MediaSink.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\MediaSink.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\MediaSource.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\MediaSource.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\MPEG4ESVideoRTPSink.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\MPEG4ESVideoRTPSink.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\MPEG4VideoStreamFramer.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\MPEG4VideoStreamFramer.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\MPEGVideoStreamFramer.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\MPEGVideoStreamFramer.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\MPEGVideoStreamParser.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\MPEGVideoStreamParser.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\MultiFramedRTPSink.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\MultiFramedRTPSink.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\MultiFramedRTPSource.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\MultiFramedRTPSource.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\NetAddress.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\NetAddress.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\NetCommon.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\NetInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\NetInterface.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\our_md5.c
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\our_md5.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\our_md5hl.c
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\rand.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\rsa.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\rsa_crypto.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\rsa_crypto.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\RTCP.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\RTCP.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\rtcp_from_spec.c
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\rtcp_from_spec.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\RTPInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\RTPInterface.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\RTPSink.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\RTPSink.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\RTPSource.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\RTPSource.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\RTSPCommon.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\RTSPCommon.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\RTSPEncoderSession.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\RTSPOverHTTPServer.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\RTSPOverHTTPServer.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\RTSPSdk.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\RTSPSdk.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\RTSPServer.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\RTSPServer.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\ServerMediaSession.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\ServerMediaSession.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\strDup.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\strDup.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\StreamParser.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\StreamParser.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\TunnelEncaps.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\UsageEnvironment.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\UsageEnvironment.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\UsageEnvironment_version.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\VideoRTPSink.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\VideoRTPSink.h
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\xvid_interface.cpp
# End Source File
# Begin Source File

SOURCE=..\rtsp_stack\xvid_interface.h
# End Source File
# Begin Source File

SOURCE=..\lib\libxvidcore.lib
# End Source File
# Begin Source File

SOURCE=..\lib\libeay32.lib
# End Source File
# Begin Source File

SOURCE=..\lib\libx264.lib
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
