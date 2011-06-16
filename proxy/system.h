/*! 
 * \copyright Copyright(c)200x, All right reserved
 * \brief 需要根据系统不同而修改的一些功能函数
 * \author Liu jiaxiang
 * \version  0.0.1
 * \date   2010年01月12日
 */



#ifndef  system_h_INC
#define  system_h_INC

#define  SOCKET_TIMEOUT      30 * 2         //socket 连接超时的秒数


#define savelog printf

extern unsigned int sys_getTime();
extern int sys_getLocalMac( unsigned char* buf, int size );
extern int sys_getLocalSerial( unsigned char* buf, int size );
extern int sys_getProtoProfile( unsigned char* buf, int size );
extern int sys_getServerAddr( char* buf, int size );
extern int sys_getServerPort();
extern int sys_getActServerAddr( char* buf, int size );
extern int sys_getActServerPort();
extern int sys_setActServerAddr( char* addr  );
extern int sys_setActServerPort( int port );


#endif   /* ----- #ifndef system_h_INC  ----- */

