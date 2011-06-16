/*! 
 * \copyright Copyright(c)200x, All right reserved
 * \brief 
 * \author Liu jiaxiang
 * \version  0.0.1
 * \date   2010年01月12日
 */

#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "socketmanager.h"
#include "tools.h"


static struct SocketManager g_sockList;

int sm_init()
{
	int i=0;
	for(; i< SocketNum; ++ i)
		g_sockList.sockets[i] = INVALID_FD;

	return 0;
}

int sm_destroy()
{
	int i=0;
	for(; i< SocketNum; ++ i)
	{
		if( g_sockList.sockets[i] != INVALID_FD )
		{
			close( g_sockList.sockets[i] );
		}
	}

	return 0;
}

int sm_add( int sock )
{
	int i=0;
	for(; i< SocketNum; ++ i)
	{
		if( g_sockList.sockets[i] == INVALID_FD )
		{
			g_sockList.sockets[i] = sock;
			break;
		}
	}
	if( i >= SocketNum )
		Debug(DBG_WARING, "Add socket too many.");

	return 0;
}

int sm_remove( int sock )
{
	int i=0;
	for(; i< SocketNum; ++ i)
	{
		if( g_sockList.sockets[i] == sock )
		{
			shutdown( sock ,SHUT_RDWR);
			close( sock );
			g_sockList.sockets[i] = INVALID_FD;
			break;
		}
	}

	return 0;
}

int sm_getReady()
{
	int ready = -1;
	int maxFd;
	fd_set  readSet;
	struct timeval timeout;
	int sr;

	maxFd = sm_addToFdset( &readSet );
	if( maxFd >= 0)
	{
		timeout.tv_sec = 0;
		timeout.tv_usec = 500000;  //500毫秒

		//printf(".");fflush( stdout );

		if((sr=select( maxFd+1, &readSet, NULL,NULL, &timeout ))< 0)
		{
			//( errno == EINTER)
			//Debug(DBG_WARING,"select return error, in device_conn_proc_thread.");
		}
		else if( sr > 0 )
		{
			ready = sm_getReadySocket( &readSet );
			if( ready >= 0)
				Debug(DBG_DEBUG,"select return socket ready:%d", ready );
		}
	}
	return ready;
}

int sm_getWritable( int sock )
{
	int ready = -1;
	fd_set  writeSet;
	struct timeval timeout;
	int sr;

	FD_ZERO( &writeSet );
	FD_SET( sock, &writeSet );
	if( sock >= 0)
	{
		timeout.tv_sec = 0;
		timeout.tv_usec = 500000;  //500毫秒

		//printf(".");fflush( stdout );

		if((sr=select( sock+1, NULL, &writeSet, NULL, &timeout ))< 0)
		{
			//( errno == EINTER)
			//Debug(DBG_WARING,"select return error, in device_conn_proc_thread.");
		}
		else if( sr == 1 )
		{
			if( FD_ISSET( sock, &writeSet ))
			{
				ready = sock;
			}
		}
	}
	return ready;
}

int sm_createConnect( char* srvAddr, int port )
{
	int ret = -1;
	if( srvAddr == NULL )
		return ret;
	if( strlen( srvAddr ) <= 0 )
		return ret;

	int sock = -1;

	char portstr[32];
	sprintf(portstr, "%d", port );
	sock = sm_tcpConnect( srvAddr, portstr );
	if( sock >= 0 )
	{
		Debug(DBG_INFO,"connect to host:%s, socket=%d.\n", srvAddr, sock);
		ret = sock;

		int val = fcntl( sock, F_GETFL, 0 );
		fcntl( sock, F_SETFL, val | O_NONBLOCK );

		sm_add( sock );
	}
	else
		Debug(DBG_DEBUG, "tcp connect, server address is empty.");

	return ret;
}

int sm_addToFdset( fd_set *set )
{
	int maxfd = -1;
	int i =0;

	FD_ZERO( set );

	for( i=0 ; i<SocketNum; ++i )
	{
		if( g_sockList.sockets[i] != INVALID_FD )
		{
			FD_SET( g_sockList.sockets[i], set );
			//Debug(DBG_DEBUG, "add socket %d to set.", g_sockList.sockets[i]);
			maxfd = maxfd > g_sockList.sockets[i] ? maxfd : g_sockList.sockets[i];
		}
	}

	return maxfd+1;
}

int sm_getReadySocket( fd_set *set)
{
	int ready = -1;
	int i = 0;
	for( i=0; i< SocketNum; ++i )
	{
		if( g_sockList.sockets[i] != -1 )
		{
			if( FD_ISSET( g_sockList.sockets[i], set ))
			{
				ready = g_sockList.sockets[i];
				break;
			}
		}
	}
	return ready;
}

int sm_tcpConnect(const char *host, const char *serv)
{
	int    sockfd, n;
	struct addrinfo hints, *res, *ressave;

	sockfd = -1;

	if( host == NULL || serv == NULL )
		return sockfd;
	if( strlen( host ) == 0 || strlen( serv ) == 0 )
		return sockfd;

	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if ( (n = getaddrinfo(host, serv, &hints, &res)) != 0)
	{
		Debug(DBG_INFO, "error for %s, %s: %s",	host, serv, gai_strerror(n));
		if( n == EAI_AGAIN  || n == EAI_FAIL )
		{
		}
		return -1;
	}
	ressave = res;

	do {
		sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (sockfd < 0)
			continue;       /* ignore this one */

		if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0)
			break;          /* success */

		close(sockfd);  /* ignore this one */
		sockfd = -1;
	} while ( (res = res->ai_next) != NULL);

	if (sockfd < 0 )        /* errno set from final connect() */
	{
		Debug(DBG_DEBUG,"tcp_connect error for %s, %s", host, serv);
	}

	freeaddrinfo(ressave);

	return(sockfd);
}
