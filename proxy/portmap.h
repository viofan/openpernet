/*! 
 * \copyright Copyright(c)200x, All right reserved
 * \brief 端口与协议的对应表
 * \author Liu jiaxiang
 * \version  0.0.1
 * \date   2010年01月12日
 */


#ifndef  PORTMAP_H_INC
#define  PORTMAP_H_INC

#define MAX_PORT  100
struct PortMap {
	char* portList[MAX_PORT];
};

extern int pm_loadProfile();
extern int pm_getProtoPort( char* proto );


#endif   /* ----- #ifndef portmap_h_INC  ----- */
