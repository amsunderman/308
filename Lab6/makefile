#makefile for lab 5

#compiler to use
CC=gcc

#executables
ALL=memory_mgmt

all: $(ALL)

memory_mgmt: memory_mgmt.c
	$(CC) $(CFLAGS) -o memory_mgmt memory_mgmt.c

clean:
	rm -f $(ALL) *.o
