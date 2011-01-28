/**********
This library is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the
Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version. (See <http://www.gnu.org/copyleft/lesser.html>.)

This library is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
**********/
// Copyright (c) 2006-2011, star valley, Inc.  All rights reserved
// A RTSP client Common program that opens a RTSP URL argument,
// and extracts the data from each incoming RTP stream.

#include "soapH.h"
#include "BasicUsageEnvironment.h"
#include "RTSPServer.h"
#include "MPEG4ESVideoRTPSink.h"
#include "RTCP.h"
#include "EncoderMediaSubsession.h"
#include "EncoderSource.h"
#include "RTSPCommon.h"
#include "RTSPClient.h"
#include "playCommon.h"
#include "GroupsockHelper.h"
#ifndef __WIN32__
#include <netdb.h>
#include <sys/socket.h>
#endif

#ifdef __WIN32__
#include <iostream.h>
#else
#include <iostream>
#endif

#include "rsa_crypto.h"
#include "RTSPSdk.h"
#include "Debug.h"
#include <stdio.h>
//#include "../CrashReport/ExceptionHandler.h"
UsageEnvironment* env;

#ifdef SDKH264
H264VideoRTPSink *videoSink;
#else
MPEG4ESVideoRTPSink* videoSink;
#endif

#include "stdsoap2.h"
// Forward function definitions:
void continueAfterOPTIONS(RTSPClient* client, int resultCode, char* resultString);
void continueAfterDESCRIBE(RTSPClient* client, int resultCode, char* resultString);
void continueAfterSETUP(RTSPClient* client, int resultCode, char* resultString);
void continueAfterPLAY(RTSPClient* client, int resultCode, char* resultString);
void continueAfterTEARDOWN(RTSPClient* client, int resultCode, char* resultString);

void setupStreams();
void closeMediaSinks();
void subsessionAfterPlaying(void* clientData);
void subsessionByeHandler(void* clientData);
void sessionAfterPlaying(void* clientData = NULL);
void sessionTimerHandler(void* clientData);
void shutdown(int exitCode = 1);
void signalHandlerShutdown(int sig);
void checkForPacketArrival(void* clientData);
void checkInterPacketGaps(void* clientData);
void beginQOSMeasurement();
static void periodicQOSMeasurement(void* clientData); // forward

#define  MAX_LISTDEVICERETURN 200
portNumBits tunnelOverHTTPPortNum = 0;
Authenticator* ourAuthenticator = NULL;
static char streamWebURL[1024] = {0};
#if 0
typedef struct _webServerGetURL
{
	unsigned char camId;
	char *streamWapURL;
	char *streamWabURL;
	char *streamRecordURL;
}webServerGetURL, *pWebServerGetURL;
#endif

Medium* ourClient = NULL;
int verbosityLevel = 1; // by default, print verbose output
char const* progName;
Boolean sendOptionsRequest = True;
Boolean sendOptionsRequestOnly = False;
extern char const* clientProtocolName;
MediaSession* session = NULL;
MediaSubsession *subsession;
Boolean madeProgress = False;
TaskToken sessionTimerTask = NULL;
TaskToken arrivalCheckTimerTask = NULL;
TaskToken interPacketGapCheckTimerTask = NULL;
TaskToken qosMeasurementTimerTask = NULL;
Boolean createReceivers = True;
Boolean outputQuickTimeFile = False;
Boolean generateMP4Format = False;
//QuickTimeFileSink* qtOut = NULL;
Boolean outputAVIFile = False;
//AVIFileSink* aviOut = NULL;
Boolean audioOnly = False;
Boolean videoOnly = False;
char const* singleMedium = NULL;
double duration = 0;
double durationSlop = -1.0; // extra seconds to play at the end
double initialSeekTime = 0.0f;
float scale = 1.0f;
double endTime;
unsigned interPacketGapMaxTime = 0;
unsigned totNumPacketsReceived = ~0; // used if checking inter-packet gaps
Boolean playContinuously = False;
int simpleRTPoffsetArg = -1;
Boolean oneFilePerFrame = False;
Boolean notifyOnPacketArrival = False;
Boolean streamUsingTCP = False;
unsigned short desiredPortNum = 0;
char* username = NULL;
char* password = NULL;
char* proxyServerName = NULL;
unsigned short proxyServerPortNum = 0;
unsigned char desiredAudioRTPPayloadFormat = 0;
char* mimeSubtype = NULL;
unsigned short movieWidth = 240; // default
Boolean movieWidthOptionSet = False;
unsigned short movieHeight = 180; // default
Boolean movieHeightOptionSet = False;
unsigned movieFPS = 15; // default
Boolean movieFPSOptionSet = False;
char const* fileNamePrefix = "";
unsigned fileSinkBufferSize = 100000;
unsigned socketInputBufferSize = 0;
Boolean packetLossCompensate = False;
Boolean syncStreams = False;
Boolean generateHintTracks = False;
unsigned qosMeasurementIntervalMS = 0; // 0 means: Don't output QOS data
int shutdownExitCode;
static unsigned nextQOSMeasurementUSecs;
struct timeval startTime;

int getURLByWebserver1(ns1__GetDevCamerasUrls &fDevCameras, ns1__GetDevCamerasUrlsResponse &fDevCamerasResponse, char *requestURL)
{

	struct soap soap;
	//	soap_ssl_init();
	soap_init(&soap);
#if 0
	if (soap_ssl_client_context(&soap,
		SOAP_SSL_NO_AUTHENTICATION, 	/* use SOAP_SSL_DEFAULT in production code */
		NULL, 		/* keyfile: required only when client must authenticate to server (see SSL docs on how to obtain this file) */
		NULL, 		/* password to read the keyfile */
		NULL,		/* optional cacert file to store trusted certificates */
		NULL,		/* optional capath to directory with trusted certificates */
		NULL		/* if randfile!=NULL: use a file with random data to seed randomness */ 
		))
	{ 
		soap_print_fault(&soap, stderr);
		exit(1);
	}
#endif
	char webServiceURL[0xff]={0};
	char host[128]={0};
	unsigned short port;
	getWebServerHostInfo(host, &port);
	snprintf(webServiceURL, 0xff, "%s%s:%d%s", "http://",host, port, "/ws/sdk/webservice_sdk.php?wsdl");
	if (soap_call_ns1__GetDevCamerasUrls(&soap, webServiceURL, NULL, fDevCameras.UserName, fDevCameras.Password,
		fDevCameras.MAC, fDevCameras.CamId, fDevCamerasResponse) == 0)
	{
		//printf("device mac %s \n", fDevCameras.MAC.c_str());
		for(int i=0; i<fDevCamerasResponse.GetDevCamerasUrlsReturn->urlresult->__size; i++)
		{
			break;	
		}
		printf("%s\n", fDevCamerasResponse.GetDevCamerasUrlsReturn->urlresult->__ptr[0]->weburl.c_str());
		strcpy(requestURL, fDevCamerasResponse.GetDevCamerasUrlsReturn->urlresult->__ptr[0]->weburl.c_str());
		//printf("%s\n", fDevCamerasResponse.GetDevCamerasUrlsReturn->urlresult->__ptr[1]->weburl.c_str());
		//printf("%s\n", fDevCamerasResponse.GetDevCamerasUrlsReturn->urlresult->__ptr[2]->weburl.c_str());
		//printf("%s\n", fDevCamerasResponse.GetDevCamerasUrlsReturn->urlresult->__ptr[3]->weburl.c_str());
	}
	else
	{
		soap_print_fault(&soap, stderr);
	}
	soap_end(&soap);
	return TRUE;
}

void continueAfterSETUP(RTSPClient*, int resultCode, char* resultString) {
	if (resultCode == 0) {
		*env << "Setup \"" << subsession->mediumName()
			<< "/" << subsession->codecName()
			<< "\" subsession (client ports " << subsession->clientPortNum()
			<< "-" << subsession->clientPortNum()+1 << ")\n";
		madeProgress = True;
	} else {
		*env << "Failed to setup \"" << subsession->mediumName()
			<< "/" << subsession->codecName()
			<< "\" subsession: " << env->getResultMsg() << "\n";
	}
	
	// Set up the next subsession, if any:
	setupStreams();
}

void setupStreams() {
  static MediaSubsessionIterator* setupIter = NULL;
  if (setupIter == NULL) setupIter = new MediaSubsessionIterator(*session);
  while ((subsession = setupIter->next()) != NULL) {
    // We have another subsession left to set up:
    if (subsession->clientPortNum() == 0) continue; // port # was not set

    setupSubsession(subsession, streamUsingTCP, continueAfterSETUP);
    return;
  }
  delete setupIter;
  if (!madeProgress) shutdown();
  if (createReceivers) {
    if (outputQuickTimeFile) {
      // Create a "QuickTimeFileSink", to write to 'stdout':
//       qtOut = QuickTimeFileSink::createNew(*env, *session, "stdout",
// 					   fileSinkBufferSize,
// 					   movieWidth, movieHeight,
// 					   movieFPS,
// 					   packetLossCompensate,
// 					   syncStreams,
// 					   generateHintTracks,
// 					   generateMP4Format);
//       if (qtOut == NULL) {
// 	*env << "Failed to create QuickTime file sink for stdout: " << env->getResultMsg();
// 	shutdown();
//       }

     /* qtOut->startPlaying(sessionAfterPlaying, NULL);*/
    } else if (outputAVIFile) {
      // Create an "AVIFileSink", to write to 'stdout':
//       aviOut = AVIFileSink::createNew(*env, *session, "stdout",
// 				      fileSinkBufferSize,
// 				      movieWidth, movieHeight,
// 				      movieFPS,
// 				      packetLossCompensate);
//       if (aviOut == NULL) {
// 	*env << "Failed to create AVI file sink for stdout: " << env->getResultMsg();
// 	shutdown();
//       }
// 
//       aviOut->startPlaying(sessionAfterPlaying, NULL);
    } else {
      // Create and start "FileSink"s for each subsession:
      madeProgress = False;
      MediaSubsessionIterator iter(*session);
      while ((subsession = iter.next()) != NULL) {
	if (subsession->readSource() == NULL) continue; // was not initiated

	// Create an output file for each desired stream:
	char outFileName[1000];
	if (singleMedium == NULL) {
	  // Output file name is
	  //     "<filename-prefix><medium_name>-<codec_name>-<counter>"
	  static unsigned streamCounter = 0;
	  snprintf(outFileName, sizeof outFileName, "%s%s-%s-%d",
		   fileNamePrefix, subsession->mediumName(),
		   subsession->codecName(), ++streamCounter);
	} else {
	  sprintf(outFileName, "stdout");
	}
//	FileSink* fileSink;
	if (strcmp(subsession->mediumName(), "audio") == 0 &&
	    (strcmp(subsession->codecName(), "AMR") == 0 ||
	     strcmp(subsession->codecName(), "AMR-WB") == 0)) {
	  // For AMR audio streams, we use a special sink that inserts AMR frame hdrs:
// 	  fileSink = AMRAudioFileSink::createNew(*env, outFileName,
// 						 fileSinkBufferSize, oneFilePerFrame);
	} else if (strcmp(subsession->mediumName(), "video") == 0 &&
	    (strcmp(subsession->codecName(), "H264") == 0)) {
	  // For H.264 video stream, we use a special sink that insert start_codes:
// 	  fileSink = H264VideoFileSink::createNew(*env, outFileName,
// 						 fileSinkBufferSize, oneFilePerFrame);
	} else {
	  // Normal case:
// 	  fileSink = FileSink::createNew(*env, outFileName,
// 					 fileSinkBufferSize, oneFilePerFrame);
	}
//	subsession->sink = fileSink;
	if (subsession->sink == NULL) {
	  *env << "Failed to create FileSink for \"" << outFileName
		  << "\": " << env->getResultMsg() << "\n";
	} else {
	  if (singleMedium == NULL) {
	    *env << "Created output file: \"" << outFileName << "\"\n";
	  } else {
	    *env << "Outputting data from the \"" << subsession->mediumName()
			<< "/" << subsession->codecName()
			<< "\" subsession to 'stdout'\n";
	  }

	  if (strcmp(subsession->mediumName(), "video") == 0 &&
	      strcmp(subsession->codecName(), "MP4V-ES") == 0 &&
	      subsession->fmtp_config() != NULL) {
	    // For MPEG-4 video RTP streams, the 'config' information
	    // from the SDP description contains useful VOL etc. headers.
	    // Insert this data at the front of the output file:
// 	    unsigned configLen;
// 	    unsigned char* configData
// 	      = parseGeneralConfigStr(subsession->fmtp_config(), configLen);
// 	    struct timeval timeNow;
// 	    gettimeofday(&timeNow, NULL);
// 	    fileSink->addData(configData, configLen, timeNow);
// 	    delete[] configData;
	  }

	  subsession->sink->startPlaying(*(subsession->readSource()),
					 subsessionAfterPlaying,
					 subsession);

	  // Also set a handler to be called if a RTCP "BYE" arrives
	  // for this subsession:
	  if (subsession->rtcpInstance() != NULL) {
	    subsession->rtcpInstance()->setByeHandler(subsessionByeHandler,
						      subsession);
	  }

	  madeProgress = True;
	}
      }
     // if (!madeProgress) shutdown();
    }
  }

  // Finally, start playing each subsession, to start the data flow:
  if (duration == 0) {
    if (scale > 0) duration = session->playEndTime() - initialSeekTime; // use SDP end time
    else if (scale < 0) duration = initialSeekTime;
  }
  if (duration < 0) duration = 0.0;

  endTime = initialSeekTime;
  if (scale > 0) {
    if (duration <= 0) endTime = -1.0f;
    else endTime = initialSeekTime + duration;
  } else {
    endTime = initialSeekTime - duration;
    if (endTime < 0) endTime = 0.0f;
  }

  startPlayingSession(session, initialSeekTime, endTime, scale, continueAfterPLAY);
}

void subsessionByeHandler(void* clientData) {
	struct timeval timeNow;
	gettimeofday(&timeNow, NULL);
	unsigned secsDiff = timeNow.tv_sec - startTime.tv_sec;
	
	MediaSubsession* subsession = (MediaSubsession*)clientData;
	*env << "Received RTCP \"BYE\" on \"" << subsession->mediumName()
		<< "/" << subsession->codecName()
		<< "\" subsession (after " << secsDiff
		<< " seconds)\n";
	
	// Act now as if the subsession had closed:
	subsessionAfterPlaying(subsession);
}

void subsessionAfterPlaying(void* clientData) {
	// Begin by closing this media subsession's stream:
	MediaSubsession* subsession = (MediaSubsession*)clientData;
	Medium::close(subsession->sink);
	subsession->sink = NULL;
	
	// Next, check whether *all* subsessions' streams have now been closed:
	MediaSession& session = subsession->parentSession();
	MediaSubsessionIterator iter(session);
	while ((subsession = iter.next()) != NULL) {
		if (subsession->sink != NULL) return; // this subsession is still active
	}
	
	// All subsessions' streams have now been closed
	sessionAfterPlaying();
}
void continueAfterTEARDOWN(RTSPClient*, int /*resultCode*/, char* /*resultString*/) {
	// Now that we've stopped any more incoming data from arriving, close our output files:
	closeMediaSinks();
	Medium::close(session);
	
	// Finally, shut down our client:
	delete ourAuthenticator;
	Medium::close(ourClient);
	
	// Adios...
	exit(shutdownExitCode);
}

void sessionAfterPlaying(void* /*clientData*/) {
	if (!playContinuously) {
		shutdown(0);
	} else {
		// We've been asked to play the stream(s) over again:
		startPlayingSession(session, initialSeekTime, endTime, scale, continueAfterPLAY);
	}
}

void shutdown(int exitCode) {
	shutdownExitCode = exitCode;
	if (env != NULL) {
		env->taskScheduler().unscheduleDelayedTask(sessionTimerTask);
		env->taskScheduler().unscheduleDelayedTask(arrivalCheckTimerTask);
		env->taskScheduler().unscheduleDelayedTask(interPacketGapCheckTimerTask);
		env->taskScheduler().unscheduleDelayedTask(qosMeasurementTimerTask);
	}
	
	if (qosMeasurementIntervalMS > 0) {
		//printQOSData(exitCode);
	}
	
	// Teardown, then shutdown, any outstanding RTP/RTCP subsessions
	if (session != NULL) {
		tearDownSession(session, continueAfterTEARDOWN);
	} else {
		continueAfterTEARDOWN(NULL, 0, NULL);
	}
}

void continueAfterPLAY(RTSPClient*, int resultCode, char* resultString) {
	if (resultCode != 0) {
		*env << "Failed to start playing session: " << resultString << "\n";
		shutdown();
	} else {
		*env << "Started playing session\n";
	}
	
	if (qosMeasurementIntervalMS > 0) {
		// Begin periodic QOS measurements:
		beginQOSMeasurement();
	}
	
	// Figure out how long to delay (if at all) before shutting down, or
	// repeating the playing
	Boolean timerIsBeingUsed = False;
	double secondsToDelay = duration;
	if (duration > 0) {
		timerIsBeingUsed = True;
		double absScale = scale > 0 ? scale : -scale; // ASSERT: scale != 0
		secondsToDelay = duration/absScale + durationSlop;
		
		int64_t uSecsToDelay = (int64_t)(secondsToDelay*1000000.0);
		sessionTimerTask = env->taskScheduler().scheduleDelayedTask(uSecsToDelay, (TaskFunc*)sessionTimerHandler, (void*)NULL);
	}
	
	char const* actionString
		= createReceivers? "Receiving streamed data":"Data is being streamed";
	if (timerIsBeingUsed) {
		*env << actionString
			<< " (for up to " << secondsToDelay
			<< " seconds)...\n";
	} else {
#ifdef USE_SIGNALS
		pid_t ourPid = getpid();
		*env << actionString
			<< " (signal with \"kill -HUP " << (int)ourPid
			<< "\" or \"kill -USR1 " << (int)ourPid
			<< "\" to terminate)...\n";
#else
		*env << actionString << "...\n";
#endif
	}
	
	// Watch for incoming packets (if desired):
	checkForPacketArrival(NULL);
	checkInterPacketGaps(NULL);
}

class qosMeasurementRecord {
public:
  qosMeasurementRecord(struct timeval const& startTime, RTPSource* src)
    : fSource(src), fNext(NULL),
      kbits_per_second_min(1e20), kbits_per_second_max(0),
      kBytesTotal(0.0),
      packet_loss_fraction_min(1.0), packet_loss_fraction_max(0.0),
      totNumPacketsReceived(0), totNumPacketsExpected(0) {
    measurementEndTime = measurementStartTime = startTime;

    RTPReceptionStatsDB::Iterator statsIter(src->receptionStatsDB());
    // Assume that there's only one SSRC source (usually the case):
    RTPReceptionStats* stats = statsIter.next(True);
    if (stats != NULL) {
      kBytesTotal = stats->totNumKBytesReceived();
      totNumPacketsReceived = stats->totNumPacketsReceived();
      totNumPacketsExpected = stats->totNumPacketsExpected();
    }
  }
  virtual ~qosMeasurementRecord() { delete fNext; }

  void periodicQOSMeasurement(struct timeval const& timeNow);

public:
  RTPSource* fSource;
  qosMeasurementRecord* fNext;

public:
  struct timeval measurementStartTime, measurementEndTime;
  double kbits_per_second_min, kbits_per_second_max;
  double kBytesTotal;
  double packet_loss_fraction_min, packet_loss_fraction_max;
  unsigned totNumPacketsReceived, totNumPacketsExpected;
};

static qosMeasurementRecord* qosRecordHead = NULL;

static void periodicQOSMeasurement(void* clientData); // forward

static void scheduleNextQOSMeasurement() {
  nextQOSMeasurementUSecs += qosMeasurementIntervalMS*1000;
  struct timeval timeNow;
  gettimeofday(&timeNow, NULL);
  unsigned timeNowUSecs = timeNow.tv_sec*1000000 + timeNow.tv_usec;
  unsigned usecsToDelay = nextQOSMeasurementUSecs - timeNowUSecs;
     // Note: This works even when nextQOSMeasurementUSecs wraps around

  qosMeasurementTimerTask = env->taskScheduler().scheduleDelayedTask(
     usecsToDelay, (TaskFunc*)periodicQOSMeasurement, (void*)NULL);
}

static void periodicQOSMeasurement(void* /*clientData*/) {
  struct timeval timeNow;
  gettimeofday(&timeNow, NULL);

  for (qosMeasurementRecord* qosRecord = qosRecordHead;
       qosRecord != NULL; qosRecord = qosRecord->fNext) {
    qosRecord->periodicQOSMeasurement(timeNow);
  }

  // Do this again later:
  scheduleNextQOSMeasurement();
}

void qosMeasurementRecord
::periodicQOSMeasurement(struct timeval const& timeNow) {
  unsigned secsDiff = timeNow.tv_sec - measurementEndTime.tv_sec;
  int usecsDiff = timeNow.tv_usec - measurementEndTime.tv_usec;
  double timeDiff = secsDiff + usecsDiff/1000000.0;
  measurementEndTime = timeNow;

  RTPReceptionStatsDB::Iterator statsIter(fSource->receptionStatsDB());
  // Assume that there's only one SSRC source (usually the case):
  RTPReceptionStats* stats = statsIter.next(True);
  if (stats != NULL) {
    double kBytesTotalNow = stats->totNumKBytesReceived();
    double kBytesDeltaNow = kBytesTotalNow - kBytesTotal;
    kBytesTotal = kBytesTotalNow;

    double kbpsNow = timeDiff == 0.0 ? 0.0 : 8*kBytesDeltaNow/timeDiff;
    if (kbpsNow < 0.0) kbpsNow = 0.0; // in case of roundoff error
    if (kbpsNow < kbits_per_second_min) kbits_per_second_min = kbpsNow;
    if (kbpsNow > kbits_per_second_max) kbits_per_second_max = kbpsNow;

    unsigned totReceivedNow = stats->totNumPacketsReceived();
    unsigned totExpectedNow = stats->totNumPacketsExpected();
    unsigned deltaReceivedNow = totReceivedNow - totNumPacketsReceived;
    unsigned deltaExpectedNow = totExpectedNow - totNumPacketsExpected;
    totNumPacketsReceived = totReceivedNow;
    totNumPacketsExpected = totExpectedNow;

    double lossFractionNow = deltaExpectedNow == 0 ? 0.0
      : 1.0 - deltaReceivedNow/(double)deltaExpectedNow;
    //if (lossFractionNow < 0.0) lossFractionNow = 0.0; //reordering can cause
    if (lossFractionNow < packet_loss_fraction_min) {
      packet_loss_fraction_min = lossFractionNow;
    }
    if (lossFractionNow > packet_loss_fraction_max) {
      packet_loss_fraction_max = lossFractionNow;
    }
  }
}

void beginQOSMeasurement() {
	// Set up a measurement record for each active subsession:
	struct timeval startTime;
	gettimeofday(&startTime, NULL);
	nextQOSMeasurementUSecs = startTime.tv_sec*1000000 + startTime.tv_usec;
	qosMeasurementRecord* qosRecordTail = NULL;
	MediaSubsessionIterator iter(*session);
	MediaSubsession* subsession;
	while ((subsession = iter.next()) != NULL) {
		RTPSource* src = subsession->rtpSource();
		if (src == NULL) continue;
		
		qosMeasurementRecord* qosRecord
			= new qosMeasurementRecord(startTime, src);
		if (qosRecordHead == NULL) qosRecordHead = qosRecord;
		if (qosRecordTail != NULL) qosRecordTail->fNext = qosRecord;
		qosRecordTail  = qosRecord;
	}
	
	// Then schedule the first of the periodic measurements:
	scheduleNextQOSMeasurement();
}

void continueAfterDESCRIBE(RTSPClient*, int resultCode, char* resultString) {
  if (resultCode != 0) {
    *env << "Failed to get a SDP description from URL \"" << streamWebURL << "\": " << resultString << "\n";
    shutdown();
  }

  char* sdpDescription = resultString;
  *env << "Opened URL \"" << streamWebURL << "\", returning a SDP description:\n" << sdpDescription << "\n";
  /*wayde*/
  char *configStrStart = NULL;
  char *configStrEnd = NULL;
  FILE *configfp;
   if (configStrStart = strstr(sdpDescription, "config="))
   {
	   configStrStart += strlen("config=");
	    if (configStrEnd = strstr(configStrStart, "\r\n"))
		{
			configfp = fopen("..//config.bin", "w");
			fwrite(configStrStart, 1, configStrEnd-configStrStart, configfp);
			fclose(configfp);
		}		
   }
   /*wayde*/
  // Create a media session object from this SDP description:
  session = MediaSession::createNew(*env, sdpDescription);
  delete[] sdpDescription;
  if (session == NULL) {
    *env << "Failed to create a MediaSession object from the SDP description: " << env->getResultMsg() << "\n";
    shutdown();
  } else if (!session->hasSubsessions()) {
    *env << "This session has no media subsessions (i.e., \"m=\" lines)\n";
    shutdown();
  }

  // Then, setup the "RTPSource"s for the session:
  MediaSubsessionIterator iter(*session);
  MediaSubsession *subsession;
  Boolean madeProgress = False;
  char const* singleMediumToTest = singleMedium;
  while ((subsession = iter.next()) != NULL) {
    // If we've asked to receive only a single medium, then check this now:
    if (singleMediumToTest != NULL) {
      if (strcmp(subsession->mediumName(), singleMediumToTest) != 0) {
		  *env << "Ignoring \"" << subsession->mediumName()
			  << "/" << subsession->codecName()
			  << "\" subsession, because we've asked to receive a single " << singleMedium
			  << " session only\n";
	continue;
      } else {
	// Receive this subsession only
	singleMediumToTest = "xxxxx";
	    // this hack ensures that we get only 1 subsession of this type
      }
    }

    if (desiredPortNum != 0) {
      subsession->setClientPortNum(desiredPortNum);
      desiredPortNum += 2;
    }

    if (createReceivers) {
      if (!subsession->initiate(simpleRTPoffsetArg)) {
	*env << "Unable to create receiver for \"" << subsession->mediumName()
	     << "/" << subsession->codecName()
	     << "\" subsession: " << env->getResultMsg() << "\n";
      } else {
	*env << "Created receiver for \"" << subsession->mediumName()
	     << "/" << subsession->codecName()
	     << "\" subsession (client ports " << subsession->clientPortNum()
	     << "-" << subsession->clientPortNum()+1 << ")\n";
	madeProgress = True;
	
	if (subsession->rtpSource() != NULL) {
	  // Because we're saving the incoming data, rather than playing
	  // it in real time, allow an especially large time threshold
	  // (1 second) for reordering misordered incoming packets:
	  unsigned const thresh = 1000000; // 1 second
	  subsession->rtpSource()->setPacketReorderingThresholdTime(thresh);
	  
	  // Set the RTP source's OS socket buffer size as appropriate - either if we were explicitly asked (using -B),
	  // or if the desired FileSink buffer size happens to be larger than the current OS socket buffer size.
	  // (The latter case is a heuristic, on the assumption that if the user asked for a large FileSink buffer size,
	  // then the input data rate may be large enough to justify increasing the OS socket buffer size also.)
	  int socketNum = subsession->rtpSource()->RTPgs()->socketNum();
	  unsigned curBufferSize = getReceiveBufferSize(*env, socketNum);
	  if (socketInputBufferSize > 0 || fileSinkBufferSize > curBufferSize) {
	    unsigned newBufferSize = socketInputBufferSize > 0 ? socketInputBufferSize : fileSinkBufferSize;
	    newBufferSize = setReceiveBufferTo(*env, socketNum, newBufferSize);
	    if (socketInputBufferSize > 0) { // The user explicitly asked for the new socket buffer size; announce it:
	      *env << "Changed socket receive buffer size for the \""
		   << subsession->mediumName()
		   << "/" << subsession->codecName()
		   << "\" subsession from "
		   << curBufferSize << " to "
		   << newBufferSize << " bytes\n";
	    }
	  }
	}
      }
    } else {
      if (subsession->clientPortNum() == 0) {
	*env << "No client port was specified for the \""
	     << subsession->mediumName()
	     << "/" << subsession->codecName()
	     << "\" subsession.  (Try adding the \"-p <portNum>\" option.)\n";
      } else {
		madeProgress = True;
      }
    }
  }
  //if (!madeProgress) shutdown();

  // Perform additional 'setup' on each subsession, before playing them:
  setupStreams();
}

void continueAfterOPTIONS(RTSPClient* client, int resultCode, char* resultString) {
	if (sendOptionsRequestOnly) {
		if (resultCode != 0) {
			*env << clientProtocolName << " \"OPTIONS\" request failed: " << resultString << "\n";
		} else {
			*env << clientProtocolName << " \"OPTIONS\" request returned: " << resultString << "\n";
		}
		//shutdown();
	}
	delete[] resultString;
	
	// Next, get a SDP description for the stream:
	getSDPDescription(continueAfterDESCRIBE);
}
void checkForPacketArrival(void* /*clientData*/) {
	if (!notifyOnPacketArrival) return; // we're not checking
	
	// Check each subsession, to see whether it has received data packets:
	unsigned numSubsessionsChecked = 0;
	unsigned numSubsessionsWithReceivedData = 0;
	unsigned numSubsessionsThatHaveBeenSynced = 0;
	
	MediaSubsessionIterator iter(*session);
	MediaSubsession* subsession;
	while ((subsession = iter.next()) != NULL) {
		RTPSource* src = subsession->rtpSource();
		if (src == NULL) continue;
		++numSubsessionsChecked;
		
		if (src->receptionStatsDB().numActiveSourcesSinceLastReset() > 0) {
			// At least one data packet has arrived
			++numSubsessionsWithReceivedData;
		}
		if (src->hasBeenSynchronizedUsingRTCP()) {
			++numSubsessionsThatHaveBeenSynced;
		}
	}
	
	unsigned numSubsessionsToCheck = numSubsessionsChecked;
	// Special case for "QuickTimeFileSink"s and "AVIFileSink"s:
	// They might not use all of the input sources:
// 	if (qtOut != NULL) {
// 		numSubsessionsToCheck = qtOut->numActiveSubsessions();
// 	} else if (aviOut != NULL) {
// 		numSubsessionsToCheck = aviOut->numActiveSubsessions();
// 	}
	
	Boolean notifyTheUser;
	if (!syncStreams) {
		notifyTheUser = numSubsessionsWithReceivedData > 0; // easy case
	} else {
		notifyTheUser = numSubsessionsWithReceivedData >= numSubsessionsToCheck
			&& numSubsessionsThatHaveBeenSynced == numSubsessionsChecked;
		// Note: A subsession with no active sources is considered to be synced
	}
	if (notifyTheUser) {
		struct timeval timeNow;
		gettimeofday(&timeNow, NULL);
		char timestampStr[100];
		sprintf(timestampStr, "%ld%03ld", timeNow.tv_sec, (long)(timeNow.tv_usec/1000));
		*env << (syncStreams ? "Synchronized d" : "D")
			<< "ata packets have begun arriving [" << timestampStr << "]\007\n";
		return;
	}
	
	// No luck, so reschedule this check again, after a delay:
	int uSecsToDelay = 100000; // 100 ms
	arrivalCheckTimerTask
		= env->taskScheduler().scheduleDelayedTask(uSecsToDelay,
		(TaskFunc*)checkForPacketArrival, NULL);
}

void closeMediaSinks() {
//	Medium::close(qtOut);
//	Medium::close(aviOut);
	
	if (session == NULL) return;
	MediaSubsessionIterator iter(*session);
	MediaSubsession* subsession;
	while ((subsession = iter.next()) != NULL) {
		Medium::close(subsession->sink);
		subsession->sink = NULL;
	}
}

void sessionTimerHandler(void* /*clientData*/) {
	sessionTimerTask = NULL;
	
	sessionAfterPlaying();
}

void checkInterPacketGaps(void* /*clientData*/) {
	if (interPacketGapMaxTime == 0) return; // we're not checking
	
	// Check each subsession, counting up how many packets have been received:
	unsigned newTotNumPacketsReceived = 0;
	
	MediaSubsessionIterator iter(*session);
	MediaSubsession* subsession;
	while ((subsession = iter.next()) != NULL) {
		RTPSource* src = subsession->rtpSource();
		if (src == NULL) continue;
		newTotNumPacketsReceived += src->receptionStatsDB().totNumPacketsReceived();
	}
	
	if (newTotNumPacketsReceived == totNumPacketsReceived) {
		// No additional packets have been received since the last time we
		// checked, so end this stream:
		*env << "Closing session, because we stopped receiving packets.\n";
		interPacketGapCheckTimerTask = NULL;
		sessionAfterPlaying();
	} else {
		totNumPacketsReceived = newTotNumPacketsReceived;
		// Check again, after the specified delay:
		interPacketGapCheckTimerTask
			= env->taskScheduler().scheduleDelayedTask(interPacketGapMaxTime*1000000,
			(TaskFunc*)checkInterPacketGaps, NULL);
	}
}

void getURLByWebserver(char *usrName, char *password, char *mac, unsigned char camId, char *webURL)
{
	ns1__GetDevCamerasUrls devCameras;
	ns1__GetDevCamerasUrlsResponse devCamerasResponse;
	devCameras.UserName = usrName;
	devCameras.Password = password;
	devCameras.MAC = mac;
	devCameras.CamId = camId;
	getURLByWebserver1(devCameras, devCamerasResponse, webURL);
}

void initBasicUsageEnvironment()
{
	TaskScheduler* scheduler = BasicTaskScheduler::createNew();
	env = BasicUsageEnvironment::createNew(*scheduler);
	
}

void *startPlaying(char *usrName, char *password, char *mac, unsigned char camId)
{	
	void *ourClient = NULL;
	getURLByWebserver(usrName, password, mac, camId, streamWebURL);
	ourClient = createClient(*env, streamWebURL, verbosityLevel, progName);
	getOptions(continueAfterOPTIONS);
	return ourClient;
}

void registerFrameCallback(void *handle, void (CALLBACK *getFrameCallBack)(void *handle,char *pBuf,long nSize,long nWidth,long nHeight))
{
	registerClientStreamCallback(handle, getFrameCallBack);
}

void activeTaskSchedulerLoop()
{
	env->taskScheduler().doEventLoop(); // does not return
}

SOAP_NMAC struct Namespace namespaces[] =
{
	{"SOAP-ENV", "http://schemas.xmlsoap.org/soap/envelope/", "http://www.w3.org/*/soap-envelope", NULL},
	{"ns1", "urn:webservice_sdk", NULL, NULL},
	{"xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL},
	{"xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL},
	{"SOAP-ENC", "http://schemas.xmlsoap.org/soap/encoding/", "http://www.w3.org/*/soap-encoding", NULL},
	{NULL, NULL, NULL, NULL}
};
