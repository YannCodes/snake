CC=gcc
CFLAGS=-Wall
LFLAGS=-lncurses
MXE=/opt/mxe/usr/bin/
MXE_CC=$(MXE)i686-w64-mingw32.static-gcc
MXE_LFLAGS=-lncurses -lpthread
SRC=main.c list.h list.c snake.h snake.c

debug:clean
	$(CC) $(CFLAGS) -g -o snake $(SRC) $(LFLAGS)
stable:clean
	$(CC) $(CFLAGS) -o snake $(SRC) $(LFLAGS)
win:clean
	$(MXE_CC) $(CFLAGS) -DWIN -o snake.exe $(SRC) $(MXE_LFLAGS)
clean:
	rm -vfr *~ snake snake.exe
