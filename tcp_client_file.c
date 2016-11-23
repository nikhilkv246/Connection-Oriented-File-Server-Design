#include <stdio.h>
#define _USE_BSD 1
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>

#define BUFSIZE 64

#ifndef INADDR_NONE
#define INADDR_NONE 0xffffffff
#endif

#define MAX_SEND_BUF 1000
#define MAX_RECV_BUF 1000




extern int errno;
int errexit(const char *format,...);
int connectTCP(const char *service,const char *host,int portnum);
int connectsock(const char *service,const char *host,int portnum,const char *transport);

/*------------------------------------------------------------------------------------
 * connectsock-Allocate and connect socket for TCP
 *------------------------------------------------------------------------------------
*/

int connectsock(const char *service,const char *host,int portnum,const char *transport)
{

/*
Arguments:
*service   - service associated with desired port
*host      - name of the host to which connection is desired
*transport - name of the transport protocol to use

*/ 

struct sockaddr_in sin; 					 //an internet endpoint address
int s,type;               					 //socket descriptor and socket type 

memset(&sin,0,sizeof(sin));  
sin.sin_family=AF_INET;   				         //family name
  
sin.sin_port=htons(portnum);                                        //port number


inet_pton(AF_INET,host,&(sin.sin_addr));                         //to convert host name into 32-bit IP address
 
/*
 * to determine the type of socket
 */

if(strcmp(transport,"udp")==0)         
type=SOCK_DGRAM;
else
type=SOCK_STREAM;

/* Allocate a socket */

s=socket(AF_INET,type,0);


if(s<0)
errexit("can't create socket : %s\n",strerror(errno));


if((connect(s,(struct sockaddr *) &sin,sizeof(sin)))<0)        //connect the socket 
errexit("can't connect to %s.%s: %s\n",host,service,strerror(errno));


return s;

}

/*
 * errexit- print and error message and exit
 */


int errexit(const char* format,...)
{
va_list args;

va_start(args,format);
vfprintf(stderr,format,args);
va_end(args);
exit(1);
}


/*------------------------------------------------------------------------
 * connectTCP-connect to a specified TCP service on specified host
 -------------------------------------------------------------------------*/

int connectTCP(const char *service,const char *host,int portnum)
{
/*
 Arguments:
 *service-service associated with desired port
 *host-name of the host to which connection is desired
 */

return connectsock(service,host,portnum,"tcp");
}


/*
 main - Client file
 */ 

int main(int argc,char *argv[])
{

char *host;

char *file_name;
ssize_t recv_bytes;
char recv_buff[1000];
int portnum;


char *service="time";                                               //default service port

int s,n;                                                            //socket descriptor

switch(argc)
{
case 1:
host="localhost";

break;

case 2:
host="localhost";

break;

case 4:
file_name=argv[3];
portnum=atoi(argv[2]);
host=argv[1];
break;

default:printf("Error in taking arguments\n");
exit(1);

}

                                             

s=connectTCP(service,host,portnum);                                        


send(s,file_name,MAX_SEND_BUF,0);                                  
int ptr;
int flag = 0;




		while((recv_bytes=recv(s,recv_buff,MAX_RECV_BUF,0))>0)
		{
			if((ptr=open(file_name,O_WRONLY|O_CREAT,0644))<0)
			{
			 printf("error in writing to file\n");
			}

			printf("%s\n",recv_buff);
			write(ptr,recv_buff,recv_bytes);  
			flag = 1;                      
		}

		if(flag == 0)
	        printf("File Not Found\n");


close(s);





exit(0);

}





















