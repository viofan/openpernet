/*! 
 * \copyright Copyright(c)200x, All right reserved
 * \brief 
 * \author Liu jiaxiang
 * \version  0.0.1
 * \date   2010年01月12日
 */


#include <stdlib.h>
#include <string.h>
#include "system.h"
#include "tools.h"

int Debug_Level  = 2;

unsigned int sys_getTime()
{
	return get_uptime();
}

int sys_getLocalMac( unsigned char* buf, int size )
{
	if( buf )
	{
		strncpy( buf, "00:01:02:03:04:55", 17 );
	}
	return 0;
}

int sys_getLocalSerial( unsigned char* buf, int size )
{
	if( buf )
	{
		strncpy(buf, "75AB3C0D26C60AF0", strlen("75AB3C0D26C60AF0") );
	}
	return 0;
}

int sys_getProtoProfile( unsigned char* buf, int size )
{
	strcpy( buf, "ft=21;rt=554;vo=666;up=5000;ht=8080");
	return strlen( buf );
}

int sys_getServerAddr( char* buf, int size )
{
	if( buf )
	{
		strncpy(buf, "192.168.10.121", strlen("192.168.10.121") );
		//strncpy(buf, "219.235.235.4", strlen("219.235.235.4") );
	}
	return 0;
}

int sys_getServerPort()
{
	return 10832;
}

int sys_getActServerAddr( char* buf, int size )
{
	return 0;
}

int sys_getActServerPort()
{
	return 0;
}

int sys_setActServerAddr( char* addr  )
{
	if( addr )
	{
	}
	return 0;
}

int sys_setActServerPort( int port )
{
	char buf[32] = {0};
	sprintf(buf, "%d", port );
	return 0;
}
