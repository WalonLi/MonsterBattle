#include "header.h"
#include "connect.h"
#include "image.h"
#include "audio.h" 

// ====  for keypad ====
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
// =====================

#define LED_SHOW 10

void Solve_Bonus() ;
void KeyPad_Method() ;
void Listen_Method() ;
void Show_From_Enymy_Attack() ;
void Show_Attack_Enymy() ; 
void Write_To_Database() ;
void Audio_Background_Method() ;
void Close_Message_Method() ;
void Check_Add_WIN_LOSE( char temp[50] ,char digit[3], int mark_win ) ;

int gBonus = 0 ;
int g_fd = 0 ;
int keypad_fd = 0 ;
int gFire_Count = 0 ;
int delay_mark = 0 ;
int battle_audio_mark = 0 ;
char enemy_Name[100] ;


pthread_t t_KeyPad_Thread, t_Listen_Thread, t_Audio_Background_Thread ,t_CloseMessage_Thread ;
GtkWidget *battle_Window ;
GtkWidget *vbox, *hbox ;
GtkWidget *progress_bar ;
GtkWidget *fire_image, *attacked_image, *monster_image ;
GtkWidget *fixed ;
GdkBitmap *fire_mask, *attacked_mask, *monster_mask ;
GdkPixmap *fire_pixmap, *attacked_pixmap, *monster_pixmap ;
GtkStyle *style ;
GtkWidget *message_dialog ;
GtkWidget *label ;

void Battle_Main( int fd ) {
   gBonus = 0 ;
   g_fd = 0 ;
   keypad_fd = 0 ;
   gFire_Count = 0 ;
   battle_audio_mark = 0 ;
   delay_mark = 0 ;
   memset( enemy_Name, '\0', 100 ) ;
   
   battle_Window = gtk_window_new( GTK_WINDOW_TOPLEVEL ) ;
   gtk_window_set_default_size( GTK_WINDOW( battle_Window ), 300, 200 ) ;
   gtk_window_set_title (GTK_WINDOW (battle_Window), "Monster Battle");
   gtk_window_set_position( GTK_WINDOW(battle_Window), GTK_WIN_POS_CENTER) ;
   
   vbox = gtk_vbox_new( FALSE, 0 ) ;
   gtk_container_add( GTK_CONTAINER(battle_Window), vbox ) ;   
   progress_bar = gtk_progress_bar_new() ;
   gtk_box_pack_start( GTK_BOX( vbox), progress_bar, TRUE, TRUE, 0 ) ;
   hbox = gtk_hbox_new( TRUE, 0 ) ;
   gtk_box_pack_start( GTK_BOX( vbox ) , hbox, TRUE, TRUE, 0 ) ;

   // ====================
   style = gtk_widget_get_style(battle_Window) ;

   fire_pixmap = gdk_pixmap_create_from_xpm_d( battle_Window->window,
                     &fire_mask,&style->bg[GTK_STATE_NORMAL],
                     (gchar **)fire_XPM);
   fire_image = gtk_pixmap_new(fire_pixmap,fire_mask);

   attacked_pixmap = gdk_pixmap_create_from_xpm_d( battle_Window->window,
                     &attacked_mask,&style->bg[GTK_STATE_NORMAL],
                     (gchar **)attacked_XPM);
   attacked_image = gtk_pixmap_new(attacked_pixmap,attacked_mask);
  
   fixed = gtk_fixed_new() ;
   gtk_widget_set_usize( fixed, 150, 100 ) ;
   gtk_fixed_put( GTK_FIXED( fixed), fire_image, 150, 100 ) ;
   gtk_fixed_put( GTK_FIXED( fixed), attacked_image, 150, 100 ) ;

   gtk_container_add(GTK_BOX(hbox), fixed ) ;

   if ( gProperty[0] == '1' ) {
      monster_pixmap = gdk_pixmap_create_from_xpm_d( battle_Window->window,
                     &monster_mask,&style->bg[GTK_STATE_NORMAL], (gchar **)a_XPM);
   } // else if
   else if ( gProperty[0] == '2' ) {
      monster_pixmap = gdk_pixmap_create_from_xpm_d( battle_Window->window,
                    &monster_mask,&style->bg[GTK_STATE_NORMAL], (gchar **)b_XPM);
   } // else if
   else if ( gProperty[0] == '3' ) {
      monster_pixmap = gdk_pixmap_create_from_xpm_d( battle_Window->window,
                    &monster_mask,&style->bg[GTK_STATE_NORMAL], (gchar **)c_XPM);
   } // else if

   monster_image = gtk_pixmap_new(monster_pixmap,monster_mask);
   gtk_container_add(GTK_BOX(hbox), monster_image ) ;
   // ============================
   g_fd = fd ;
   Solve_Bonus() ;

   pthread_create( &t_KeyPad_Thread, NULL, (void*)KeyPad_Method, NULL ) ;
   pthread_create( &t_Listen_Thread, NULL, (void*)Listen_Method, NULL ) ;
   pthread_create( &t_Audio_Background_Thread, NULL, (void*)Audio_Background_Method, NULL ) ;
   // ============================
   gtk_signal_connect( GTK_WINDOW(battle_Window), "destroy",
                                  GTK_SIGNAL_FUNC(gtk_main_quit), NULL ) ;
   
   gtk_widget_show_all( battle_Window ) ;
   
   // g_print( "%d", fd ) ;
   gtk_main() ;
   close(g_fd) ;
   // close(sock_fd) ;
   // show win or false message ;
   message_dialog = gtk_window_new( GTK_WINDOW_TOPLEVEL ) ;
   gtk_window_set_default_size( GTK_WINDOW( message_dialog ), 200, 100 ) ;
   gtk_window_set_title (GTK_WINDOW (message_dialog), "Monster Battle");
   gtk_window_set_position( GTK_WINDOW(message_dialog), GTK_WIN_POS_CENTER) ;
   
   if ( gFire_Count >= 3 ) label = gtk_label_new( "You win the this game." ) ;
   else label = gtk_label_new( "You lose the this game." ) ;
   
   gtk_container_add(GTK_WINDOW(message_dialog), label ) ;
   gtk_widget_show_all( message_dialog ) ;
   Write_To_Database() ;
   pthread_create( &t_CloseMessage_Thread, NULL, (void*)Close_Message_Method, NULL ) ;
   gtk_main() ;  
  
   battle_audio_mark  = 1 ;
   Menu_Main() ;
}  

// ======================================================
void Solve_Bonus() { 
   char recv_data[100] ;
   memset( recv_data, '\0', 100 ) ;
   send( g_fd, gProperty, 1,  0 ) ;
   recv( g_fd, recv_data, sizeof(recv_data), 0 ) ;
  
   send( g_fd, gAccount, sizeof(gAccount), 0 ) ;
   recv( g_fd, enemy_Name, sizeof(enemy_Name), 0 ) ;
   g_print( "Type:%s EnemyType:%s Enemy_Name:%s\n", gProperty, recv_data, enemy_Name ) ;

   if ( gProperty[0] == recv_data[0] ) {
      gBonus = 1 ;
   } // if
   else if ( gProperty[0] == '1' ) {
     if ( recv_data[0] == '2' ) gBonus = 2 ;
     else gBonus = 1 ;
   } // else if
   else if ( gProperty[0] == '2' ) {
     if ( recv_data[0] == '3' ) gBonus = 2 ;
     else gBonus = 1 ; 
   } // else if
   else if ( gProperty[0] == '3' ) {
     if ( recv_data[0] == '1' ) gBonus = 2 ;
     else gBonus = 1 ;
   } // else if
 
   g_print( "Bonus is %d\n" , gBonus ) ;
} // Solve_Bonus

// ==========================================================
// ==========================================================
// KEY PAD AREA
// =========================================================
void KeyPad_Method() {
   char *dev_name = "/dev/keypad" ;
   int data = 0 , pre_data ;
   gfloat count = 0 ;
   gfloat press = 0.05 * gBonus ;
   keypad_fd = open( dev_name , O_RDWR );

   if( !( keypad_fd >= 0 ) ) {
      printf("%s file open failed\n", dev_name );
      exit(-1);
   } // if
   
   g_print( "KeyPad will be start\n" ) ;
   ioctl( keypad_fd, LED_SHOW, 1 ) ;

   while( 1 ) {

      do {
         //this will be modify
         if ( delay_mark != 0 ) {
            //  show animation
            if ( count < 0.3 ) count = 0 ;     
            else count = count - 0.3 ;

            gtk_progress_bar_update( progress_bar, count ) ;   
            Show_From_Enymy_Attack() ;
            if ( delay_mark == 2 ) {
               // here will be close BattleActivity
               gtk_widget_destroy( battle_Window ) ;
               gtk_main_quit() ;
               return ;
            } // if
            delay_mark = 0 ;
         } // if

         pre_data = data;
         read( keypad_fd, (char * )&data, sizeof(data) );
         data = (data & 0xff);
      }while(data == 0);

      if( pre_data == 0) {
         printf("Write %d LED\n",data);
         
         count = count + press ;
         gtk_progress_bar_update( progress_bar, count ) ;
         if ( count > 0.99 ) {
            send( g_fd, "Fire_Attack", 11, 0 ) ;
            gFire_Count ++ ;
            g_print( "yor attack enemy\n" ) ;
            Show_Attack_Enymy() ;
            count = 0 ;
            gtk_progress_bar_update( progress_bar, count ) ;
            if ( gFire_Count == 3 ) {
	       send( g_fd, "LOSE", 4, 0 ) ;
               g_print( "You Win the This Game.\n" ) ;
 	       gtk_widget_destroy( battle_Window ) ;
               gtk_main_quit() ;
               // here will be close BattleActivity
               return ;
            } // if
         } // if

         // gtk_progress_get_current_percentage
         // gtk_fixed_put( GTK_FIXED( fixed), fire_image, 30, 0 ) ;
         write( keypad_fd, (const char *)&data, sizeof((const char )data) );
      } // if
   } // while()   

} // KeyPad_Mathod()

// ==========================================================================
void Listen_Method() {
   char data[100] ;
   while ( 1 ) { 
      memset( data, '\0', 100 ) ;
      recv( g_fd, data, sizeof(data), 0 ) ;
      if ( strcmp( data, "Fire_Attack" ) == 0 ) {
         delay_mark = 1 ;
      } // if
      else if ( strcmp( data, "LOSE") == 0 ) {
         delay_mark = 2 ;
         g_print( "You Lose this game\n" ) ;
         break ;
      } // else if
   } // while
} // Listen_Method() 

// ============================================================================
void Write_To_Database() {
   FILE *fp, *w_fp;
   char data[500], temp[50], digit[3], name[20] ;
   int n = 0, m = 0, num = 0 ;

   memset( data, '\0', 500 ) ;
   memset( temp, '\0', 50 ) ;
   memset( digit, '\0', 3 ) ;
   fp = fopen( gAccount, "r" ) ;

   fscanf( fp, "%s", temp ) ;
   strcat( data, temp ) ;
   strcat( data, "\n" ) ;

   memset( temp, '\0', 50 ) ;
   fscanf( fp, "%s", temp ) ;
   strcat( data, temp ) ;
   strcat( data, "\n" ) ;
   
   memset( temp, '\0', 50 ) ;
   fscanf( fp, "%s", temp ) ;
   Check_Add_WIN_LOSE( temp, digit, 1 ) ;   
   strcat( data, "WINS:" ) ;
   strcat( data, digit ) ;

   memset( digit, '\0', 3 ) ;
   memset( temp, '\0', 50 ) ;
   fscanf( fp, "%s", temp ) ;
   Check_Add_WIN_LOSE( temp, digit, 0 ) ;
   strcat( data, " LOSE:" ) ;
   strcat( data, digit ) ;
   strcat( data, "\n" ) ;

   memset( temp, '\0', 50 ) ;
   while ( !feof(fp) ) {
      fscanf( fp, "%s", temp) ;
      strcat( data, temp ) ;
      strcat( data, "\t\t" ) ;
      memset( temp, '\0', 50 ) ;
      fscanf( fp, "%s", temp ) ;
      strcat( data, temp ) ;
      memset( temp, '\0', 50 ) ;
      strcat( data, "\n" ) ;
   } // for

   fclose( fp ) ;
  
   memset( name, '\0', 20 ) ;
   for ( n = 22 ; enemy_Name[n] != '\0' ; n++ ) {
      name[m] = enemy_Name[n] ;
      m++ ;
   } // for

   strcat( data, name ) ;
   strcat( data, "\t\t" ) ;
   if ( gFire_Count >= 3 ) strcat( data, "WIN" ) ;
   else strcat( data, "LOSE" ) ;

   // to...overrid the account data
   w_fp = fopen( gAccount, "w+" ) ;
   fprintf( w_fp, "%s", data ) ;
   fclose(w_fp) ; 
} // Write_To_Database()

void Check_Add_WIN_LOSE( char temp[50] ,char digit[3], int mark_win ) {
   int num = 0, x , n = 0 ;
   for ( x = 5 ; temp[x] != '\0' ; x++ ) {
      digit[n] = temp[x] ;
      n++ ;
   } // for

   num = atoi( digit ) ;
   if ( mark_win && gFire_Count >= 3 ) num++ ;
   else if ( !mark_win && gFire_Count < 3 ) num++ ;

   memset( digit, '\0', 3 ) ;
   sprintf( digit, "%d", num ) ;
} // Check_Add_WIN_LOSE

// =========================================================================
void Audio_Background_Method() {
    // system( "cat DataBase/audio/battle.wav > /dev/dsp" ) ;
    while ( 1 ) {
        int fp_src;
        int fp_dst;
        unsigned char buf[BUFFER_SIZE];
        int arg;        // argument for ioctl calls
        int status;  // return status of system calls


        fp_src = open( "DataBase/audio/battle_background.wav", O_RDWR );
        fp_dst = open( "/dev/dsp", O_RDWR );
        printf( "-------->Play Background Audio Start\n" );

        while( battle_audio_mark != 1  && read( fp_src, buf, BUFFER_SIZE ) >= BUFFER_SIZE )
        {
                write( fp_dst, buf, BUFFER_SIZE );
        }
 
        close( fp_src );
        close( fp_dst );
	if ( battle_audio_mark == 1 ) return ;
   } // while

} // Audio_Background_Method()
// ===========================================================================
void Close_Message_Method() {
   sleep( 2 ) ;
   gtk_widget_destroy( message_dialog ) ;
   gtk_main_quit() ;
} // Close_Message_Method()
// =============================================================================
void Show_From_Enymy_Attack() {
   sleep( 1 ) ;
   gtk_fixed_put( GTK_FIXED( fixed), fire_image, 0, 0 ) ;
   sleep( 1 ) ;
   gtk_fixed_put( GTK_FIXED( fixed), fire_image, 40, 0 ) ;
   sleep( 1 ) ;
   gtk_fixed_put( GTK_FIXED( fixed), fire_image, 90, 0 ) ;
   sleep( 1 ) ;
   gtk_fixed_put( GTK_FIXED( fixed), fire_image, 150, 100 ) ;
   // system( "cat DataBase/audio/bomb.wav > /dev/dsp" ) ;
} // Show_From_Enymy_Attack()

void Show_Attack_Enymy() {
   // system( "cat DataBase/audio/fire.wav > /dev/dsp" ) ;
   gtk_fixed_put( GTK_FIXED( fixed), fire_image, 90, 0 ) ;
   sleep( 1 ) ;
   gtk_fixed_put( GTK_FIXED( fixed), fire_image, 40, 0 ) ;
   sleep( 1 ) ;
   gtk_fixed_put( GTK_FIXED( fixed), fire_image, 0, 0 ) ;
   sleep( 1 ) ;
   gtk_fixed_put( GTK_FIXED( fixed), fire_image, 150, 100 ) ;
   sleep( 1 ) ;
} // Show_From_Enymy_Attack()






