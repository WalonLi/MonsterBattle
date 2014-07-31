#include "header.h"

void Send_Clicked() ;


// lazy..
GtkWidget *text_Input ;
GtkWidget *loggin_Window ;

void Loggin_Main() {
    // GtkWidget *loggin_Window ;
    GtkWidget *text_Label ;
    GtkWidget *vbox ;
    GtkWidget *send_Button ;
    // GtkWidget *text_Input ;
    loggin_Window = gtk_window_new( GTK_WINDOW_TOPLEVEL ) ;
    gtk_window_set_default_size( GTK_WINDOW( loggin_Window ), 250, 180 ) ;
    gtk_window_set_title (GTK_WINDOW (loggin_Window), "Monster Battle");
    gtk_window_set_position( GTK_WINDOW(loggin_Window), GTK_WIN_POS_CENTER) ; 

    vbox = gtk_vbox_new( FALSE, 5 ) ;
    text_Label = gtk_label_new( "Please input your account name" ) ;
    send_Button = gtk_button_new_with_label( "Send" ) ;
    gtk_widget_set_usize( send_Button, 150,50 ) ;
    text_Input = gtk_entry_new() ;
    gtk_entry_set_text( text_Input, "admin" ) ;
    gtk_widget_set_usize( text_Input, 250, 40 ) ;

    gtk_container_add( GTK_CONTAINER( vbox ), text_Label );
    gtk_container_add( GTK_CONTAINER( vbox ), text_Input );
    gtk_container_add( GTK_CONTAINER( vbox ), send_Button );
    gtk_container_add( GTK_WINDOW( loggin_Window ), vbox );

    gtk_signal_connect( GTK_WINDOW( loggin_Window), "destroy", 
                                  GTK_SIGNAL_FUNC(gtk_main_quit), NULL ) ;
    gtk_signal_connect( GTK_OBJECT(send_Button), "clicked", 
                                  GTK_SIGNAL_FUNC(Send_Clicked), NULL ) ;
    gtk_widget_show_all( loggin_Window ) ;
    gtk_main() ;
} 

void Send_Clicked() {
    FILE *fp ;
    char *text ;
    memset( gAccount, '\0', sizeof(gAccount) ) ;
    strcat( gAccount,"DataBase/account_data/") ;
    text = gtk_entry_get_text( text_Input ) ;
    // strcpy( text, "77445" ) ;
    if ( strlen(text) < 1 ) return ;
    strcat( gAccount, text ) ;
    g_print( gAccount ) ;
    g_print( "\n" ) ; 
    fp = fopen( gAccount, "r" ) ;
    gtk_widget_destroy( loggin_Window ) ;
    if ( fp != NULL ) {
        g_print("Starting the Menu Actvity.\n") ;
        fclose( fp ) ;
        // gtk_main_quit() ;
        Menu_Main() ;
    } // if
    else {
        g_print( "Starting the Choose Activity.\n" ) ;
        Choose_Main() ;
        // gtk_main_quit() ;
    } // else
    
} // Send_Clicked


