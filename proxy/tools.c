/*! 
 * \copyright Copyright(c)200x, All right reserved
 * \brief 
 * \author Liu jiaxiang
 * \version  0.0.1
 * \date   2010年01月15日
 */

#include "tools.h"
#include <errno.h>
#include <unistd.h>
#include <signal.h>


size_t writen(int fd, const void *vptr, size_t n)
{
	size_t      nleft;
	size_t     nwritten;
	const char  *ptr;
	struct timeval tv;
	
	tv.tv_sec = 0;
	tv.tv_usec = 100000;

	ptr = vptr;
	nleft = n;
	while (nleft > 0)
	{
		if ( (nwritten = write(fd, ptr, nleft)) <= 0)
		{
			if (nwritten < 0 && errno == EINTR)
				nwritten = 0;       /* and call write() again */
			else if(errno == SIGPIPE)
			{
				fprintf(stderr, "errno = %d\n", errno);
				sleep(1);
			}
			else
				return(-1);         /* error */
		}
		//select(0, NULL, NULL, NULL, &tv);
		//usleep(1000);
		nleft -= nwritten;
		ptr   += nwritten;
	}
	return(n);
}
