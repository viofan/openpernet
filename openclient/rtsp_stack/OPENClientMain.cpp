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
// test program
//

#include "RTSPSdk.h"
#include "wtypes.h"

typedef struct _DevCamerasInfo
{
	char userName[20];
	char password[20];
	char mac[20];
	unsigned char camId;
}DevCamerasInfo;

void (CALLBACK CapFrameFun)(void *handle,char * pBuf,long nSize,long nWidth,long nHeight)
{
#if 0
	FILE *stream_fp;
	char name[128];
	static int cnt = 0;
	sprintf(name, "streamStream%d.yuv", cnt++);
	stream_fp = fopen(name, "wb");
	fwrite(pBuf, 1, nWidth * nHeight * 3 / 2, stream_fp);
	fclose(stream_fp);
#endif
}

int main(int argc, char *argv[])
{
	DevCamerasInfo devC;
	void* m_StockHandle = NULL;

	memset((void*)&devC, 0, sizeof(DevCamerasInfo));
	memcpy(devC.userName, argv[1], strlen(argv[1]));
	memcpy(devC.password, argv[2], strlen(argv[2]));
	memcpy(devC.mac, argv[3], strlen(argv[3]));
	devC.camId = *argv[4];
	initBasicUsageEnvironment();
	m_StockHandle = startPlaying(devC.userName, devC.password, devC.mac, devC.camId);
	registerFrameCallback(m_StockHandle, CapFrameFun);
	activeTaskSchedulerLoop();
	return 0;
}
