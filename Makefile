all: Game


CC = arm-linux-gcc
LIBS = -lm
#CFLAGS = -Wall -ansi -g 
PKG_CONFIG += -g $(shell pkg-config --cflags --libs gthread)
GTK_CFLAGS = -I/usr/local/hybus-arm-linux-R1.1/arm-linux/include/gtk-1.2 -I/usr/local/hybus-arm-linux-R1.1/arm-linux/lib/glib/include -I/usr/local/hybus-arm-linux-R1.1/arm-linux/X11R6-arm/include
GTK_CONFIG = /usr/local/hybus-arm-linux-R1.1/bin/gtk-config
GTK_LIBS = -L/usr/local/hybus-arm-linux-R1.1/arm-linux/lib -L/usr/local/hybus-arm-linux-R1.1/arm-linux/X11R6-arm/lib -lgtk -lgdk -rdynamic -lgmodule -lglib -ldl -lXi -lXext -lX11 -lm -gthread

LIBS += ${GTK_CFLAGS}  ${GTK_LIBS} ${PKG_CONFIG}

Game: menu.o main.o battle.o loggin.o choose.o
	${CC} ${LIBS} menu.o main.o battle.o loggin.o choose.o  -lpthread -o Game
main.o: main.c
	${CC} $(LIBS) -lpthread -c main.c

menu.o: menu.c
	${CC} $(LIBS) -lpthread -c menu.c
battle.o: battle.c
	${CC} $(LIBS) -lpthread -c battle.c
loggin.o: loggin.c
	${CC} $(LIBS) -lpthread -c loggin.c
choose.o: choose.c
	${CC} $(LIBS) -lpthread -c choose.c

clean:
	rm -rf *.o gtk_test
