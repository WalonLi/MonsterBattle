/*
 * This is include file
 * And it's include all the header
 * All.c will include this file 
 *
 */ 
#ifndef _STDIO_H_
#define _SRDIO_H_
#include <stdio.h>
#endif

#ifndef _STDLIB_H_
#define _STDLIB_H_
#include <stdlib.h>
#endif

#ifndef _GTK_GTK_H_
#define _GTK_GTK_H_
#include <gtk/gtk.h>
#endif

#ifndef _GDK_GDK_H_
#define _GDK_GDK_H_
#include <gdk/gdk.h>
#endif

#ifndef _PTHREAD_H_
#define _PTHREAD_H_
#include <pthread.h>
#endif

#ifndef _UNISTD_H_
#define _UNISTD_H_
#include <unistd.h>
#endif

#ifndef _STRING_H_
#define _STRING_H_
#include <string.h>
#endif


char gAccount[100] ;
void Loggin_Main( void ) ;
void Choose_Main( void ) ;
void Menu_Main( void ) ;
void Battle_Main( int fd ) ;

char gProperty[1] ;
// int enemy_Property = 0 ;

