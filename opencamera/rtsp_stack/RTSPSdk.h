#ifndef _RTSP_SDK_H
#define _RTSP_SDK_h

#define FrameTime_t int 

void getVideoCodecConfig(int width, int height, int *profile_level_id, unsigned char *config, unsigned int *config_length);
void getServerInfo(char *host, unsigned short *port, int defaultType);
void getDeviceInfo(char *ip, unsigned short *port, char *mac);
void getHttpServerInfo(char *host, unsigned short *port);
void getUserAgent(char *ua_buf);
enum State{READY,KEYFAILED, DNSFAILED, SETUP, PLAY};
int setSessionState(char *session_name, int st);
int getSessionState(char *session_name);
void setSSRCMask(char *type, unsigned int mask);
void *getSSRCMask(void);
extern int httpRequestKey(char *recvBuffer, char *mac);
extern void realEntryMain(void);

extern void* getFileHandle( int camid, char* filename, char* sdp, int* sdpSize);
extern void releaseFileHandle( void* handle );
extern void getRecordDirectories( int camid, char** dirList, int* count, int* index, int* total_count);
extern void getRecordFiles( int camid, char* time, char** fileList, int* count, int* index, int* total_count);
extern void getFileVideoFrame( void* handle, FrameTime_t* offset, unsigned char* framebuf, int* framesize, int* videoType, bool force_I_frame);
extern void getFileAudioFrame( void* handle, FrameTime_t* offset, unsigned char* framebuf, int* framesize, int* audioType);
extern void videoGetFrameInfo( void *handle, char* session_name, unsigned char *framebuf, int *framesize, int *videoType);
extern void audioGetFrameInfo( void *handle, char* session_name, char *framebuf, int *framesize, int*audioType);
extern void getSubsessionParaConfig(char *session_name, int width, int height, unsigned int& video_bitrate, unsigned int& framerate, unsigned int& keyinterval,
																			   unsigned int& audio_bitrate, unsigned int& framelenght, unsigned int& samplerate);
extern void getStoreFileResolution(char *file_name, int& width, int &height);
extern void getUpgradeStatus(char *upgrade_version, char *upgrade_status);

#endif 	
