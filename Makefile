SRC = $(shell find src lib/*/src -name '*.c') config/*.c
TEST_SRC = $(SRC) test/*.c
CFLAGS = -std=c11 -g -Wall -Wextra -ftrapv -Wshadow -Wundef -Wcast-align -Wunreachable-code -I lib/mem-pool/src -l ncursesw  -l m -isystem lib -D _GNU_SOURCE


.PHONY: test


run-debug:
	$(CC) $(SRC) main.c $(CFLAGS) -D DEBUG_MODE -o rogue.o
	./rogue.o

test:
	$(CC) $(TEST_SRC) $(CFLAGS) -D UNIT_TEST -D DEBUG_MODE -o test.o
	./test.o

test-valgrind:
	make test
	valgrind --track-origins=yes --leak-check=full --show-reachable=yes ./test.o
