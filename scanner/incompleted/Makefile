SHELL = /bin/sh

OBJS =  scanner.o reader.o charcode.o token.o
CFLAG = -Wall -g
CC = gcc
INCLUDE =
LIBS = -lm

scanner:${OBJS}
	${CC} ${CFLAGS} ${INCLUDES} -o $@ ${OBJS} ${LIBS}

clean:
	-rm -f *.o core *.core

.c.o:
	${CC} ${CFLAGS} ${INCLUDES} -c $<
