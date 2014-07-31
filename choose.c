#include "header.h"
#include "image.h" 
#include "audio.h"

void Send_Data() ;
void Get_Data( int type, char data[500] ) ;
void Play_Audio_Thread() ;


int type = 1 ;

pthread_t t_Audio_Thread ;
void choose_a(){ type = 1 ; }
void choose_b(){ type = 2 ; }
void choose_c(){ type = 3 ; }


GtkWidget *choose_Window ;


void Choose_Main() {
    GtkWidget * send_Button ;
    GtkWidget * h_vbox ;
    GtkWidget * m_hbox ;   
    GtkWidget * l_a_vbox, *l_b_vbox, *l_c_vbox ;
    GtkWidget *a_button, *b_button, *c_button ;
    GtkWidget *a_image, *b_image, *c_image ;
    GdkBitmap *a_mask, *b_mask, *c_mask ;
    GdkPixmap *a_pixmap, *b_pixmap, *c_pixmap ;
    GtkStyle *style ;
    GtkWidget *fixed_button ;   
    char data[500] ; 

    choose_Window = gtk_window_new( GTK_WINDOW_TOPLEVEL ) ;
    gtk_window_set_default_size( GTK_WINDOW(choose_Window), 600,400 ) ;
    gtk_window_set_position( GTK_WINDOW(choose_Window), GTK_WIN_POS_CENTER ) ;
    gtk_window_set_title (GTK_WINDOW (choose_Window), "Monster Battle");
   
    h_vbox = gtk_vbox_new( FALSE, 0 ) ;    
    gtk_container_add( GTK_CONTAINER(choose_Window), h_vbox ) ;    
    m_hbox = gtk_hbox_new( TRUE, 0 ) ;
    gtk_box_pack_start( GTK_BOX(h_vbox), m_hbox, TRUE, TRUE, 0 ) ;

    l_a_vbox = gtk_vbox_new( FALSE, 0 ) ;
    gtk_box_pack_start( GTK_BOX(m_hbox), l_a_vbox, TRUE, TRUE, 0 ) ;
    l_b_vbox = gtk_vbox_new( FALSE, 0 ) ;
    gtk_box_pack_start( GTK_BOX(m_hbox), l_b_vbox, TRUE, TRUE, 0 ) ;
    l_c_vbox = gtk_vbox_new( FALSE, 0 ) ;
    gtk_box_pack_start( GTK_BOX(m_hbox), l_c_vbox, TRUE, TRUE, 0 ) ;    

    // Radio Button setting    

    
    style = gtk_widget_get_style(choose_Window) ;
    
    a_pixmap = gdk_pixmap_create_from_xpm_d( choose_Window->window,
                     &a_mask,&style->bg[GTK_STATE_NORMAL],
                     (gchar **)a_XPM);
    a_image = gtk_pixmap_new(a_pixmap,a_mask);
    

    // a_image = gtk_image_new_from_file( "DataBase/resource/manwalk.gif" ) ;
    
    Get_Data( 1, data ) ;
    a_button = gtk_radio_button_new_with_label(NULL, data) ;
    gtk_container_add(GTK_BOX(l_a_vbox), a_image ) ;
    gtk_container_add( GTK_BOX(l_a_vbox), a_button ) ; 

    
    b_pixmap = gdk_pixmap_create_from_xpm_d( choose_Window->window,
                     &b_mask,&style->bg[GTK_STATE_NORMAL],
                     (gchar **)b_XPM);
    b_image = gtk_pixmap_new(b_pixmap,b_mask);
    

    // b_image = gtk_image_new_from_file( "DataBase/resource/manwalk.gif" ) ;
    
    Get_Data( 2, data ) ;
    b_button = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(a_button), data ) ;
    gtk_container_add(GTK_BOX(l_b_vbox), b_image ) ;
    gtk_container_add( GTK_BOX(l_b_vbox), b_button ) ;

    
    c_pixmap = gdk_pixmap_create_from_xpm_d( choose_Window->window,
                     &c_mask,&style->bg[GTK_STATE_NORMAL],
                     (gchar **)c_XPM);
    c_image = gtk_pixmap_new(c_pixmap,c_mask);
    

    // c_image = gtk_image_new_from_file( "DataBase/resource/manwalk.gif" ) ;
    
    Get_Data( 3, data ) ;
    c_button = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(b_button), data ) ;
    gtk_container_add(GTK_BOX(l_c_vbox), c_image ) ;
    gtk_container_add( GTK_BOX(l_c_vbox), c_button ) ;
 
    // gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON(a_button), FALSE) ;
    

    fixed_button = gtk_fixed_new() ;
    send_Button = gtk_button_new_with_label( "Send" ) ;    
    gtk_fixed_put( GTK_FIXED(fixed_button), send_Button, 175, 0 ) ;
    gtk_widget_set_usize( send_Button, 250, 40 ) ;
    gtk_box_pack_start( GTK_BOX(h_vbox), fixed_button, FALSE,FALSE,0 ) ;

    // =============Set Siganl =========   
    gtk_signal_connect( GTK_WINDOW(choose_Window), "destroy",
                                  GTK_SIGNAL_FUNC(gtk_main_quit), NULL ) ;

    gtk_signal_connect( GTK_OBJECT(a_button), "clicked",
                                  GTK_SIGNAL_FUNC(choose_a), NULL ) ;
    gtk_signal_connect( GTK_OBJECT(b_button), "clicked",
                                  GTK_SIGNAL_FUNC(choose_b), NULL ) ;
    gtk_signal_connect( GTK_OBJECT(c_button), "clicked",
                                  GTK_SIGNAL_FUNC(choose_c), NULL ) ;       

    gtk_signal_connect( GTK_OBJECT(send_Button), "clicked", 
                                  GTK_SIGNAL_FUNC(Send_Data), NULL ) ;
    // ==================================

    gtk_widget_show_all( choose_Window ) ;
    gtk_main() ;
    
} // Choose_Main()



void Send_Data() {    
    FILE *fp ;
    char temp[15] ;
    int x = 0 ;
    int length = strlen( gAccount ) ;
    memset( temp, '\0', 15 ) ;

    g_print( "%s%d\n", "Choose Monster ",type ) ;
    fp = fopen( gAccount, "w" ) ;
    for ( ; (x+22) < length ; x++) temp[x] = gAccount[x+22] ;
    fprintf( fp, "%s\n", temp ) ;
    fprintf( fp, "%d\n", type ) ;
    fprintf( fp, "%s\n", "WINS:0 LOSE:0" ) ;
    fclose(fp) ;
    gtk_widget_destroy( choose_Window ) ;
    g_print( "Starting the Menu activity.\n" ) ;
    Menu_Main() ;
} // Send_Data() 


void Get_Data( int type, char data[500] ) {
    FILE *fp  ;
    int x = 0 ;
    memset( data, '\0', sizeof(data) ) ;
    if ( type == 1 ) fp = fopen( "DataBase/monster_detail/a_monster", "r" ) ;
    else if ( type == 2 ) fp = fopen( "DataBase/monster_detail/b_monster", "r" ) ;
    else if ( type == 3 ) fp = fopen( "DataBase/monster_detail/c_monster", "r" ) ;
    for ( ; !feof(fp) ; x++ ) data[x] = getc(fp) ; 
   
    // data have some garbage??...
    data[x-2] = '\0' ;

    fclose(fp) ;  
    // g_print( "%s\n", data ) ;
} // Get_Data

/*
void Play_Audio_Thread() {
        int fp_src;
        int fp_dst;
        unsigned char buf[BUFFER_SIZE];
        int arg;        // argument for ioctl calls
        int status;  // return status of system calls


        fp_src = open( "DataBase/audio/audio.raw", O_RDWR );
        fp_dst = open( "/dev/dsp", O_RDWR );
        printf( "-------->Play Audio Start\n" );

        while( read( fp_src, buf, BUFFER_SIZE ) >= BUFFER_SIZE )
        {
                write( fp_dst, buf, BUFFER_SIZE );
        }

        close( fp_src );
        close( fp_dst );
} // Play_Audio_Thread

*/
