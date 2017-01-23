SRC = $(shell find src -name '*.c') data/*.c
TEST_SRC = $(SRC) test/*.c
CFLAGS = -g -Wall -Wextra -ftrapv -Wshadow -Wundef -Wcast-align -Wunreachable-code -l ncurses -l m -isystem lib -D _GNU_SOURCE


.PHONY: test


run-debug:
	$(CC) $(SRC) main.c $(CFLAGS) -D DEBUG_MODE -o rogue.o
	./rogue.o

test:
	$(CC) $(TEST_SRC) $(CFLAGS) -D DEBUG_MODE -o test.o
	./test.o

test-valgrind:
	make test
	valgrind --track-origins=yes --leak-check=full --show-reachable=yes ./test.o

palette:
	$(CC) palette.c $(CFLAGS) -o palette.o
	./palette.o
