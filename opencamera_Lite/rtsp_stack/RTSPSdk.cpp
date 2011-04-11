#include "RTSPServer.h"
#include "EncoderSource.h"
#include "RTSPCommon.h"
#include "GroupsockHelper.h"
#include "rsa_crypto.h"
#include "RTSPSdk.h"
#include "Debug.h"
#ifdef __WIN32__
#include <iostream.h>
#else
#include <iostream>
#endif
#if defined(__WIN32__) || defined(_WIN32) || defined(_QNX4)
#else
#include <signal.h>
#define USE_SIGNALS 1
#endif
#include <time.h> // for "strftime()" and "gmtime()"

#ifndef __WIN32__
using namespace std;
#endif

#ifdef SDKH264
#include "Base64.h"
#endif

#ifndef __WIN32__
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#endif


void getServerInfo(char *host, unsigned short *port, int defaultType)
{
	if(defaultType)
	{

	}
	else
	{
		memcpy(host, "219.235.235.3", sizeof("219.235.235.3"));
		//memcpy(host, "s.starv.tv", sizeof("s.starv.tv"));
		*port = 554;
	}
}

void getDeviceInfo(unsigned char channel, char *ip, unsigned short *port, char *mac)
{
	memcpy(ip, "192.168.10.131", sizeof("192.168.10.131"));
	*port = 554;
	memcpy(mac, "000102030455", sizeof("000102030455"));
}

void getHttpServerInfo(char *host, unsigned short *port)
{
	memcpy(host, "219.235.235.4", sizeof("219.235.235.4"));
	*port = 80;
}

void getListenPortByChannel(unsigned short *port, unsigned char channel)
{
	switch (channel)
	{
	case CAMERA_CHANNEL_1:
		*port = 8553+CAMERA_CHANNEL_1;
	case CAMERA_CHANNEL_2:
		*port = 8553+CAMERA_CHANNEL_2;
	}
}

void getUserAgent(char *ua_buf, int buf_len)
{
	memcpy(ua_buf, "STARVALLEY-PCSIMU1 0.1", sizeof("STARVALLEY-PCSIMU1 0.1"));
}

#define PLAY_FILE_DIR "../../IPCamera"

char  temp_key[257] = {"A9D860C8F2ECB94F959C75B28620A71E0D27B42B5989DCC2DDB99EB546E6A"
					   "7EBD0DAB90335A8B08F9E80DF6BAF38AAB363A3D4A7AA74A677F4661AF3E1"
					   "1F169E88A8D8CE911D36B50BDD79C921C38A4B8D06FBDD44F149239044DE1"
					   "FECB42AB22D1BFF8B1DA232430CEC62CD8208507E88E994A51BC92B6C2BC2"
					   "C8DE6A062357"};
void readkey(unsigned char channel, char *chCode)
{	
	memcpy(chCode, temp_key, strlen(temp_key));
}

void writekey(unsigned char channel, char const *chCode)
{
	//memcpy(temp_key, chCode, strlen((char const *)chCode));
}

void RTSPServer::RTSPEncoderSession
::readSerial(char *serial)
{

}

void RTSPServer::RTSPEncoderSession
::writeSerial(char const *serial)
{
	Debug(ckite_log_message, "serial = %s\n", serial);
}

void RTSPServer::RTSPEncoderSession
::writeActiveServerInfo(char const *host, unsigned short port)
{

}

#define CIF_CONFIG "\x00\x00\x01\xb0\x08\x00\x00\x01\xb5\x09\x00\x00\x01\x00\x00\x00\x01\x20\x00\x84\x40\xfa\x28\x58\x21\x20\xA3\x1f" // fix_vop_rate = 0 
#define H264CIF_SPS_CONFIG "\x67\x42\xc0\x0c\xf2\x02\xc1\x2d\x08\x00\x00\x03\x03\x20\x00\x00\x03\x00\10\x78\xa1\x52\x40"
#define H264CIF_PPS_CONFIG "\x68\xcb\x83\xcb\x20"
void getVideoCodecConfig(int width, int height, int *profile_level_id, unsigned char *config, unsigned int *config_length)
{
#ifdef SDKH264
	char *spsOut = NULL;
	char *ppsOut = NULL;
	*profile_level_id = 4366366;
	if(width == 352 && height == 288)	
	{
		spsOut = base64Encode((char *)H264CIF_SPS_CONFIG, 23);
		Debug(ckite_log_message, "spsOut = %s\n", spsOut);
		ppsOut = base64Encode((char *)H264CIF_PPS_CONFIG, 5);
		Debug(ckite_log_message, "ppsOut = %s\n", ppsOut);
		sprintf((char *)config, "%s,%s", spsOut, ppsOut);
	}
	*config_length = strlen((char const *)config);
	delete [] spsOut;
	delete [] ppsOut;
#else
	*profile_level_id = 8;
	if(width == 352 && height == 288)	
	{
		unsigned char *cifConfig = (unsigned char *)CIF_CONFIG;
		memcpy(config, cifConfig, 28);
		*config_length = 28;
	}
#endif
}

void RTSPServer::RTSPEncoderSession
::deviceHandleArmAndDisarmScene( int actionType)
{
	//scene
	switch(actionType)
	{
	case DISARM:
		Debug(ckite_log_message, "disarm\n");
		break;
	case ZONEARM:
		Debug(ckite_log_message, "zone-arm\n");
		break;
	case ACTIVEALARM:
		Debug(ckite_log_message, "active-alarm\n");
		break;
	case STOPALARM:
		Debug(ckite_log_message, "stop-alarm\n");
		break;
	default:
		Debug(ckite_log_message, "unknown  action\n");
	}
}

void RTSPServer::RTSPEncoderSession
::get_LanWebURL(char *p, int p_len)
{
	sprintf(p+p_len,"%s%s\r\n", "LanWebURL: ","http://192.168.10.131:80");
}

void RTSPServer::RTSPEncoderSession
::get_WanWebURL(char *p, int p_len)
{
	sprintf(p+p_len,"%s%s\r\n", "WanWebURL: ","http://[WANIP]:80");
}

void RTSPServer::RTSPEncoderSession
::get_P2PLanURL(char *p, int p_len)
{
	sprintf(p+p_len,"%s%s\r\n", "P2PLanURL: ","rtsp://192.168.10.131:8554/live");
}

void RTSPServer::RTSPEncoderSession
::get_P2PWanURL(char *p, int p_len)
{
	sprintf(p+p_len,"%s%s\r\n", "P2PWanURL: ","rtsp://[WANIP]:8150/live_video.sdp");
}

void RTSPServer::RTSPEncoderSession
::get_LanFtpURL(char *p, int p_len)
{
	sprintf(p+p_len,"%s%s\r\n", "LanFtpURL: ","ftp://192.168.10.131:21");
}

void RTSPServer::RTSPEncoderSession
::get_WanFtpURL(char *p, int p_len)
{
	sprintf(p+p_len,"%s%s\r\n", "WanFtpURL: ","ftp://192.168.10.102:3011");
}

void RTSPServer::RTSPEncoderSession
::get_LanVoiceAddr(char *p, int p_len)
{
	sprintf(p+p_len,"%s%s\r\n", "LanVoiceAddr: ","192.168.10.131:8000");
}

void RTSPServer::RTSPEncoderSession
::get_WanVoiceAddr(char *p, int p_len)
{
	sprintf(p+p_len,"%s%s\r\n", "WanVoiceAddr: ","192.168.10.102:15400");
}

void RTSPServer::RTSPEncoderSession
::get_LanDataAddr(char *p, int p_len)
{
	sprintf(p+p_len,"%s%s\r\n", "LanDataAddr: ","192.168.10.131:8000");
}

void RTSPServer::RTSPEncoderSession
::get_WanDataAddr(char *p, int p_len)
{
	sprintf(p+p_len,"%s%s\r\n", "WanDataAddr: ","192.168.10.102:15401");
}

void videoGetFrameInfo(unsigned char channel, void *handle, char* session_name, unsigned char *framebuf, int *framesize, int *videoType)
{
	enum videotype {VIDEO_RAW, VIDEO_MPEG4, VIDEO_H264};
	int width = 0, height = 0;
	FILE *fp = (FILE *)handle;
	int getFrameSize = *framesize;
	if (strcmp(session_name, "live") == 0)
	{
		width = 352;
		height = 288;
	}
	int n = fread(framebuf, 1, getFrameSize, fp);
	if (n != getFrameSize)
	{
		fseek(fp, SEEK_SET, 0);
		n = fread(framebuf, 1, getFrameSize, fp);
		if (n != getFrameSize)
		{
			Debug(ckite_log_message, "FATAL error: cannot read bytes from input file!\n");
			return;
		}
	}
	*framesize = getFrameSize;
	*videoType = VIDEO_RAW;
}

void audioGetFrameInfo(void * handle, char* session_name, char *framebuf, int *framesize, int*audioType)
{
	enum audiotype {AUDIO_RAW, AUDIO_AMRNB, AUDIO_AMRWB};
	int getFrameSize = *framesize;
	FILE *fp = (FILE *)handle;
	int n = fread(framebuf, 1, getFrameSize, fp);
	if (n != getFrameSize)
	{
		fseek(fp, SEEK_SET, 0);
		n = fread(framebuf, 1, getFrameSize, fp);
		if (n != getFrameSize)
		{
			Debug(ckite_log_message, "FATAL error: cannot read bytes from input file!\n");
			return;
		}
	}
	*framesize = getFrameSize;
	*audioType = AUDIO_RAW;
}

void getSubsessionParaConfig(char *session_name, int width, int height, unsigned int& video_bitrate, unsigned int& framerate, unsigned int& keyinterval,
																		unsigned int& audio_bitrate, unsigned int& framelength, unsigned int& samplerate)
{
	if ((strcmp(session_name, "live") == 0) && (width == 352) && (height == 288))	
	{
		video_bitrate = 120;
		framerate = 10;
		keyinterval = 5;
	}
	audio_bitrate = 5;
	framelength = 160;
	samplerate = 8000;
}