#ifndef _SYS_SOCKET_H_
#define _SYS_SOCKET_H_
#include <sys/socket.h>
#endif

#ifndef _NETINET_IN_H_
#define _NETINET_IN_H_
#include <netinet/in.h>
#endif

#define SERVER_PORT 9999
#define SEND_PORT 9999


// s_* means server side component
struct sockaddr_in s_Myself_Addr ;
struct sockaddr_in s_Remote_Addr ;

// c_* means client side component
struct hostent *c_Host ;
struct sockaddr_in c_Connect_Addr ;

int sock_fd ;
