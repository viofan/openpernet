/*! 
 * \copyright Copyright(c)200x, All right reserved
 * \brief 
 * \author Liu jiaxiang
 * \version  0.0.1
 * \date   2010年01月12日
 */

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "clientmanager.h"
#include "clientprotoobject.h"
#include "socketmanager.h"
#include "portmap.h"
#include "system.h"
#include "serverconnmanager.h"

static struct ClientManager g_clientMngList;

char targethost[128];

int cm_init()
{
	g_clientMngList.lastCheckTime = sys_getTime();
	return 0;
}

int cm_add( ClientProtoObject* cpobj )
{
	if( g_clientMngList.cpObject == NULL )
	{
		g_clientMngList.cpObject = cpobj;
	}
	else
	{
		ClientProtoObject *pos  = g_clientMngList.cpObject;
		for( ; pos ; pos = pos->next )
		{
			if( pos->next == NULL )
			{
				pos->next = cpobj;
				cpobj->prev = pos;
				break;
			}
		}
	}

	return 0;
}

int cm_remove( ClientProtoObject* cpobj )
{
	if( cpobj == g_clientMngList.cpObject )
	{
		g_clientMngList.cpObject = cpobj->next;
		if( g_clientMngList.cpObject )
			g_clientMngList.cpObject->prev = NULL;
	}
	else
	{
		cpobj->prev->next = cpobj->next;
		if( cpobj->next )
			cpobj->next->prev = cpobj->prev;
	}
	free( cpobj );
	return 0;
}

//int cm_getProtoSourceId( char* proto )
//{
//}
//
//ClientProtoObject* cm_getClientObjBySource( int sourceId )
//{
//}

ClientProtoObject* cm_getClientObj( int srcId, char* proto )
{
	ClientProtoObject *cobj = NULL;

	ClientProtoObject *pos  = g_clientMngList.cpObject;
	for( ; pos ; pos = pos->next )
	{
		if( pos->sourceId == srcId )
		{
			if( strncmp( pos->proto, proto, 2 ) == 0 )
			{
				cobj = pos;
				break;
			}
		}
	}
	return cobj;
}

ClientProtoObject* cm_getClientObjBySock( int sock )
{
	ClientProtoObject *cobj = NULL;

	ClientProtoObject *pos  = g_clientMngList.cpObject;
	for( ; pos ; pos = pos->next )
	{
		if( pos->clientSocket == sock )
		{
			cobj = pos;
			break;
		}
	}
	return cobj;
}

int cm_sendDataToClient( char* proto, int sourceId, char* data, int size )
{
	Debug(DBG_DEBUG, "Wall send data to client, proto=%s, srcId=%d, datasize=%d",
					proto, sourceId, size );
	int ret = -1;
	ClientProtoObject *cobj = cm_createConnToClient( proto, sourceId );
	if( cobj )
	{
		ret = cpo_sendData( cobj, data, size );
		if( ret < 0 )
		{
			if( errno != EINTR )
			{
				Debug(DBG_WARING, "Send data to client fial.");
				cm_closeClientConn( cobj->clientSocket );
			}
		}
		else
		{
			Debug(DBG_DEBUG, "Send data %d byte to client, socket=%d.", ret, cobj->clientSocket);
		}
	}
	else
		Debug(DBG_WARING, "Send data to client fail, con't create client connect.");

	return ret;
}

ClientProtoObject* cm_createConnToClient( char* proto, int sourceId )
{
	ClientProtoObject *cobj = NULL;
	cobj = cm_getClientObj( sourceId, proto );
	if( cobj == NULL)
	{
		int port = pm_getProtoPort( proto );
		int sock = sm_createConnect( targethost, port );
		if( sock >= 0 )
		{
			cobj = (ClientProtoObject*)calloc( sizeof( ClientProtoObject), 1 );
			cpo_init( cobj, sourceId, proto, sock );
			cm_add( cobj );

			Debug(DBG_DEBUG, "Create client object: proto=%s, srcId=%d", proto, sourceId);
		}
		else
			Debug( DBG_WARING, "Connect to client port fail, port=%d", port);
	}
	return cobj;
}

int cm_closeClientConn( int sock )
{
	ClientProtoObject* cobj = cm_getClientObjBySock( sock );
	if( cobj )
	{
		cm_remove( cobj );
	}

	sm_remove( sock );
	return 0;
}

void cm_checkClientTimeout()
{
	if(( sys_getTime() - g_clientMngList.lastCheckTime) > 5)
	{
		g_clientMngList.lastCheckTime = sys_getTime();
		//Debug(DBG_DEBUG, "check client timeout.");
		Debug(DBG_DEBUG, "sys_getTime = %d\n", sys_getTime);
		ClientProtoObject *pos  = g_clientMngList.cpObject;
		for( ; pos ; pos = pos->next )
		{
			Debug(DBG_DEBUG, "client keeplive:%d\n", pos->lastKeeplive );
			if((sys_getTime() - pos->lastKeeplive ) >= 20 )
			{
				cpo_clearData( pos, pos->bufPos );
				pos->dpdata->pyLen = 0;
				scm_sendData( pos->data, 4 );
				Debug(DBG_DEBUG, "Client socket %d timeout, by close.", pos->clientSocket);
				cm_closeClientConn( pos->clientSocket );
				//cm_remove( pos );
				break;
			}
		}
	}
}

int cm_procClientRepData( int sock )
{
	ClientProtoObject* cobj = cm_getClientObjBySock( sock );
	if( cobj )
	{
		if( cpo_recvData( cobj ) <= 0 )  //接收出错
		{
			cpo_clearData( cobj, cobj->bufPos );
			cobj->dpdata->pyLen = 0;
			scm_sendData( cobj->data, 4 );
			Debug(DBG_DEBUG, "Receive client data fail, close socket.");
			cm_closeClientConn( cobj->clientSocket );
		}
		else
		{
			Debug(DBG_DEBUG, "Send data to servier from client.srcid=%d,size=%d", cobj->sourceId, cobj->bufPos );
			Debug(DBG_DEBUG, "To server, Heard=%02X %02X %02X %02X.",
				  cobj->data[0], cobj->data[1],cobj->data[2],cobj->data[3] );
			scm_sendData( cobj->data, cobj->bufPos + 4 );
			cpo_clearData( cobj, cobj->bufPos );
		}
	}
	return 0;
}
