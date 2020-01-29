CFLAGS = -Werror -Wall -Wextra -std=c99 -o2
CC=gcc


btree: btree_test.c btree.c
	$(CC) $(CFLAGS) -o btree btree_test.c btree.c 


.PHONY: clean
clean:
	$(RM) btree btree.o btree_test.o

