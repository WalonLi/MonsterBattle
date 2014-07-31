#include "header.h"
#include "connect.h"
#include "image.h"
#include "audio.h"
// the menu  start at this
void Set_Menu_Window() ;
void Set_Menu_Container() ;

void Battle_Request() ;

void Set_Signal_Connect() ;

void Set_All_Thread() ;
gpointer Set_Socket_Server() ;
void Set_Pop_Window( int fd ) ;
void Set_Send_Battle_Socket() ;

void Click_YES_Event( GtkWidget *window, gpointer _data ) ;
void Click_NO_Event( GtkWidget *window, gpointer _data ) ;
void Convert_Page( void ) ; 

void Set_Audio() ;

typedef struct _Option {
    GtkWidget * window ;
    int fd ;
} Option ;


int g_Error_Message = 0 ;
int g_menu_fd = 0 ;
int menu_audio_mark = 0 ;
GtkWidget *main_Window ;

GtkWidget *menu_Container ;

GtkWidget *z_Monster_Tag ;
GtkWidget *z_Monster_Page ;
GtkWidget *z_Information_Tag ;
GtkWidget *z_Information_Page ;
GtkWidget *z_Battle_Log_Tag ;
GtkWidget *z_Battle_Log_Page ;
GtkWidget *z_Battle_Tag ;
GtkWidget *z_Battle_Page ;
GtkWidget *z_Battle_IP_Entry ;
GtkWidget *z_Battle_Button ;

pthread_t t_Socket_Thread ;
pthread_t t_Send_Battle_Thread, t_Audio_Thread ;

void Menu_Main( void ) {
    menu_audio_mark = 0 ;
    Set_All_Thread() ;
    Set_Menu_Window() ;
    Set_Signal_Connect() ;
    gtk_main() ;
    if ( g_menu_fd != 0 ) Battle_Main(g_menu_fd) ;
}

void Set_Menu_Window() {
    // set window to top level
    main_Window = gtk_window_new( GTK_WINDOW_TOPLEVEL ) ;
    // set window size
    gtk_window_set_default_size( GTK_WINDOW( main_Window ), 500, 300 ) ;
    // set window title
    gtk_window_set_title (GTK_WINDOW (main_Window), "Monster Battle");
    gtk_window_set_position( GTK_WINDOW(main_Window), GTK_WIN_POS_CENTER) ;
    // gtk_widget_show( main_Window ) ;
    // set menu tag and container 
    Set_Menu_Container() ;

    gtk_widget_show_all( main_Window ) ;

} // Set_Menu_Window

void Set_Menu_Container() {
    GdkBitmap *mask ; 
    GdkPixmap *pixmap;
    GtkStyle *style ;
    FILE *fp, *detail ;
    char data[400] ;
    char temp[100] ;
    char detail_filename[100] ;
    int x = 0 ;

    memset( data, '\0', 400 ) ;
    memset( temp, '\0', 100 ) ;
    memset( detail_filename, '\0', 100 ) ;


    menu_Container = gtk_notebook_new() ;
    gtk_notebook_set_tab_pos( GTK_NOTEBOOK( menu_Container ), GTK_POS_LEFT ) ;
     
    z_Monster_Tag = gtk_label_new( "Monster" ) ;
    gtk_widget_set_usize( z_Monster_Tag, 85, 75 ) ;
    z_Information_Tag = gtk_label_new( "Monster Detail" ) ;
    gtk_widget_set_usize( z_Information_Tag, 85, 75 ) ;
    z_Battle_Log_Tag = gtk_label_new( "Battle Log" ) ;
    gtk_widget_set_usize( z_Battle_Log_Tag, 85, 75 ) ;
    z_Battle_Tag = gtk_label_new( "Battle!!" ) ;
    gtk_widget_set_usize( z_Battle_Tag, 85, 75 ) ;

 
    
    style = gtk_widget_get_style(main_Window ) ;
    

    // for dektop linux
    // z_Monster_Page = gtk_image_new_from_file( "DataBase/resource/123.jpg" ) ;
     
    fp = fopen( gAccount, "r" ) ;   
    //*********************************
    fscanf( fp, "%s", temp ) ;
    strcat( data, temp ) ;

    memset( temp, '\0', 100 ) ;
    fscanf( fp, "%s", temp ) ;

    
    strcpy( detail_filename, "DataBase/monster_detail/" ) ;
    if ( temp[0] == '1' ) {
       strcat( detail_filename, "a_monster" ) ;
       pixmap = gdk_pixmap_create_from_xpm_d( main_Window->window,
                     &mask,&style->bg[GTK_STATE_NORMAL], (gchar **)a_XPM);
    } // else if
    else if ( temp[0] == '2' ) {
       strcat( detail_filename, "b_monster" ) ;
       pixmap = gdk_pixmap_create_from_xpm_d( main_Window->window,
                     &mask,&style->bg[GTK_STATE_NORMAL], (gchar **)b_XPM);
    } // else if
    else if ( temp[0] == '3' ) {
       strcat( detail_filename, "c_monster" ) ;
       pixmap = gdk_pixmap_create_from_xpm_d( main_Window->window,
                     &mask,&style->bg[GTK_STATE_NORMAL], (gchar **)c_XPM);
    } // else if

    z_Monster_Page = gtk_pixmap_new(pixmap,mask);
    gProperty[0] = temp[0] ;
  
    strcat( data, "\n\n" ) ;   
    detail = fopen( detail_filename, "r" ) ; 
    for ( x = strlen(data) ; !feof(detail); x++ ) fscanf( detail, "%c", &data[x] ) ;
   
    z_Information_Page = gtk_label_new( data ) ;
    //**********************************

    memset( data, '\0', 400 ) ;
    strcat( data, "Battle History\nRecord:   " ) ;

    memset( temp, '\0', 100 ) ;
    fscanf( fp, "%s", temp ) ;
    strcat( data, temp ) ;
    strcat( data, "   " ) ;
    memset( temp, '\0', 100 ) ;
    fscanf( fp, "%s", temp ) ;
    strcat( data, temp ) ;    
    strcat( data, "\n\nMonster\t\tResult\n" ) ;
    for ( x = strlen(data) ; !feof(fp) ; x++ ) fscanf( fp, "%c", &data[x] ) ; 
    z_Battle_Log_Page = gtk_label_new( data ) ;
    // **********************************
    
    z_Battle_Page = gtk_fixed_new() ;

    fclose(fp) ;
    fclose(detail) ;

    z_Battle_IP_Entry = gtk_entry_new() ;
    gtk_fixed_put( GTK_FIXED(z_Battle_Page), z_Battle_IP_Entry, 100, 150 ) ;
    gtk_widget_set_usize( z_Battle_IP_Entry, 170,50 ) ;
    gtk_entry_set_text( z_Battle_IP_Entry , "192.168.0.50" ) ;
    z_Battle_Button = gtk_button_new_with_label( "Send!!" ) ;
    gtk_widget_set_usize( z_Battle_Button, 70,50 ) ;
    gtk_fixed_put( GTK_FIXED(z_Battle_Page), z_Battle_Button, 270, 150 ) ;    

    gtk_notebook_append_page( GTK_NOTEBOOK( menu_Container ), 
                              z_Monster_Page, z_Monster_Tag ) ;
    gtk_notebook_append_page( GTK_NOTEBOOK( menu_Container ), 
                              z_Information_Page, z_Information_Tag ) ; 
    gtk_notebook_append_page( GTK_NOTEBOOK( menu_Container ), 
                              z_Battle_Log_Page, z_Battle_Log_Tag ) ;
    gtk_notebook_append_page( GTK_NOTEBOOK( menu_Container ),
                              z_Battle_Page, z_Battle_Tag ) ;

    gtk_container_add( GTK_WINDOW( main_Window ), menu_Container ) ;

} // Set_Menu_Container() 

// =================================================================
// =================================================================
// =================================================================
void Set_Signal_Connect() {
    gtk_signal_connect(main_Window, "destroy", GTK_SIGNAL_FUNC(gtk_main_quit), NULL);
    gtk_signal_connect(GTK_OBJECT(z_Battle_Button), "clicked", 
                     GTK_SIGNAL_FUNC(Battle_Request), NULL) ;
    // g_signal_connect_after(menu_Container, "switch-page", 
    //                   G_CALLBACK( Convert_Page ), NULL ) ;

} // Set_Signal_Connect()
// =================================================================
// =================================================================
// =================================================================
void Battle_Request(){
    g_Error_Message = pthread_create( &t_Send_Battle_Thread, NULL,
                                (void*)Set_Send_Battle_Socket, NULL ) ;
    // Set_Send_Battle_Socket() ;  
    // Set_Pop_Window( 5 ) ;
    g_print( "Send battle request\n" ) ;
} // Battle_Request()


// ================================================================
//=================================================================
// =================================================================
void Set_Send_Battle_Socket() {
    // connect to the another user to battle 
    // send_fd is Sender FD 
    int send_fd ;
    char *ip_addr ;
    char buffer[100] ;
    send_fd = socket( AF_INET, SOCK_STREAM, 0 ) ;
    if ( send_fd  == -1 ) perror( "socket" ) ;

    c_Connect_Addr.sin_family = AF_INET;
    c_Connect_Addr.sin_port = htons( SEND_PORT ) ;
    ip_addr = gtk_entry_get_text( z_Battle_IP_Entry ) ;
    c_Connect_Addr.sin_addr.s_addr = inet_addr( ip_addr ) ;
    connect( send_fd, (struct sockaddr*)&c_Connect_Addr, sizeof( c_Connect_Addr) ) ;
    send( send_fd, "BATTLE REQUEST", 14, 0 ) ;
 
    bzero( buffer, 100 ) ;

    recv( send_fd, buffer, sizeof(buffer),0) ;

    if ( strcmp( buffer, "YES" ) == 0 ) {
       g_print( "Starting the Battle Activity.\n" ) ;
       g_menu_fd = send_fd ;
       menu_audio_mark = 1 ;
       gtk_widget_destroy( main_Window ) ;
       // Battle_Main( send_fd ) ;
    } // if
    else if ( strcmp( buffer, "NO" ) == 0 ) {
	close( send_fd ) ;
    } // else if  

} // Set_Send_Battle_Socket
// ================================================================
// =================================================================
// =================================================================
gpointer Set_Socket_Server() {
    int error = 0 ;
    int address = 0 ;
    int length = 0 ;
    char data[100] ;


 
    while( 1 ) {
        int client_fd ;
        client_fd = accept( sock_fd, 
                            ( struct sockaddr*)&s_Remote_Addr, &address) ;
        if ( client_fd == -1 ) perror( "socket" ) ;
         
        bzero( data, 100 ) ;
        length = recv( client_fd, data, 100, 0 ) ;
        g_print( data, "\n" ) ;
        if ( length > 0 ) {
            if ( strcmp( data, "BATTLE REQUEST") == 0 ) {
                // this function will console yes and no reponse
                Set_Pop_Window( client_fd ) ;
		// send( client_fd, "YES", 3, 0 ) ;  
		// g_menu_fd = client_fd ;
    		// gtk_widget_destroy( main_Window ) ;
                if ( error == -1 ) perror( "Socket Send ERROR\n" ) ;
            } // if
        } //if

    } //while()

   //  close( sock_fd ) ;
} // Set_Socket_Server()
// =================================================================
// =================================================================
// =================================================================
void Set_Pop_Window( int fd ) {
    GtkWidget *popup_window ;
    GtkWidget *vbox,  *hbox ;
    GtkWidget *text_label ;
    GtkWidget *button_yes ;
    GtkWidget *button_no ;
    Option *data ;
    popup_window = gtk_window_new( GTK_WINDOW_TOPLEVEL ) ;
    // gdk_window_set_decorations( popup_window->window, (GdkWMDecoration)0 ) ;
    gtk_container_set_border_width( GTK_CONTAINER( popup_window ), 10 ) ;
    gtk_window_set_position( GTK_WINDOW(popup_window), GTK_WIN_POS_CENTER) ;
    gtk_window_set_default_size( GTK_WINDOW(popup_window), 250, 50 ) ;
    vbox = gtk_vbox_new( TRUE, 5 ) ;
    hbox = gtk_hbox_new( TRUE, 5 ) ;
    text_label = gtk_label_new( "Are You Want to Battle?" ) ;
    button_yes = gtk_button_new_with_label( "Yes" ) ;
    button_no = gtk_button_new_with_label( "No" ) ;

    gtk_container_add( GTK_CONTAINER( vbox ), text_label );
    gtk_container_add( GTK_CONTAINER( vbox ), hbox );
    gtk_container_add( GTK_CONTAINER( hbox ), button_yes );
    gtk_container_add( GTK_CONTAINER( hbox ), button_no );
    gtk_container_add( GTK_WINDOW( popup_window ), vbox );
   
    data = g_new0( Option*, 1 ) ;
    data->window = popup_window ;
    data->fd = fd ;
   
    gtk_signal_connect(GTK_OBJECT(button_yes), "clicked", GTK_SIGNAL_FUNC
                             (Click_YES_Event), data) ;
    gtk_signal_connect(GTK_OBJECT(button_no), "clicked", GTK_SIGNAL_FUNC 
                             (Click_NO_Event), data) ;
   
    // gdk_threads_enter ();
    gtk_widget_show_all( popup_window ) ;
    // gdk_threads_leave ();
} // Set_Pop_Window()  
// =================================================================
// =================================================================
// =================================================================
void Click_YES_Event( GtkWidget * widget, gpointer _data ) {
    Option *data = _data ;
    send( data->fd, "YES", 3, 0 ) ;
    gtk_widget_destroy( data->window ) ;
    gtk_widget_destroy( main_Window ) ;
    g_print( "Starting the Battle Activity.\n" ) ;
    menu_audio_mark = 1 ;
    g_menu_fd = data->fd ;
    // Battle_Main( data->fd ) ;
} 
void Click_NO_Event( GtkWidget * widget, gpointer _data ) {
    Option *data = _data ;
    send( data->fd, "NO", 2, 0 ) ;
    close( data->fd ) ;
    gtk_widget_destroy( data->window ) ;
}
// =================================================================
// =================================================================
// =================================================================
void Set_All_Thread() {
    // if ( !g_thread_supported() ) g_thread_init(NULL) ;
    pthread_create( &t_Socket_Thread , NULL , (void *)Set_Socket_Server, NULL ) ;
    pthread_create( &t_Audio_Thread , NULL , (void *)Set_Audio, NULL ) ;
} // Set_All_Thread() 

// =================================================================
// =================================================================
void Set_Audio() {
    while ( 1 ) {
        int fp_src;
        int fp_dst;
        unsigned char buf[BUFFER_SIZE];
        int arg;        // argument for ioctl calls
        int status;  // return status of system calls


        // fp_src = open( "DataBase/audio/menu_background.wav", O_RDWR );
        fp_src = open( "DataBase/audio/battle_background.wav", O_RDWR );
        fp_dst = open( "/dev/dsp", O_RDWR );
        printf( "-------->Play Background Audio Start\n" );

        while( menu_audio_mark != 1  && read( fp_src, buf, BUFFER_SIZE ) >= BUFFER_SIZE )         {
                write( fp_dst, buf, BUFFER_SIZE );
        }

        close( fp_src );
        close( fp_dst );
        if ( menu_audio_mark == 1 ) return ;
   } // while
} // Set_Audio
// =================================================================
void Convert_Page( void ) {
     // gtk_notebook_set_current_page( GTK_NOTEBOOK( menu_Container ), 3 ) ;
     
     // gtk_notebook_set_tab_label( GTK_NOTEBOOK( menu_Container ), 
     //                                z_Battle_Page, z_Battle_Tag ) ;
} // Convert_Page
