CFLAGS = -Werror -Wall -Wextra -std=c99
CC=gcc
LIB = -lrt -pthread


all: consumer service producer

consumer: consumer.c
	$(CC) $(CFLAGS) consumer.c -o consumer $(LIB)

service: service.c
	$(CC) $(CFLAGS) service.c -o service $(LIB)

producer: producer.c
	$(CC) $(CFLAGS) producer.c -o producer $(LIB)

.PHONY: clean
clean:
	$(RM) consumer producer service

