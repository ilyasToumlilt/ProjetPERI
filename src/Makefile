CC=g++
CFLAGS=-Wall -Wextra -lrf24-bcm -pthread

RF24LIB=
daemon : daemon.o
	$(CC) $(CFLAGS) -o $@ $^

daemon.o : daemon.c daemon.h
	$(CC) $(CFLAGS) -c $<

clean :
	rm -rf daemon *.o *~

