#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wtypes.h>
#include "HikData.h"
//#include "XClientDLL.h"
#include "RTSPSdk.h"
#include "../CrashReport/ExceptionHandler.h"

typedef struct ChannelObject
{
	unsigned int channel_id;
	long startCapturePic;
	BOOLEAN IsFirst;
	char *MidFrameBuffer[MAX_FRAME_COUNT];
	CRITICAL_SECTION cs;
} ChannelObject;

#define  FRAMEQUEUE    
static FRAME_QUEUE_ST frameQueue;
static CRITICAL_SECTION criticalSection;
#ifdef FRAMEQUEUE
static char cifBuf[CIF_WIDTH*CIF_HEIGHT*3/2] = {0};
static MidFrameCount = 0;
long volatile startCapturePic[MAX_CHANNEL] = {0};

static ChannelObject channelList[MAX_CHANNEL];
static hikCommonInfo HCInfo[MAX_CHANNEL];
#endif


void initChannelList()
{
	int i = 0;
	while (i<MAX_CHANNEL)
	{
		channelList[i].channel_id = -1;
		channelList[i].startCapturePic = -1;
		channelList[i].IsFirst = TRUE;
		InitializeCriticalSection(&channelList[i].cs);
		i++;
	}
}

long getChannelState(unsigned int channel_id)
{
	EnterCriticalSection(&channelList[channel_id].cs);
	int i = 0;
	while (i<MAX_CHANNEL)
	{
		if (channelList[i].channel_id == channel_id)
		{
			LeaveCriticalSection(&channelList[i].cs);
			return channelList[i].startCapturePic;
		}
		i++;
	}
	LeaveCriticalSection(&channelList[channel_id].cs);
	return -1;
}

long setChannelState(unsigned int channel_id, long state)
{
	EnterCriticalSection(&channelList[channel_id].cs);
	int i = 0;
	while (i<MAX_CHANNEL)
	{
		if (i == channel_id)
		{
			channelList[i].channel_id = channel_id;
			channelList[i].startCapturePic = state;
			break;
		}
		i++;
	}
	LeaveCriticalSection(&channelList[channel_id].cs);
	return TRUE;
}

void reclaimChannelState(unsigned int channel_id)
{
	EnterCriticalSection(&channelList[channel_id].cs);
	int i = 0;
	while (i<MAX_CHANNEL) 
	{
		if (i == channel_id)
		{
			channelList[i].channel_id = -1;
			channelList[i].startCapturePic = -1;
			//channelList[i].IsFirst = FALSE;
			break;
		}
		i++;
	}
	LeaveCriticalSection(&channelList[channel_id].cs);	
}

void initFrameData(LPFRAME_QUEUE_ST lq)
{
#ifdef FRAMEQUEUE
	for (int i=0; i<MAX_FRAME_COUNT; i++)
	{
		for (int j=0; j<MAX_FRAME_COUNT; j++)
		{
			channelList[i].MidFrameBuffer[j] = new char[CIF_WIDTH*CIF_HEIGHT*3/2];
			memset(channelList[i].MidFrameBuffer[j], 0, CIF_WIDTH*CIF_HEIGHT*3/2);
		}
	}
#else
	lq->front=lq->rear = new FRAME_DATA_ST;
	if(!lq->front)
	{
		fprintf(stderr, "malloc is failed.\n");
	}
	lq->front->next=NULL;
	lq->totalLenght = MAX_FRAME_COUNT;
	lq->currLenght = 0;
#endif
}

int deleteFrontNodeData(LPFRAME_QUEUE_ST lq)
{
	if (lq->front == lq->rear)
	{
		//fprintf(stderr, "deleteFrontNodeData queue is empty.\n");
		return -1;
	}
	LPFRAME_DATA_ST temp = lq->front->next;
	lq->front->next = temp->next; 
	if (lq->rear == temp)
	{
		lq->rear = lq->front;
	}
	freeSingleFrameData(temp);
	lq->currLenght--;
	//fprintf(stderr, "delete front node data sucess.\n");
	return 1;
}

int addFrameData(unsigned char channel, LPFRAME_QUEUE_ST lq, LPSINGLE_FRAME lframe)
{
#ifdef FRAMEQUEUE
	EnterCriticalSection(&criticalSection);
	if (MidFrameCount == MAX_FRAME_COUNT)
	{
		memset(channelList[channel].MidFrameBuffer[0], 0, CIF_WIDTH*CIF_HEIGHT*3/2);
		memcpy(channelList[channel].MidFrameBuffer[0], channelList[channel].MidFrameBuffer[1], CIF_WIDTH*CIF_HEIGHT*3/2);
		memcpy(channelList[channel].MidFrameBuffer[1], channelList[channel].MidFrameBuffer[2], CIF_WIDTH*CIF_HEIGHT*3/2);
		MidFrameCount--;
	}
	else
	{
		memcpy(channelList[channel].MidFrameBuffer[MidFrameCount], lframe->buf, lframe->size);
		MidFrameCount++;
	}
	LeaveCriticalSection(&criticalSection);	
	return 1;
#else

	BOOL stat = TRUE;
	EnterCriticalSection(&criticalSection);
	//fprintf(stderr, "add start.\n");
	if (lq->currLenght == lq->totalLenght)
	{
		//fprintf(stderr, "queue is full.\n");
		deleteFrontNodeData(lq);
	}
	LPFRAME_DATA_ST node = new FRAME_DATA_ST;
	if (!node)
	{
		fprintf(stderr, "node malloc failed.\n");
		stat = FALSE;
	}
	else
	{
		node->frame = new SINGLE_FRAME;
		if (!node->frame)
		{
			fprintf(stderr, "node->frame malloc failed.\n");
			stat = FALSE;
			//delete [] node;
			free(node);
			node = NULL;
		}
		else
		{
			node->frame->size = lframe->size;
			node->frame->buf = new char[lframe->size];
			if (!node->frame->buf)
			{
				fprintf(stderr, "node->frame->buf malloc failed.\n");
				stat = FALSE;
				delete [] node->frame;
				node->frame = NULL;
				delete [] node;
				node = NULL;
			}
		}
	}
	if (FALSE == stat)
	{
		fprintf(stderr, "stat is failed.\n");
		LeaveCriticalSection(&criticalSection);	
		return FALSE;
	}
	
	memcpy(node->frame->buf, lframe->buf, lframe->size);
	node->frame->height = lframe->height;
	node->frame->width = lframe->width;
	node->frame->type = lframe->type;
	node->next = NULL;
	lq->rear->next = node;
	lq->rear = node;
	lq->currLenght++;
	//fprintf(stderr, "add end.\n");
	LeaveCriticalSection(&criticalSection);	
	return 1;
#endif
}

int readFrameData(unsigned char channel, LPFRAME_QUEUE_ST lq, LPSINGLE_FRAME lframe)
{
#ifdef FRAMEQUEUE
	EnterCriticalSection(&criticalSection);
	if (MidFrameCount == 0)
	{
		LeaveCriticalSection(&criticalSection);
		return -1;
	}
	else
	{
		memcpy(lframe->buf, channelList[channel].MidFrameBuffer[0], CIF_WIDTH*CIF_HEIGHT*3/2);
		memcpy(channelList[channel].MidFrameBuffer[0], channelList[channel].MidFrameBuffer[1], CIF_WIDTH*CIF_HEIGHT*3/2);
		memcpy(channelList[channel].MidFrameBuffer[1], channelList[channel].MidFrameBuffer[2], CIF_WIDTH*CIF_HEIGHT*3/2);
		MidFrameCount--;
	}
	LeaveCriticalSection(&criticalSection);	
	return 1;
#else
	EnterCriticalSection(&criticalSection);
	if (lq->front == lq->rear)
	{
		//fprintf(stderr, "queue is empty.\n");
		LeaveCriticalSection(&criticalSection);	
		return -1;
	}
	//fprintf(stderr, "read start.\n");
	LPFRAME_DATA_ST temp = lq->front->next;
	lframe->size = temp->frame->size;
	memcpy(lframe->buf, temp->frame->buf, temp->frame->size);
	lframe->height = temp->frame->height;
	lframe->width = temp->frame->width;
	lframe->type = temp->frame->type;
	lq->front->next = temp->next;
	if(lq->rear==temp) 
	{
		lq->rear=lq->front;
	}
	freeSingleFrameData(temp);
	lq->currLenght--;
	//fprintf(stderr, "read end.\n");
	LeaveCriticalSection(&criticalSection);	
	return 1;
#endif
}

void DestoryFrameData(LPFRAME_QUEUE_ST lq)
{
	while(lq->front)
	{
		lq->rear = lq->front->next;
		freeSingleFrameData(lq->front);
		lq->front = lq->rear;
	} 
}

void freeSingleFrameData(LPFRAME_DATA_ST node)
{
	if (node)
	{
		if (node->frame)
		{
			if(node->frame->buf)
			{
				delete [] node->frame->buf;
				node->frame->buf = NULL;
			}
			delete node->frame;
			node->frame = NULL;
		}
		delete node;
		node = NULL;
	}
}

void I420Cif2Qcif(unsigned char* pCif, unsigned char* pQcif)
{
	unsigned char* pSY, *pSU, *pSV;
	unsigned char* pDY, *pDU, *pDV;
	int i, j;
	
	pSY = pCif;
	pSU = pSY + CIF_WIDTH*CIF_HEIGHT;
	pSV = pSU + CIF_WIDTH*CIF_HEIGHT/4;
	
	pDY = pQcif;
	pDU = pDY + QCIF_WIDTH*QCIF_HEIGHT;
	pDV = pDU + QCIF_WIDTH*QCIF_HEIGHT/4;
	
	for (j=0; j<QCIF_HEIGHT; j++)
	{
		for (i=0; i<QCIF_WIDTH; i++)
		{
			pDY[j*QCIF_WIDTH+i] = (pSY[2*j*CIF_WIDTH+2*i] + pSY[2*j*CIF_WIDTH+2*i+1] + pSY[(2*j+1)*CIF_WIDTH+2*i] + pSY[(2*j+1)*CIF_WIDTH+2*i+1]+2)/4;
			if (!(j&1) && !(i&1))
			{
				pDU[j*QCIF_WIDTH/4+i/2] = (pSU[j*CIF_WIDTH/2+i] + pSU[j*CIF_WIDTH/2+i+1] + pSU[(j+1)*CIF_WIDTH/2+i] + pSU[(j+1)*CIF_WIDTH/2+i+1]+2)/4;
				pDV[j*QCIF_WIDTH/4+i/2] = (pSV[j*CIF_WIDTH/2+i] + pSV[j*CIF_WIDTH/2+i+1] + pSV[(j+1)*CIF_WIDTH/2+i] + pSV[(j+1)*CIF_WIDTH/2+i+1]+2)/4;
			}
		}
	}
}

void videoGetFrameInfoByHik(unsigned char channel, char *framebuf, int *framesize, int width, int height)
{
	setChannelState(channel, TRUE);
	SINGLE_FRAME frame;
	//fprintf(stderr, "get channel %d video.\n",channel);
	
	if (width == CIF_WIDTH && height == CIF_HEIGHT)
	{
		frame.buf = framebuf;
		while (1)
		{
			if (readFrameData(channel, &frameQueue, &frame) > 0)
			{
				break;
			}
			else
			{
				Sleep(10);
			}
		}
	}
	else if (width == QCIF_WIDTH && height == QCIF_HEIGHT)
	{
		frame.buf = cifBuf;
		while (1)
		{
			if (readFrameData(channel, &frameQueue, &frame) > 0)
			{
				break;
			}
			else
			{
				Sleep(10);
			}
		}
		I420Cif2Qcif((unsigned char*)cifBuf, (unsigned char*)framebuf);
	}
}

void savePicFile(char* buf, long size, long width, long height, long type, char* filename)
{
	FILE *stream_fp;
	char name[0xff];
	memset(name, 0, 0xff);
	sprintf(name, "%s%s", "E:\\starvSDK\\temp\\", filename);
	stream_fp = fopen(name, "wb+");
	fwrite(buf, 1, size, stream_fp);
	fclose(stream_fp);
}

void resetCapturePicState(unsigned char channel)
{
	fprintf(stderr, "resetCapturePicState.\n");
	reclaimChannelState(channel);
	//if (X_ClientStopCapture(HCInfo[channel].m_lStockHandle, (WORD)HCInfo[channel].type))
	//{
	//	X_ClientSetCapPicCallBack(NULL);
	channelList[channel].IsFirst = TRUE;
	//}
	X_ClientStop(HCInfo[channel].m_lStockHandle, (WORD)HCInfo[channel].type);
}

void (CALLBACK CapPicFun)(long StockHandle,char * pBuf,long nSize,long nWidth,long nHeight,long nStamp,long nType,long nReceaved)
{
	//fprintf(stderr, "CapPicFun StockHandle = %d\n", StockHandle);
	// 	char filename[0xff];
	// 	memset(filename, 0, 0xff);
	// 	sprintf(filename,"%lu.bmp", GetTickCount());
	 	SINGLE_FRAME frame;
	 	frame.buf = pBuf;
	 	frame.height = nHeight;
	 	frame.size = nSize;
	 	frame.type = nType;
	 	frame.width = nWidth;
	//savePicFile(pBuf,nSize,nWidth,nHeight,nType,filename);
	int i = 0;
	while (i<MAX_CHANNEL)
	{
		if (StockHandle == HCInfo[i].m_lStockHandle)
		{
			addFrameData(HCInfo[i].channel, &frameQueue, &frame);
			break;
		}
		i++;
	}
}


void ThreadCallbackEntry(unsigned char subChannel)
{

	LPhikProfile proInfo = getProfileParamenterInfo();

	HikInstance* inst = new HikInstance;
	inst->state = HikInstance_Idle;
	
	inst->hMsgThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MessageLoop, inst, 0, &inst->dwThreadId);
	X_ClientInit(WM_DVR_MSG, inst->hWnd);
	
	static PXCLIENT_VIDEOINFO pXClientInfo[MAX_CHANNEL];
	LONG lUserID = -1;
	WORD wType = 2;	
	
	pXClientInfo[subChannel] = (XCLIENT_VIDEOINFO *)calloc(sizeof(XCLIENT_VIDEOINFO), 1);
	pXClientInfo[subChannel]->m_bytRemoteChannel = subChannel;//0; //接收通道号从0开始
	pXClientInfo[subChannel]->m_bytSendMode = TCPMODE; //网络连接方式{UDPMODE=0,TCPMODE,MULTIMODE}
	pXClientInfo[subChannel]->m_bytSendType = LAN; //网络传输类型{LAN = 0, ADSL, ISDN, PSTN}
	pXClientInfo[subChannel]->m_bytImgFormat = 0; //接收图像格式（0为服务端主通道图像；1为服务端子通道图像）
	pXClientInfo[subChannel]->m_nPort = proInfo->port;//5050; //接收端口号
	pXClientInfo[subChannel]->m_szIPAddress = (char*)calloc(MAX_USE_LEN, 1);
	memset(pXClientInfo[subChannel]->m_szIPAddress, 0, MAX_USE_LEN);
	memcpy(pXClientInfo[subChannel]->m_szIPAddress, proInfo->IpAddr, strlen(proInfo->IpAddr)); //服务端IP地址

	pXClientInfo[subChannel]->m_szUserName = (char*)calloc(MAX_USE_LEN, 1);
	memset(pXClientInfo[subChannel]->m_szUserName, 0, MAX_USE_LEN);
	memcpy(pXClientInfo[subChannel]->m_szUserName, proInfo->userName, strlen(proInfo->userName)); //用户名
	
	pXClientInfo[subChannel]->m_szUserPassword = (char*)calloc(MAX_USE_LEN, 1);
	memset(pXClientInfo[subChannel]->m_szUserPassword, 0, MAX_USE_LEN);
	memcpy(pXClientInfo[subChannel]->m_szUserPassword, proInfo->password, strlen(proInfo->password)); //密码
	
	pXClientInfo[subChannel]->m_bUserCheck = 1; //是否进行用户校验
	pXClientInfo[subChannel]->m_hShowVideo = inst->hWnd;//显示区域
	
	while(1)
	{
		int i = 0;
		while (i<MAX_CHANNEL)
		{	
			if (getChannelState(i) > 0 && channelList[i].IsFirst)
			{	
				channelList[i].IsFirst = FALSE;
				HCInfo[i].m_lStockHandle = X_ClientStart(pXClientInfo[i], lUserID, wType);
				HCInfo[i].type = wType;
				HCInfo[i].channel = i;
				fprintf(stderr, "HCInfo[%d].m_lStockHandle=%d,channel = %d\n", i, HCInfo[i].m_lStockHandle, pXClientInfo[i]->m_bytRemoteChannel);
				X_ClientSetCapPicCallBack(CapPicFun);
				reclaimChannelState(i);
			}
			i++;
		}
		Sleep(1000);
	}	
}

unsigned long ThreadCallback(unsigned char *channel)
{
	unsigned char subChannel = *channel;
#ifdef __WIN32__
	__try{
#endif
		ThreadCallbackEntry(subChannel);
#ifdef __WIN32__
		}
	__except(RecordExceptionInfo(GetExceptionInformation(), 
		"HikData.cpp - _Entry"))
	{
	}
#endif
	return TRUE;
}

CRITICAL_SECTION *getCritialSection(void)
{
	return &criticalSection;
}

void getFrameByHikSDK(unsigned char channelTotal)
{
	InitializeCriticalSection(&criticalSection);
	initChannelList();
	initFrameData(&frameQueue);
	static unsigned char  j = 0;

	HikThreadFuncCallback ThreadFunc = (HikThreadFuncCallback)ThreadCallback;
	for (int i=0; i<channelTotal; i++)
	{
		j = i;
		HCInfo[i].hThread = CreateThread( NULL,  
			0, 
			ThreadFunc, 
			&j,
			0,
			&HCInfo[i].threadID);   
		Sleep(1000);
	}
}
