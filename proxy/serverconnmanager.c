/*! 
 * \copyright Copyright(c)200x, All right reserved
 * \brief 
 * \author Liu jiaxiang
 * \version  0.0.1
 * \date   2010年01月12日
 */


#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include "serverconnmanager.h"
#include "tools.h"
#include "socketmanager.h"
#include "portmap.h"
#include "system.h"

static struct ServerConnManager g_srvConnMng;

int scm_init()
{
	memset( &g_srvConnMng, 0, sizeof(struct ServerConnManager));
	g_srvConnMng.srvSocket = -1;
	g_srvConnMng.buf = (char*)calloc( SRV_DATA_BUF_SIZE , 1 );

	char addbuf[MAX_ADDR_LEN] = {0};
	sys_getServerAddr( addbuf, MAX_ADDR_LEN );
	strncpy( g_srvConnMng.defSrvAddr, addbuf, MAX_ADDR_LEN );
	g_srvConnMng.defSrvPort = sys_getServerPort();

	memset( addbuf, 0, MAX_ADDR_LEN );
	sys_getActServerAddr( addbuf, MAX_ADDR_LEN );
	strncpy( g_srvConnMng.actSrvAddr, addbuf, MAX_ADDR_LEN );
	g_srvConnMng.actSrvPort = sys_getActServerPort();

	return 0;
}

int scm_createConnect()
{
	Debug(DBG_DEBUG, "create connect to server.");
	if( g_srvConnMng.srvSocket >= 0 )
		scm_closeConnect();

	if( strlen(g_srvConnMng.actSrvAddr) > 10 )
		g_srvConnMng.srvSocket = sm_createConnect( g_srvConnMng.actSrvAddr,
												   g_srvConnMng.actSrvPort );

	if( g_srvConnMng.srvSocket < 0 )
		g_srvConnMng.srvSocket = sm_createConnect( g_srvConnMng.defSrvAddr,
												   g_srvConnMng.defSrvPort );

	if( g_srvConnMng.srvSocket >= 0 )
	{
		g_srvConnMng.lastKeeplive = sys_getTime();
		scm_register();
	}

	return 0;
}

int scm_closeConnect()
{
	if( g_srvConnMng.srvSocket >= 0 )
	{
		sm_remove( g_srvConnMng.srvSocket );
		g_srvConnMng.srvSocket = -1;
	}
	return 0;
}

int scm_reconnect()
{
	scm_closeConnect();
	scm_createConnect();
	return 0;
}

int scm_sendData( unsigned char* data, int size )
{
	int ret = -1;
	if( g_srvConnMng.srvSocket >= 0 )
	{
		if( sm_getWritable( g_srvConnMng.srvSocket ) == g_srvConnMng.srvSocket )
		{
			ret = writen( g_srvConnMng.srvSocket, data, size );
		}
	}
	Debug(DBG_DEBUG, "Send data to server, size:%d", ret );
	return ret;
}

int scm_sendNullData( int sourceId, char* proto )
{
	char buf[4];
	uint16_t srcId = htons( sourceId );
	buf[0] = srcId >> 8;
	buf[1] = srcId;
	buf[2] = 0;
	buf[3] = 0;

	scm_sendData( buf, 4 );

	return 0;
}

int scm_getDataPacket( unsigned char* buf, int size )
{
	int datasize = 0;
	int len = 0;
	static unsigned int total_size = 0;

	if( g_srvConnMng.bufPos > 4 )
	{
		if( scm_procHeartbeat() < 0 )
		{
			if( scm_procRedirect() < 0 )
			{
				//if( g_srvconnmng.buf[0] == 'h' && g_srvconnmng.buf[1] == 't' )
				{
					int port = pm_getProtoPort( g_srvConnMng.buf );
					Debug(DBG_INFO, "recevie proto port %d data from server.", port);
					if( port >= 0 )
					{
						DataPack ndp;
						memcpy( &ndp, g_srvConnMng.buf, 6 );

						len = ntohs( ndp.pysize );
						datasize = len+6;
						//fprintf(stderr, "datasize = %d, bufpos = %d\n", datasize, g_srvConnMng.bufPos);
						if( datasize >= size )
						{
						//	Debug(DBG_WARING, "payload too much data:%d", datasize );
							datasize = size-1;
						}
						if( g_srvConnMng.bufPos >= datasize )
						{
							total_size += len;
							//fprintf(stderr, "total_size = %d\n", total_size);
							memcpy( buf, g_srvConnMng.buf, datasize );
							//Debug(DBG_INFO, "payload size: %d, bufpos=%d", len, g_srvConnMng.bufPos);
#if 0
							int fp = fopen("./file.txt", "a+");
							if(fp < 0)
							{
								fprintf(stderr, "fopen is error.\n");
								return ;
							}
							fwrite(buf+6, 1, len, fp);
							fclose(fp);
#endif

							g_srvConnMng.bufPos -= datasize;
							memmove( g_srvConnMng.buf, g_srvConnMng.buf + datasize , g_srvConnMng.bufPos );
						}
						else
						{
							return 0;
						}
					}
				}
			}
		}
	}
	return datasize;
}

int scm_readData()
{
	int rn = read( g_srvConnMng.srvSocket, g_srvConnMng.buf+g_srvConnMng.bufPos,
					SRV_DATA_BUF_SIZE - g_srvConnMng.bufPos -10);


	if( rn == 0 )
	{
		Debug(DBG_INFO, "Server close connect.");
		scm_closeConnect();
	}
	else if( rn < 0 )
	{
		if( errno != EINTR )
		{
			Debug(DBG_INFO, "Server connect read error,by close.");
			scm_closeConnect();
		}
	}
	else
	{
		g_srvConnMng.bufPos += rn;
		g_srvConnMng.lastKeeplive = sys_getTime();
	}

	Debug(DBG_DEBUG, "Read %d byte data from server, bufpos=%d.", rn,g_srvConnMng.bufPos  );

	return 0;
}

int scm_procHeartbeat()
{
	int ret = -1;
	if( g_srvConnMng.buf[0] == 'h' && g_srvConnMng.buf[1] == 'b' )
	{
		unsigned short sourceId;
		sourceId = ntohs( *(g_srvConnMng.buf + 2 ));
		if( sourceId == 0 )
		{
			g_srvConnMng.lastKeeplive = sys_getTime();
			ret = 0;

			char rbrsp[6];
			memset( rbrsp, 0, 6 );
			rbrsp[0] = 0;//'h';
			rbrsp[1] = 0;//'b';
			scm_sendData( rbrsp, 4 );

			Debug(DBG_DEBUG, "Receive heartbeat data, bufpos=%d.",g_srvConnMng.bufPos);
		}

		memmove( g_srvConnMng.buf, g_srvConnMng.buf + 6, g_srvConnMng.bufPos - 6 );
		g_srvConnMng.bufPos -= 6;
	}
	return ret;
}

int scm_procRedirect()
{
	int ret = -1;
	int i = 0;
	if( g_srvConnMng.buf[0] == 0 && g_srvConnMng.buf[1] == 0 )
	{
		uint16_t dstId;
		dstId = g_srvConnMng.buf[0] << 8 | g_srvConnMng.buf[1];
		dstId = ntohs( dstId );
		fprintf(stderr, "redirect.\n");
		if( dstId == 0 )
		{
			//uint32_t drSrvAddr =  (uint32_t)*(g_srvConnMng.buf + 2);
			//uint16_t drSrvPort = (uint16_t)*(g_srvConnMng.buf+6) ;
			uint32_t drSrvAddr = ((g_srvConnMng.buf[2] << 24)&0xff000000) | ((g_srvConnMng.buf[3] << 16)&0x00ff0000) |
								 ((g_srvConnMng.buf[4] << 8)&0x0000ff00) | ((g_srvConnMng.buf[5])&0x000000ff);
			uint16_t drSrvPort = g_srvConnMng.buf[6] << 8 | g_srvConnMng.buf[7];
			fprintf(stderr, "drSrvAddr = %x\n", drSrvAddr);
			fprintf(stderr, "drSrvPort = %x\n", drSrvPort);
			drSrvAddr = ntohl( drSrvAddr);
			//drSrvPort = ntohs(drSrvPort); 
			char addBuf[64] = {0};
			char* addr = inet_ntop( AF_INET, &drSrvAddr, addBuf, INET_ADDRSTRLEN );
			fprintf(stderr, "addr = %s\n", addr);
			fprintf(stderr, "drSrvPort = %d\n", drSrvPort);
			if( addr != NULL )
			{
				strcpy( g_srvConnMng.actSrvAddr, addr );
				g_srvConnMng.actSrvPort = drSrvPort;

				sys_setActServerAddr( addr );
				sys_setActServerPort( drSrvPort );
				scm_reconnect();
			}
			memmove( g_srvConnMng.buf, g_srvConnMng.buf + 8, g_srvConnMng.bufPos - 8 );
			g_srvConnMng.bufPos -= 8;
			ret = 0;
			Debug(DBG_DEBUG, "Receive redirect server data.");
		}
	}
	return ret;
}

int scm_isServerSocket( int sock )
{
	return sock == g_srvConnMng.srvSocket;
}

int scm_checkConnect()
{
	if((sys_getTime()- g_srvConnMng.lastKeeplive) >= SOCKET_TIMEOUT )
	{
		Debug(DBG_WARING, "Server connect timeout, reconnect.");
		scm_reconnect();
		g_srvConnMng.lastKeeplive = sys_getTime();
	}
	return 0;
}

int scm_register()
{
	unsigned char regPack[23] = {0};
	char mac[32] = {0};
	char serial[32] = {0};

	sys_getLocalMac( mac, 32 );
	sys_getLocalSerial( serial, 32 );

	char *delim=":";
	char *p;
	char *str = mac;
	int pos = 0;
	while((p = strtok(str,delim)) && pos < 6 )
	{
		regPack[pos] = (unsigned char)strtol( p, NULL, 16 );

		++pos;
		str = NULL;
	}

	int idx = 0;
	char buf[2] = {0};
	while( serial[idx] && idx < 16  )
	{
		//buf[0] = serial[idx++] ;
		//regPack[pos++] =  (unsigned char)strtol( buf, NULL, 16 );
		regPack[pos++] = serial[idx++];
	}

	scm_sendData( regPack, 22 );
	Debug(DBG_DEBUG, "Send register data.");
}
