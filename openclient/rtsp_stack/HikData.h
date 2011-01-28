#ifndef _HIK_SDK_H
#define _HIK_SDK_H

#define MAX_FRAME_COUNT 3
#define MAX_USE_LEN 256
#define WM_DVR_MSG		(WM_USER + 0x200)

#define CIF_WIDTH	352
#define CIF_HEIGHT	288
#define QCIF_WIDTH	176
#define QCIF_HEIGHT	144

#if 0
typedef struct threadAttribute
{
	HANDLE hThread;
	DWORD threadID;
}THREAD_ATTR_PARAM, *LPTHREAD_ATTR_PARAM;
#endif

typedef unsigned long (__stdcall *HikThreadFuncCallback)(void*);

typedef struct singleframe
{
	char *buf;
	long size;
	long width;
	long height;
	long type;
	//char *filename;

}SINGLE_FRAME, *LPSINGLE_FRAME;

typedef struct frameData
{
	LPSINGLE_FRAME frame;
	struct frameData *next;
}FRAME_DATA_ST, *LPFRAME_DATA_ST;

typedef struct frameQueue
{
	LPFRAME_DATA_ST front; 
	LPFRAME_DATA_ST rear;
	unsigned int totalLenght;
	unsigned int currLenght;
}FRAME_QUEUE_ST, *LPFRAME_QUEUE_ST;

typedef enum {
	HikInstance_Idle,
	HikInstance_Running,
	HikInstance_Quit
} HikInstanceState;


typedef struct _HikInstance
{
	HWND hWnd;
	HikInstanceState state;
	DWORD dwThreadId;
	HANDLE hMsgThread;
} HikInstance;

typedef struct _HikProfile
{
	char IpAddr[20];
	unsigned short port;
	char userName[20];
	char password[20];
	unsigned char channelNumTotal;
	unsigned char subChannel[32];
	char deviceMAC[20];
	char winServerIP[20];
	unsigned short winPort;
	unsigned short winListenPort;
	unsigned int processID;
}hikProfile, *LPhikProfile;

typedef struct _hikCommonInfo
{
	long m_lStockHandle;
	long type;
	unsigned char channel;
	HANDLE hThread;
	DWORD threadID;
}hikCommonInfo, *LPhikCommonInfo;

extern DWORD WINAPI MessageLoop(LPVOID pParam);
extern void StopHikMessageLoop(HikInstance* inst);
void freeSingleFrameData(LPFRAME_DATA_ST node);
unsigned long ThreadCallback(unsigned char *channel);
void getFrameByHikSDK(unsigned char channelTotal);
extern void videoGetFrameInfoByHik(unsigned char channel, char *framebuf, int *framesize, int width, int height);
extern void resetCapturePicState(unsigned char channel);

#endif