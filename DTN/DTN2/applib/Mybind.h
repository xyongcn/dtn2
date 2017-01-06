//add by gaorui
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <netinet/in.h>
 #include <arpa/inet.h>
#include<errno.h>

class Mybind
{
public:
	static int mybind(int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len)
	{
		int t=bind(__fd,__addr,__len);
		if(t==-1)
		{
			printf("errno=%d\n",errno);
			char *message=strerror(errno);
			printf("Mesg:%s\n",message);
		}
		return t;
	}

	static int mysendto(int __fd, __const void *__buf, size_t __n,
		       int __flags, __CONST_SOCKADDR_ARG __addr,
		       socklen_t __addr_len)
	{
		return sendto(__fd,__buf,__n,__flags,__addr,__addr_len);
	}

	static int myrecvfrom(int __fd, void *__restrict __buf, size_t __n,
			 int __flags, __SOCKADDR_ARG __addr,
			 socklen_t *__restrict __addr_len)
	{
		return recvfrom(__fd,__buf,__n,__flags,__addr,__addr_len);
	}
	static int myclose(int __fd)
	{
		return close(__fd);
	}

};
//end by gaorui
