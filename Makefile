CC = gcc
OBJS = operator_wav.o compress_wav.o
DEBUG =  -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG) 




p1 : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o p1 -lm

operator.o: compress_wav.c operator_wav.h
	$(CC) $(CFLAGS) operator.c

compress_wav.o: compress_wav.c operator_wav.h
	$(CC) $(CFLAGS) compress_wav.c


clean:
	-rm *.o
	-rm p1

