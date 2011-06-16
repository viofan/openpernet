/*! 
 * \copyright Copyright(c)200x, All right reserved
 * \brief 
 * \author Liu jiaxiang
 * \version  0.0.1
 * \date   2010年01月12日
 */

#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include "tools.h"
#include "clientprotoobject.h"
#include "portmap.h"
#include "socketmanager.h"
#include "system.h"


int cpo_init(ClientProtoObject* cpObj, uint16_t sourceId, char* proto, int sock )
{

	if( cpObj->data )
		free( cpObj->data );

	memset( cpObj, 0, sizeof( cpObj ));
	cpObj->sourceId = sourceId,
	cpObj->clientSocket = sock;
	strncpy( cpObj->proto, proto, 2 );
	cpObj->clientPort = pm_getProtoPort( proto );

	cpObj->data = (char*)calloc( MAX_BUF_SIZE * 4 + 8, 1 );
	cpObj->dpdata = cpObj->data;
	cpObj->dpdata->srcId = htons( sourceId );
	cpObj->dpdata->pyLen = 0;
	cpObj->buf = cpObj->data + 4;

	cpObj->lastKeeplive = sys_getTime();
	return 0;
}

int cpo_destory( ClientProtoObject* cpObj )
{
	if( cpObj->data )
		free( cpObj->data );
	cpObj->data = NULL;
	memset( cpObj, 0, sizeof( cpObj));

	cpObj->clientSocket = INVALID_FD;

	return 0;
}

int cpo_sendData( ClientProtoObject* cpObj, char* data, int size )
{
	int ret = 0;
	if( sm_getWritable( cpObj->clientSocket ) == cpObj->clientSocket )
	{
		ret = writen( cpObj->clientSocket, data, size );
		cpObj->lastKeeplive = sys_getTime();
		fprintf(stderr, "write is lastKeeplive = %d\n", cpObj->lastKeeplive);
	}
	return ret;
}

int cpo_recvData( ClientProtoObject* cpObj )
{
	int rn = read( cpObj->clientSocket, cpObj->buf,	MAX_BUF_SIZE * 4 );
	if( rn > 0 )
	{
		cpObj->bufPos = rn;
		cpObj->dpdata->pyLen = htons( (uint16_t)rn );
		cpObj->dpdata->srcId = htons( (uint16_t)cpObj->sourceId );
		cpObj->lastKeeplive = sys_getTime();
		Debug(DBG_DEBUG, "Receive %d byte data from client.", rn );
	}
	else
	{
		Debug(DBG_DEBUG, "Receive client data error:%d", errno );
	    fprintf(stderr, "strerrno = %s\n", strerror(errno));
	}

	return rn;
}

int cpo_clearData( ClientProtoObject* cpObj, int size )
{
	memset( cpObj->buf + 4 , 0, cpObj->bufPos );
	cpObj->bufPos = 0;

	return size;
}
