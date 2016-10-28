CC=gcc
CFLAGS="-Wall"
LFLAGS='-lncurses'

debug:clean
	$(CC) $(CFLAGS) -g -o snake main.c list.c list.h snake.c snake.h $(LFLAGS)
stable:clean
	$(CC) $(CFLAGS) -o snake main.c list.c list.h snake.c snake.h $(LFLAGS)
clean:
	rm -vfr *~ snake
