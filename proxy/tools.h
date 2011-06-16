/*! 
 * \copyright Copyright(c)200x, All right reserved
 * \brief 
 * \author Liu jiaxiang
 * \version  0.0.1
 * \date   2009年12月31日
 */


#ifndef  TOOLS_H_INC
#define  TOOLS_H_INC

#include <sys/times.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "system.h"

#define MAX_BUF_SIZE      1024
#define MAX_ADDR_LEN      512

#define INVALID_SROUCE_ID         -1
#define INVALID_FD                -1


#define showmsg printf

enum DebugLevel
{
	DBG_LOG,
	DBG_ERR,
	DBG_WARING,
	DBG_INFO,
	DBG_DEBUG
};

typedef struct DataPack{
	char     pytype[2];
	uint16_t srcid;
	uint16_t pysize;
	char*    data;
}DataPack;

extern int Debug_Level;

#define Debug(Level,fmt,...) \
		do{ \
		 if( Level < Debug_Level ){ \
				 savelog("%d:%s %d:"fmt"\n",Level,__FILE__,__LINE__, ##__VA_ARGS__);}\
		  else { showmsg("%d:%s %d:"fmt"\n",Level,__FILE__,__LINE__, ##__VA_ARGS__);} \
		 }while(0)

#if 0
unsigned int get_uptime()
{
	return times( NULL) /  sysconf( _SC_CLK_TCK );
}

static unsigned int get_uptime_ms()
{
	float ut = sysconf( _SC_CLK_TCK ) / 1000.0;
	return times(NULL) / ut;
}
#else
#  define get_uptime() (times( NULL) /  sysconf( _SC_CLK_TCK ))
#endif

extern size_t writen(int fd, const void *vptr, size_t n);

#endif   /* ----- #ifndef tools_h_INC  ----- */
