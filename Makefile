SRC = $(shell find src lib/*/src config -path '*.c') lib/parson/parson.* lib/tinydir/tinydir.h
TEST_SRC = $(SRC) $(shell find test -path '*.c')
CFLAGS = -std=c11 -g -Wall -Wextra -ftrapv -Wshadow -Wundef -Wcast-align -Wunreachable-code -I lib/mem-pool/src -I lib/worker/src -I lib/collection/src -I lib/tinydir -I lib/parson -l ncursesw -l pthread -l m -isystem lib -D _GNU_SOURCE


.PHONY: test


run-debug:
	$(CC) $(SRC) main.c $(CFLAGS) -D DEBUG_MODE -D DIR_CONFIG=\"./config\" -D DIR_CACHE=\"./cache\" -o rogue.o
	./rogue.o

test:
	$(CC) $(TEST_SRC) $(CFLAGS) -D UNIT_TEST -D DEBUG_MODE -o test.o
	./test.o

test-valgrind:
	make test
	valgrind --track-origins=yes --leak-check=full --show-reachable=yes ./test.o

palette:
	make -C ./lib/dev palette

cache-clear:
	rm -rf ./cache/*.cache
