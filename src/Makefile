CC		= gcc
DEFS	= -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_SVID_SOURCE -D_POSIX_C_SOURCE=200809L
CFLAGS	= -std=c99 -pedantic -Wall -g $(DEFS)
TARN	= 2-fb_arc_set-todorovic.tar.gz

.PHONY: all clean

all: supervisor #generator

supervisor: supervisor.o
	@$(CC) supervisor.o -o supervisor

supervisor.o: supervisor.c
	@$(CC) $(CFLAGS) -c supervisor.c

generator: generator.o
	@$(GCC) generator.o -o generator

generator.o: generator.c
	@$(GCC) $(CFLAGS) -c generator.c

clean:
	@rm -rf *.o supervisor generator $(TARN)

pack:
	@tar -cvzf $(TARN) *.c *.h Makefile