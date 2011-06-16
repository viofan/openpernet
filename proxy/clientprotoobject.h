/*! 
 * \copyright Copyright(c)200x, All right reserved
 * \brief 
 * \author Liu jiaxiang
 * \version  0.0.1
 * \date   2010年01月12日
 */


#ifndef  CLIENTPROTOOBJECT_H_INC
#define  CLIENTPROTOOBJECT_H_INC

#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct RDataPack{
	uint16_t  srcId;
	uint16_t  pyLen;
}__attribute__ ((packed)) RDataPack;

typedef struct ClientProtoObject {
	char proto[3];
	uint16_t sourceId;
	int clientPort;
	int clientSocket;
	char *data;          //返回给服务器的数据
	RDataPack *dpdata;   //指向data
	char *buf;           //指向data + sizeof(RDataPack);
	int bufPos;
	int lastKeeplive;
	struct ClientProtoObject *prev;
	struct ClientProtoObject *next;
}ClientProtoObject;

//extern ClientProtoObject* cpo_create( int sourceId, char* proto );
extern int cpo_init(ClientProtoObject* cpObj, uint16_t sourceId, char* proto, int sock );
extern int cpo_destory( ClientProtoObject* cpObj );
extern int cpo_sendData( ClientProtoObject* cpObj, char* data, int size );
extern int cpo_recvData( ClientProtoObject* cpObj );
extern int cpo_clearData( ClientProtoObject* cpObj, int size );
#endif   /* ----- #ifndef clientprotoobject_h_INC  ----- */
