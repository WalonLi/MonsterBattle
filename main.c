#include "header.h"
#include "connect.h"
int main( int argc, char *argv[]) {
    int error = 0, opt = 1 ;
    gtk_init( &argc, &argv ) ;
    g_thread_init(NULL) ;
 

    // set socket_server ;
    sock_fd = socket( AF_INET,SOCK_STREAM, 0 ) ;
    if ( sock_fd  == -1 ) perror( "socket" ) ;


    bzero( &s_Myself_Addr, sizeof(s_Myself_Addr) ) ;
    s_Myself_Addr.sin_family = AF_INET ;
    s_Myself_Addr.sin_port = htons( SERVER_PORT ) ;
    s_Myself_Addr.sin_addr.s_addr = INADDR_ANY ;
    // setsockopt to solve Address already in use
    // when restart the program at short time
    setsockopt( sock_fd,SOL_SOCKET,SO_REUSEADDR, &opt, sizeof(opt) ) ;
    error = bind( sock_fd, ( struct sockaddr*)&s_Myself_Addr,
              sizeof(s_Myself_Addr)) ;
    if ( error == -1 ) perror( "socket" ) ;

    error = listen( sock_fd, 10 ) ;
    if ( error == -1 ) perror( "socket" ) ;



    g_print( "Starting the Loggin Activity.\n" ) ; 
    Loggin_Main() ;
    // Menu_Main() ;
}
