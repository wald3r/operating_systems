CFLAGS = -std=gnu99
CC=gcc
LIB = -lrt -pthread


all: customer server

customer: customer.c
	$(CC) $(CFLAGS) customer.c -o customer $(LIB)

server: server.c
	$(CC) $(CFLAGS) server.c -o server $(LIB)

.PHONY: clean
clean:
	$(RM) customer server

