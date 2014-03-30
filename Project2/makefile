#makefile to ensure Project2 is up to date
#usage: simply type "make" to compile all files that you need recompiled
#use make clean to remove .o and executable files.

#compiler to use
CC=gcc

#all exectubles
ALL=appserver appserver-coarse

all: $(ALL)

#executables
appserver: appserver.o Bank.o
	$(CC) -pthread -g -o appserver appserver.o Bank.o
appserver-coarse:appserver-coarse.o Bank.o
	$(CC) -pthread -g -o appserver-coarse appserver-coarse.o Bank.o

#object files
appserver.o: appserver.c
	$(CC) -g -c appserver.c
appserver-coarse.o: appserver-coarse.c
	$(CC) -g -c appserver-coarse.c
Bank.o: Bank.c
	$(CC) -g -c Bank.c

#cleanup files
clean:
	rm -f $(ALL) *.o
