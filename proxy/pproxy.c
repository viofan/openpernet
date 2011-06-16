/*! 
 * \copyright Copyright(c)200x, All right reserved
 * \brief 中转服务器协议的前端设备协议代理程序
 * \author Liu jiaxiang
 * \version  0.0.1
 * \date   2010年01月12日
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <errno.h>
#include "tools.h"
#include "portmap.h"
#include "socketmanager.h"
#include "serverconnmanager.h"
#include "clientprotoobject.h"
#include "clientmanager.h"
#include "signal.h"


int init()
{
	pm_loadProfile();
	sm_init();
	scm_init();
	scm_createConnect();
	return 0;
}

int procSrvToClientData()
{

	//static char databuf[ MAX_BUF_SIZE * 4];
	static char databuf[ SRV_DATA_BUF_SIZE];
	static char proto[3];
	static uint16_t pyLen = 0;

	memset( databuf, 0, SRV_DATA_BUF_SIZE );
	proto[2] = 0;
	int datasize = scm_getDataPacket( databuf, SRV_DATA_BUF_SIZE);
	if( datasize > 0 )
	{
		DataPack ndp;
		memcpy( (void*)&ndp, databuf, 6 );
		ndp.data = databuf + 6;

		proto[0] = databuf[0];
		proto[1] = databuf[1];

		uint16_t srcId = ntohs( ndp.srcid );
		pyLen = ntohs( ndp.pysize );
		Debug(DBG_DEBUG,"Get server data: proto=%s,srcID=%d, PayLoadSize=%d", proto, srcId, pyLen);
		int send = cm_sendDataToClient( proto, srcId, databuf+6, pyLen );
		if( send < 0 )
		{
			//scm_sendNullData( srcId, proto );
			fprintf(stderr, "errno = %d\n", errno);
			fprintf(stderr, "strerror = %s\n", strerror(errno));
		}
	}
	return 0;
}

int procSocketData()
{
	procSrvToClientData();

	int sock = sm_getReady();
	if( sock >= 0 )
	{
		if( scm_isServerSocket( sock ))
		{
			scm_readData();
			procSrvToClientData( sock );
		}
		else
		{
			cm_procClientRepData( sock );
		}
	}

	return 0;
}

void processSignal(int signal)
{
	//fprintf(stderr, "signal pipe.\n");
}

int main(int argc, char** argv)
{
	int isdaemon = 0;
	int ch;
	opterr = 0;
	memset( targethost, 0, 128 );
	strcpy( targethost, "localhost" );
	//signal(SIGPIPE, processSignal);
	signal(SIGPIPE, SIG_IGN);
	while(( ch = getopt( argc, argv, "dg:h:" )) != -1 )
	{
		if( ch == 'd' )
		{
			isdaemon = 1;
			break;
		}
		else if( ch == 'g' )
		{
			Debug_Level = atoi(optarg);
		}
		else if( ch == 'h')
		{
			strncpy( targethost, optarg, 127 );
		}
	}

	if( isdaemon == 1 )
	{

		char *pid_file = "/var/run/pproxy.pid";

		{
			FILE *pid_fp;
			/* Daemonize and log PID */
			if (daemon(0, 0) == -1)
			{
				perror("PProxy daemon");
				exit(errno);
			}
			if (!(pid_fp = fopen(pid_file, "w")))
			{
				perror(pid_file);
				return errno;
			}
			fprintf(pid_fp, "%d", getpid());
			fclose(pid_fp);
		}

	}

	init();

	while( 1 )
	{
		procSocketData();

		scm_checkConnect();
		cm_checkClientTimeout();
	}

	return 0;
}

