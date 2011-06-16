/*! 
 * \copyright Copyright(c)200x, All right reserved
 * \brief 
 * \author Liu jiaxiang
 * \version  0.0.1
 * \date   2010年01月12日
 */


#ifndef  SERVERCONNMANAGER_H_INC
#define  SERVERCONNMANAGER_H_INC

#include "tools.h"

#define  SRV_DATA_BUF_SIZE   (1024 * 8)

struct ServerConnManager{
	int srvSocket;
	int lastKeeplive;
	char defSrvAddr[MAX_ADDR_LEN];
	int  defSrvPort;
	char actSrvAddr[MAX_ADDR_LEN];
	int  actSrvPort;
	char* buf;
	int bufPos;  //buf中有数据长度
};

extern int scm_init();
extern int scm_createConnect();
extern int scm_closeConnect();
extern int scm_reconnect();
extern int scm_sendData( unsigned char* data, int size );
extern int scm_sendNullData( int sourceId, char* proto );
extern int scm_getDataPacket( unsigned char* buf, int size );
extern int scm_readData();
extern int scm_procHeartbeat();
extern int scm_procRedirect();
extern int scm_isServerSocket( int sock );
extern int scm_checkConnect();
extern int scm_register();


#endif   /* ----- #ifndef serverconnmanager_h_INC  ----- */
