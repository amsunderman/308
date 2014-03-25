#makefile to ensure Project2 is up to date
#usage: simply type "make" to compile all files that you need recompiled
#use make clean to remove .o and executable files.

#compiler to use
CC=gcc

#all exectubles
ALL=appserver

all: $(ALL)

#executables
appserver: appserver.o Bank.o
	$(CC) -pthread -g -o appserver appserver.o Bank.o

#object files
appserver.o: appserver.c
	$(CC) -pthread -g -c appserver.c
Bank.o: Bank.c
	$(CC) -c -g Bank.c

#cleanup files
clean:
	rm -f $(ALL) *.o
