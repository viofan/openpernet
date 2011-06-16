/*! 
 * \copyright Copyright(c)200x, All right reserved
 * \brief 管理所有连接到本地端口的连接及数据收发
 * \author Liu jiaxiang
 * \version  0.0.1
 * \date   2010年01月12日
 */


#ifndef  CLIENTMANAGER_H_INC
#define  CLIENTMANAGER_H_INC

#include "clientprotoobject.h"

#define PROTOOBJ_COUNT     20

extern char targethost[128];

struct ClientManager {
	//ClientProtoObject cpObject[PROTOOBJ_COUNT];
	ClientProtoObject* cpObject;
	int lastCheckTime;
};

extern int cm_init();
extern int cm_add( ClientProtoObject* cpobj );
extern int cm_remove( ClientProtoObject* cpobj );
//extern int cm_getProtoSourceId( char* proto );
//extern ClientProtoObject* cm_getClientObjBySource( int sourceId );
extern ClientProtoObject* cm_getClientObj( int srcId, char* proto );
extern ClientProtoObject* cm_getClientObjBySock( int sock );
extern int cm_sendDataToClient( char* proto, int sourceId, char* data, int size );
extern ClientProtoObject* cm_createConnToClient( char* proto, int sourceId );
extern int cm_closeClientConn( int sock );
extern void cm_checkClientTimeout();
extern int cm_procClientRepData( int sock );


#endif   /* ----- #ifndef clientmanager_h_INC  ----- */
