CC		= gcc
DEFS	= -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_SVID_SOURCE -D_POSIX_C_SOURCE=200809L
CFLAGS	= -std=c99 -pedantic -Wall $(DEFS) -g
TARN	= 2-fb_arc_set-todorovic.tar.gz

.PHONY: all clean

all: supervisor generator

supervisor: supervisor.o
	@$(CC) supervisor.o -o supervisor

supervisor.o: supervisor.c
	@$(CC) $(CFLAGS) -c supervisor.c

generator: generator.o graph_util.o
	@$(CC) -o generator generator.o graph_util.o

generator.o: generator.c graph_util.h
	@$(CC) $(CFLAGS) -c -o generator.o generator.c

graph_util.o: graph_util.c graph_util.h
	@$(CC) $(CFLAGS) -c -o graph_util.o graph_util.c

clean:
	@rm -rf *.o supervisor generator $(TARN)

pack:
	@tar -cvzf $(TARN) *.c *.h Makefile
