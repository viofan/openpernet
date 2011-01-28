CC = g++
H264 = undefined
FLAGS += -g
ifeq (defined, $(H264)) 
FLAGS += -DSDKH264
endif
LIBS = -lxvidcore -lssl -lpthread -lopencore-amrnb -lopencore-amrwb -lx264
OBJECTS = AMRAudioRTPSink.o	AMRAudioSource.o AudioRTPSink.o BasicHashTable.o 	\
		  BasicTaskScheduler.o BasicTaskScheduler0.o BasicUDPSink.o	BasicUsageEnvironment.o	\
		  BasicUsageEnvironment0.o DelayQueue.o DigestAuthentication.o \
		  EncoderMediaSubsession.o EncoderSource.o FramedFilter.o	\
		  FramedSource.o GroupEId.o	Groupsock.o	GroupsockHelper.o HashTable.o \
		  inet.o IOHandlers.o Locale.o Media.o MediaSink.o	\
		  MediaSource.o MPEG4ESVideoRTPSink.o MPEG4VideoStreamFramer.o MPEGVideoStreamFramer.o	\
	      MPEGVideoStreamParser.o MultiFramedRTPSink.o NetAddress.o	NetInterface.o	\
		  our_md5.o	our_md5hl.o	rsa_crypto.o RTCP.o rtcp_from_spec.o RTPInterface.o	\
		  RTPSink.o	RTPSource.o	RTSPCommon.o RTSPEncoderSession.o RTSPOverHTTPServer.o	\
		  RTSPServer.o ServerMediaSession.o strDup.o StreamParser.o mainSdk.o	\
		  UsageEnvironment.o VideoRTPSink.o	xvid_interface.o RTSPSdk.o	Base64.o
ifeq (defined, $(H264))
OBJECTS += H264VideoRTPSink.o H264VideoRTPSource.o H264VideoStreamFramer.o MultiFramedRTPSource.o
endif

rtsp_protocol: $(OBJECTS)
	$(CC) -o $@  $^ $(LIBS) 
.cpp.o:
	$(CC) -c $(FLAGS) $<
.PHONY clean:
clean: 
	-rm rtsp_protocol $(OBJECTS) 
