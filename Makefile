CC = gcc
OBJS = test_wav.o make_wav.o
DEBUG =  -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG) 




p1 : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o p1

make_wav.o: make_wav.c make_wav.h
	$(CC) $(CFLAGS) make_wav.c

test_wav.o: test_wav.c make_wav.h
	$(CC) $(CFLAGS) test_wav.c


clean:
	-rm *.o
	-rm p1

