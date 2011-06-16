/*! 
 * \copyright Copyright(c)200x, All right reserved
 * \brief 管理所有socket连接，只有一个到服务器的连接，多个到本地不同端口的连接;
使用select监听端口的活动,并向主程序报告;
 * \author Liu jiaxiang
 * \version  0.0.1
 * \date   2010年01月12日
 */


#ifndef  SOCKETMANAGER_H_INC
#define  SOCKETMANAGER_H_INC

#include <sys/types.h>
#include <sys/socket.h>

#define  SocketNum      100

struct SocketManager {
	int sockets[ SocketNum ];
};

extern int sm_init();
extern int sm_destroy();
extern int sm_add( int sock );
extern int sm_remove( int sock );
extern int sm_getReady();
extern int sm_getWritable( int sock );
extern int sm_createConnect( char* srvAddr, int port );

extern int sm_addToFdset( fd_set *set );
extern int sm_getReadySocket( fd_set *set);
extern int sm_tcpConnect(const char *host, const char *serv);

#endif   /* ----- #ifndef socketmanager_h_INC  ----- */
