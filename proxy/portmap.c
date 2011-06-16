/*! 
 * \copyright Copyright(c)200x, All right reserved
 * \brief 
 * \author Liu jiaxiang
 * \version  0.0.1
 * \date   2010年01月12日
 */

#include <stdlib.h>
#include <string.h>
#include "portmap.h"
#include "tools.h"
#include "system.h"

static struct PortMap g_portMap;

int pm_loadProfile()
{
	memset( g_portMap.portList, 0, sizeof( g_portMap.portList ));
	char buf[MAX_BUF_SIZE] = {0};

	sys_getProtoProfile( buf, MAX_BUF_SIZE );
	if( strlen( buf ) > 0 )
	{
		char *delim=";";
		char *p;
		char *val;
		char *str = buf;
		int pos = 0;
		while( (p = strtok(str,delim)) && pos < MAX_PORT )
		{
			val = (char*)calloc( strlen(p)+1, 1 );
			if( val )
			{
				strcpy( val, p );
				g_portMap.portList[pos] = val;
				fprintf(stderr, "port = %s\n", val);
			}

			++pos;
			str = NULL;
		}
	}
	return 0;
}

int pm_getProtoPort( char* proto )
{
	int ret = -1;
	int pos = 0;
	while( pos < MAX_PORT )
	{
		if( g_portMap.portList[pos])
		{
			char* aval = g_portMap.portList[pos];
			if( proto[0] == aval[0]
				&& proto[1] == aval[1])
			{
				ret = atoi( aval+3 );
				Debug(DBG_DEBUG, "Get Proto port:%d ", ret );
				break;
			}
		}
		else
			break;

		++pos;
	}

	return ret;
}
