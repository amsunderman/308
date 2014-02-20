CC=gcc
DEPS = message.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $<
lab1: lab1.o message.o
	$(CC) -o lab1 lab1.o message.o
clean:
	rm -rf *.o *~ lab1
