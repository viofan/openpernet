#include "RTSPServer.h"
#include "EncoderSource.h"
#include "RTSPCommon.h"
#include "GroupsockHelper.h"
#include "rsa_crypto.h"
#include "RTSPSdk.h"
#include "Debug.h"
//#include "HikData.h"
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
		memcpy(host, "219.235.235.47", sizeof("219.235.235.47"));
		//memcpy(host, "s.starv.tv", sizeof("s.starv.tv"));
		*port = 554;
	}
}

void getDeviceInfo(unsigned char channel, char *ip, unsigned short *port, char *mac)
{
#if 1
// 	LPhikProfile proInfo = getProfileParamenterInfo();
// 	memcpy(ip, proInfo->winServerIP, strlen(proInfo->winServerIP));
// 	*port = proInfo->winPort;
// 	memcpy(mac, proInfo->deviceMAC, strlen(proInfo->deviceMAC));
#endif
}

void getHttpServerInfo(char *host, unsigned short *port)
{
	memcpy(host, "219.235.235.41", sizeof("219.235.235.41"));
	*port = 80;
}

void getListenPortByChannel(unsigned short *port, unsigned char channel)
{
#if 0
	LPhikProfile proInfo = getProfileParamenterInfo();
	*port = proInfo->winListenPort;
#endif
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
	memcpy(ua_buf, "YingHe-SDK 1.0", sizeof("YingHe-SDK 1.0"));
}

#define PLAY_FILE_DIR "../../IPCamera"

char  temp_key[257] = {"A9D860C8F2ECB94F959C75B28620A71E0D27B42B5989DCC2DDB99EB546E6A7EBD0DAB90335A8B08F9E80DF6BAF38AAB363A3D4A7AA74A677F4661AF3E11F169E88A8D8CE911D36B50BDD79C921C38A4B8D06FBDD44F149239044DE1FECB42AB22D1BFF8B1DA232430CEC62CD8208507E88E994A51BC92B6C2BC2C8DE6A062357"};

void readkey(unsigned char channel, char *chCode)
{
#if 1
	memcpy(chCode, temp_key, strlen(temp_key));
#else
	char challengeString[60] = {0};
	char temp[257] = {0};
	int count = 0;
	LPhikProfile proInfo = getProfileParamenterInfo();
	snprintf(challengeString, sizeof(challengeString), "..//exec//challenge%d.ini", proInfo->processID);
	//fprintf(stderr, "path = %s\n", challengeString);
	FILE *fp ;
	fp = fopen(challengeString, "rb");
	if (fp == NULL)
	{
		return ;
	}
	count = fread(chCode, 1, 256, fp);
	//fprintf(stderr, "count=%d, temp=%s\n", count, chCode);
	fclose(fp);
#endif
}

void writekey(unsigned char channel, char const *chCode)
{
//	Debug(ckite_log_message, "http request key is of = %s\n", chCode);
// 	char challengeString[60] = {0};
// 	LPhikProfile proInfo = getProfileParamenterInfo();
// 
// 	snprintf(challengeString, sizeof(challengeString), "..//exec//challenge%d.ini", proInfo->processID);
// 	//fprintf(stderr, "path = %s\n", challengeString);
// 	FILE *fp ;
// 	fp = fopen(challengeString, "wb+");
// 	if (fp == NULL)
// 	{
// 		return ;
// 	}
// 	fwrite(chCode, 1, 256, fp);
// 	fclose(fp	//memcpy(temp_key, chCode, strlen((char const *)chCode));
//	Debug(ckite_log_message, "temp key is of = %s\n", chCode);

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
   
//#define CIF_CONFIG	"\x00\x00\x01\xb0\x08\x00\x00\x01\xb5\x09\x00\x00\x01\x00\x00\x00\x01\x20\x00\x84\x40\x7d\x32\x9c\x2c\x10\x90\x51\x8f" 
//#define CIF_CONFIG  "\x00\x00\x01\xB0\x03\x00\x00\x01\xB5\x09\x00\x00\x01\x00\x00\x00\x01\x20\x00\x84\x40\x06\x68\x50\x20\xF0\xA3\x1F" // Open Area
//#define CIF_CONFIG "\x00\x00\x01\xb0\x08\x00\x00\x01\xb5\x09\x00\x00\x01\x00\x00\x00\x01\x20\x00\x84\x40\xfa\x31\x92\x16\x08\x48\x28\xc7" // actual 
#define CIF_CONFIG "\x00\x00\x01\xb0\x08\x00\x00\x01\xb5\x09\x00\x00\x01\x00\x00\x00\x01\x20\x00\x84\x40\xfa\x28\x58\x21\x20\xA3\x1f" // fix_vop_rate = 0 

// #define QCIF_CONFIG "\x00\x00\x01\xb0\x08\x00\x00\x01\xb5\x09\x00\x00\x01\x00\x00\x00\x01\x20\x00\x84\x40\x02\x33\x05\x84\x12\x14\x63"
#define QCIF_CONFIG "\x00\x00\x01\xb0\x08\x00\x00\x01\xb5\x09\x00\x00\x01\x00\x00\x00\x01\x20\x00\x84\x40\x06\x68\x2c\x20\x90\xa3\x1f"									
#define D1_CONFIG	"\x00\x00\x01\xb0\x01\x00\x00\x01\xb5\x09\x00\x00\x01\x01\x00\x00\x01\x20\x00\x84\x40\xfa\x28\xb4\x22\x40\xa3\x1f" 
//#define CIF_CONFIG "\x00\x00\x01\xb0\x08\x00\x00\x01\xb5\x09\x00\x00\x01\x00\x00\x00\x01\x20\x00\x84\x40\xfa\x28\x58\x21\x20\xa3\x1f" // p2p is ok

#define H264CIF_SPS_CONFIG "\x67\x42\xc0\x0c\xf2\x02\xc1\x2d\x08\x00\x00\x03\x03\x20\x00\x00\x03\x00\10\x78\xa1\x52\x40"
#define H264CIF_PPS_CONFIG "\x68\xcb\x83\xcb\x20"
#define H264QCIF_SPS_CONFIG "\x67\x42\xc0\x0a\xf2\x05\x89\xd0\x80\x00\x00\x32\x00\x00\x03\x00\x01\x07\x89\x13\x24"
#define H264QCIF_PPS_CONFIG "\x68\xcb\x83\xcb\x20"
#define H264D1_SPS_CONFIG "\x67\x42\xc0\x16\xf2\x01\x68\x24\xd0\x80\x00\x00\x32\x00\x00\x03\x00\x01\x07\x8b\x17\x24"
#define H264D1_PPS_CONFIG "\x68\xcb\x83\xcb\x20"
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
	else if(width == 176 && height == 144)
	{
	    Debug(ckite_log_message, "getVideoCodecConfig store.\n");
		spsOut = base64Encode((char *)H264QCIF_SPS_CONFIG, 21);
		ppsOut = base64Encode((char *)H264QCIF_PPS_CONFIG, 5);
		//spsOut = base64Encode((char *)H264_STORE_SPS_CONFIG, 21);
		//ppsOut = base64Encode((char *)H264_STORE_PPS_CONFIG, 5);
		sprintf((char *)config, "%s,%s", spsOut, ppsOut);
		Debug(ckite_log_message, "config = %s\n", config);

	}
	else if (width == 720 && height == 576)
	{
		spsOut = base64Encode((char *)H264D1_SPS_CONFIG, 22);
		Debug(ckite_log_message, "spsOut = %s\n", spsOut);
		ppsOut = base64Encode((char *)H264D1_PPS_CONFIG, 5);
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
	else if(width == 176 && height == 144)
	{
		unsigned char *qcifConfig = (unsigned char *)QCIF_CONFIG;
		memcpy(config, qcifConfig, 28);
		*config_length = 28;
	}
	else if(width == 720 && height == 576)
	{
		unsigned char *D1Config = (unsigned char *)D1_CONFIG;
		memcpy(config, D1Config, 28);
		*config_length = 28;
	}
#endif
}

void RTSPServer::RTSPEncoderSession
::deviceReset(void)
{
	//reset
	Debug(ckite_log_message, "reset \n");
}

void RTSPServer::RTSPEncoderSession
::deviceHandlePtz( int actionType, int speed)
{
	//ptz action
	switch(actionType)
	{
	case LEFTSTOP:
		Debug(ckite_log_message, "ptz left stop\n");break;
	case RIGHTSTOP:
		Debug(ckite_log_message, "ptz right stop\n");break;
	case UPSTOP:
		Debug(ckite_log_message, "ptz up stop\n");break;
	case DOWNSTOP:
		Debug(ckite_log_message, "ptz down stop\n");break;
	case AUTOSTOP:
		Debug(ckite_log_message, "ptz auto stop\n");break;
	case FOCUSFAR:
		Debug(ckite_log_message, "ptz focus far\n");break;
	case FOCUSNEAR:
		Debug(ckite_log_message, "ptz focus near\n");break;
	case ZOOMTELESTOP:
		Debug(ckite_log_message, "ptz zoomtele stop\n");break;
	case ZOOMWIDESTOP:
		Debug(ckite_log_message, "ptz zoomwide stop\n");break;
	case IRISOPENSTOP:
		Debug(ckite_log_message, "ptz irisopen stop\n");break;
	case IRISCLOSESTOP:
		Debug(ckite_log_message, "ptz irisclose stop\n");break;
	case WIPERON:
		Debug(ckite_log_message, "ptz wiperon stop\n");break;
	case WIPEROFF:
		Debug(ckite_log_message, "ptz wiperoff stop\n");break;
	case LIGHTON:
		Debug(ckite_log_message, "ptz lighton stop\n");break;
	case LEFT:
		Debug(ckite_log_message, "ptz left stop\n");break;
	case RIGHT:
		Debug(ckite_log_message, "ptz right stop\n");break;
	case UP:
		Debug(ckite_log_message, "ptz up stop\n");break;
	case DOWN:
		Debug(ckite_log_message, "ptz down stop\n");break;
	case AUTO:
		Debug(ckite_log_message, "ptz auto stop\n");break;
	case STOP:
		Debug(ckite_log_message, "ptz stop stop\n");break;
	default:
		Debug(ckite_log_message, "ptz Unknown\n");
	}
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
::deviceReponseVideoPlayBack(char *fileName)
{

	Debug(ckite_log_message, "vedio play back \n");

}



void RTSPServer::RTSPEncoderSession
::get_LanWebURL(char *p, int p_len)
{
	sprintf(p+p_len,"%s%s\r\n", "LanWebURL: ","http://192.168.10.103:80");
}

void RTSPServer::RTSPEncoderSession
::get_WanWebURL(char *p, int p_len)
{
	sprintf(p+p_len,"%s%s\r\n", "WanWebURL: ","http://[WANIP]:80");
}

void RTSPServer::RTSPEncoderSession
::get_P2PLanURL(char *p, int p_len)
{
	sprintf(p+p_len,"%s%s\r\n", "P2PLanURL: ","rtsp://192.168.10.103:8554/live");
}

void RTSPServer::RTSPEncoderSession
::get_P2PWanURL(char *p, int p_len)
{
	sprintf(p+p_len,"%s%s\r\n", "P2PWanURL: ","rtsp://[WANIP]:8150/live_video.sdp");
}

void RTSPServer::RTSPEncoderSession
::get_LanFtpURL(char *p, int p_len)
{
	sprintf(p+p_len,"%s%s\r\n", "LanFtpURL: ","ftp://192.168.10.103:21");
}

void RTSPServer::RTSPEncoderSession
::get_WanFtpURL(char *p, int p_len)
{
	sprintf(p+p_len,"%s%s\r\n", "WanFtpURL: ","ftp://192.168.10.102:3011");
}

void RTSPServer::RTSPEncoderSession
::get_LanVoiceAddr(char *p, int p_len)
{
	sprintf(p+p_len,"%s%s\r\n", "LanVoiceAddr: ","192.168.10.103:8000");
}

void RTSPServer::RTSPEncoderSession
::get_WanVoiceAddr(char *p, int p_len)
{
	sprintf(p+p_len,"%s%s\r\n", "WanVoiceAddr: ","192.168.10.102:15400");
}

void RTSPServer::RTSPEncoderSession
::get_LanDataAddr(char *p, int p_len)
{
	sprintf(p+p_len,"%s%s\r\n", "LanDataAddr: ","192.168.10.103:8000");
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
	else if (strcmp(session_name, "mobile") == 0)
	{
		width = 176;
		height = 144;
	}
//	videoGetFrameInfoByHik(channel-1, (char*)framebuf, framesize, width, height);
#if 0
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
#endif

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

void* getFileHandle( int camid, char* filename, char* sdp, int* sdpSize)
{
	char path[100] = {0};

	snprintf(path, sizeof(path), "%s/%s", PLAY_FILE_DIR, filename);
	Debug(ckite_log_message, "open file path = %s\n", path);
	return fopen(path, "rb");
}

void releaseFileHandle( void* handle )
{

}

void getRecordDirectories( int camid, char** dirList, int* count, int* index, int* rest_count)
{
// 	struct dirent *dp;
// 	DIR *dfd;
// 	struct stat stbuf;
// 	char dir[20];
// 	int MaxDisplayNum = *count; 
// 	int total_count = 0;
// 	int startIndex = 0, currIndex = 0;
// 	int i = 0;
// 
// 	memset(dir, 0x0, sizeof dir);
// 	memcpy(dir, PLAY_FILE_DIR, strlen(PLAY_FILE_DIR));
// 	*count = 0;
// 	*rest_count = 0;
// 	if(stat(dir,&stbuf) == -1)
// 		return ;
// 	if(S_ISDIR(stbuf.st_mode) == 0)
// 		return ;
// 	if((dfd = opendir(dir)) == NULL)
// 		return ;
// 	startIndex = (*index) - 1;
// 	while((dp = readdir(dfd)) != NULL)
// 	{
// 		if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
// 			continue;
// 		if( currIndex >= startIndex && currIndex < (startIndex +MaxDisplayNum))
// 		{
// 			memcpy(dirList[i], dp->d_name, strlen(dp->d_name));
// 			Debug(ckite_log_message, "currIndex = %d\n", currIndex);
// 			Debug(ckite_log_message, "len = %d, i = %d\n", strlen(dp->d_name), i);
// 			Debug(ckite_log_message, "dp->d_name = %s\n", dp->d_name);
// 			Debug(ckite_log_message, "dir = %s\n", dirList[i]);
// 			(*count)++;
// 			i++;
// 		}
// 		currIndex++;
// 		total_count++;
// 	}
// 	if(*count < *index)
// 	{
// 		*rest_count = 0;
// 	}
// 	else
// 	{
// 		*rest_count = (total_count - *count);	
// 	}
// 	Debug(ckite_log_message, "count = %d\n", *count);
// 	closedir(dfd);
}


void paraseTimeRangeBy_tzp(char *time, char *time_range_start[])
{
	Boolean is_delimiter = 1, k = 0;
	if(time == NULL)
	{
		return ;
	}
	int i = 0;
	while(time[i] != '\r' && time[i+1] != '\n')
	{
		if(is_delimiter)
		{
			if(time[i] != 'T' || time[i] != 'Z' || time[i] != 'P')
			{
				time_range_start[k++] = &time[i];
				is_delimiter = 0;
			}
		}
		else
		{
			if(time[i] == 'T' || time[i] == 'Z' || time[i] == 'P')
			{
				time[i] = '\0';
				is_delimiter = 1;
			}
		}
		++i;
	}
}

void parseTime_getTimeFileList(char **fileList, char * time_file_dir, time_t start_time_sec, time_t end_time_sec, int *count, int *index, int *rest_count)
{
// 	struct dirent *dp;
// 	DIR *dfd;
// 	struct stat st_buf;
// 	struct tm tmp_time;
// 	time_t file_time;
// 	char year[5] = {0};
// 	char mon[3] = {0};
// 	char day[3] = {0};
// 	char hour[3] = {0};
// 	char min[3] = {0};
// 	char sec[3] = {0};
// 	char *tmp = NULL;
// 	char timeMS[10] = {0};
// 	int total_count = 0;
// 	char timeDir[30];
// 	int MaxDisplayNum = *count;
// 	int startIndex = 0, currIndex = 0;
// 	int i = 0, j = 0;
// 
// 	if(time_file_dir == NULL) return;
// 
// 	startIndex = (*index) - 1;
// 	*count = 0;
// 	*rest_count = 0;
// 	memset(timeDir, 0x0, sizeof timeDir);
// 	sprintf(timeDir, "%s/%s", PLAY_FILE_DIR, time_file_dir);
// 
// 	if(stat(timeDir,&st_buf) == -1){
// 		printf("error 1\n");
// 		return ;
// 	}
// 	if(S_ISDIR(st_buf.st_mode) == 0){
// 		printf("error 2\n");   
// 		return ;
// 	}
// 	if((dfd = opendir(timeDir)) == NULL){
// 		printf("error 3\n");
// 		return ;
// 	}
// 	while((dp = readdir(dfd)) != NULL)
// 	{
// 		if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
// 			continue;
// 		else{
// 			//	printf("dp->d_name file = %s", dp->d_name);
// 			tmp = dp->d_name;
// 			tmp += 1;
// 			strncpy(timeMS, tmp, 6);
// 			Debug(ckite_log_message, "time_file_dir = %s\n", time_file_dir);
// 			Debug(ckite_log_message, "timeMS = %s\n", timeMS);
// 			parseTime_GetEndTime(time_file_dir, timeMS, year, mon, day, hour, min, sec);
// 			tmp_time.tm_year = atoi(year) - 1900;
// 			tmp_time.tm_mon = atoi(mon) - 1;
// 			tmp_time.tm_mday = atoi(day);
// 			tmp_time.tm_hour = atoi(hour); 
// 			tmp_time.tm_min = atoi(min); 
// 			tmp_time.tm_sec = atoi(sec); 
// 			file_time = mktime(&tmp_time);
// 			if(file_time >= start_time_sec && file_time <= end_time_sec){
// 				Debug(ckite_log_message, "j = %d, currIndex = %d, startIndex = %d\n", j++, currIndex, startIndex);
// 				if(currIndex >= startIndex && currIndex < (startIndex + MaxDisplayNum))
// 				{
// 					memcpy(fileList[i], dp->d_name, strlen(dp->d_name)); // file name maximun lenght is 30
// 					Debug(ckite_log_message, "i = %d, fileList[i] = %s\n", i, fileList[i]);
// 					(*count)++;
// 					i++;
// 				}
// 			}
// 		}
// 		currIndex++;
// 		total_count++;
// 	}
// 	if(*count < *index)
// 	{
// 		*rest_count = 0;
// 	}
// 	else
// 	{
// 		*rest_count = (total_count - *count);	
// 	}
// 	closedir(dfd);
}


void getRecordFiles( int camid, char* time, char** fileList, int* count, int* index, int* rest_count)
{
// 	char *time_range_start[3];// year month day 
// 	char year[5] = {0};
// 	char month[3] = {0};
// 	char day[3] = {0};
// 	char hour[3] = {0};
// 	char minute[3] = {0};
// 	char second[3] = {0};
// 	struct tm my_time;
// 	time_t start_time_sec = 0;
// 	time_t end_time_sec = 0;
// 	time_t start_time_day = 0;
// 	time_t end_time_day = 0;
// 	paraseTimeRangeBy_tzp(time, time_range_start); // find time range
// 
// 	parseTime_GetEndTime(time_range_start[0],time_range_start[1], year, month, day, hour, minute, second); //get start time info
// 	Debug(ckite_log_message, "time_range_start[0] = %s\n", time_range_start[0]);
// 	Debug(ckite_log_message, "time_range_start[1] = %s\n", time_range_start[1]);
// 	my_time.tm_year = atoi(year) - 1900;
// 	my_time.tm_mon = atoi(month) - 1;
// 	my_time.tm_mday = atoi(day);
// 	my_time.tm_hour = atoi(hour);
// 	my_time.tm_min = atoi(minute);
// 	my_time.tm_sec = atoi(second);
// 	start_time_sec = mktime(&my_time);
// 	end_time_sec = start_time_sec + atoi(time_range_start[2])*60; // end time sec num 
// 
// 	parseTime_getTimeFileList(fileList, time_range_start[0], start_time_sec, end_time_sec, &(*count), &(*index), &(*rest_count));
}

void getFileVideoFrame( void* handle, FrameTime_t* offset, unsigned char* framebuf, int* framesize, int* videoType, bool force_I_frame)
{
	enum videotype {VIDEO_RAW, VIDEO_MPEG4, VIDEO_H264};
	FILE *fp = (FILE *)handle;
	int getFrameSize = *framesize;

	int n = fread(framebuf, 1, getFrameSize, fp);
	if (n != getFrameSize)
	{
		fseek(fp, SEEK_SET, 0);
		n = fread(framebuf, 1, getFrameSize, fp);
		if (n != getFrameSize)
		{
			Debug(ckite_log_message, "FATAL error: cannot read bytes from input file!\n");
			*framesize = 0;
			return;
		}
	}
	*framesize = getFrameSize;
	*videoType = VIDEO_RAW;
}

void getFileAudioFrame( void* handle, FrameTime_t* offset, unsigned char* framebuf, int* framesize, int* audioType)
{
	enum audiotype {AUDIO_RAW, AUDIO_AMRNB, AUDIO_AMRWB};
	FILE *fp = (FILE *)handle;
	int getFrameSize = *framesize;
	
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
	else if ((strcmp(session_name, "mobile") == 0) && (width == 176) && (height == 144))
	{
		video_bitrate = 40;
		framerate = 8;
		keyinterval = 5;
	}
	else if ((strcmp(session_name, "livehd") == 0) && (width == 720) && (height == 576))
	{
		video_bitrate = 200;	
		framerate = 10;
		keyinterval = 5;
	}
	else if ((strcmp(session_name, "store") == 0))
	{
		if ((width == 352) && (height == 288))	
		{
			video_bitrate = 200;
			framerate = 10;
			keyinterval = 5;
		}
		else if ((width == 176) && (height == 144))
		{
			video_bitrate = 60;
			framerate = 8;
			keyinterval = 5;
		}
		else if ((width == 720) && (height == 576))
		{
			video_bitrate = 200;
			framerate = 10;
			keyinterval = 5;
		}
	}
	audio_bitrate = 5;
	framelength = 160;
	samplerate = 8000;
}

void getStoreFileResolution(char *file_name, int& width, int& height)
{
	if (strstr(file_name,".qcf"))	
	{
		width = 176;
		height = 144;
	}
	else if (strstr(file_name, ".cif"))
	{
		width = 352;
		height = 288;
	}
	else if (strstr(file_name, ".d1"))
	{
		width = 720;
		height = 576;
	}
}

void getUpgradeStatus(char *upgrade_version, char *upgrade_status)
{
	
}

void getWebServerHostInfo(char *host, unsigned short *port)
{
	memcpy(host, "219.235.235.4", strlen("219.235.235.4"));
	*port = 80;
}