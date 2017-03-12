SRC = $(shell find src config -name '*.c')
LIB = $(shell find lib/*/src -name '*.c') lib/parson/parson.c
TEST_SRC = $(SRC) $(shell find test -name '*.c')
CFLAGS = -std=c11 -g -Wall -Wextra -ftrapv -Wshadow -Wundef -Wcast-align -Wunreachable-code -I lib/mem-pool/src -I lib/worker/src -I lib/collection/src -I lib/tinydir -I lib/parson -l ncursesw -l pthread -l m -isystem lib -D _GNU_SOURCE

OBJ = rogue-craft
TEST_OBJ = rogue-craft-test
DEP_LIB = rogoue-craft-deps
DEP_OBJ = lib$(DEP_LIB).so

PWD = $(shell pwd)


.PHONY: test lib


dependency-update:
	git submodule foreach git pull origin master
	$(CC) -c -fpic $(CFLAGS) $(LIB)
	$(CC) *.o -shared -o $(DEP_OBJ)
	rm -f *.o
	sudo mv $(DEP_OBJ) /usr/lib

run-debug:
	$(CC) $(SRC) main.c $(CFLAGS) -l$(DEP_LIB) -D DEBUG_MODE -D DIR_CONFIG=\"./config\" -D DIR_CACHE=\"./cache\" -o $(OBJ)
	./$(OBJ)

test:
	rm -rf ./test/fixture/cache/*.cache
	$(CC) $(TEST_SRC) $(CFLAGS) -l$(DEP_LIB) -D UNIT_TEST -D DEBUG_MODE -D DIR_CONFIG=\"./test/fixture/config\" -D DIR_CACHE=\"./test/fixture/cache\" -o $(TEST_OBJ)
	./$(TEST_OBJ)

test-valgrind:
	make test
	valgrind --track-origins=yes --leak-check=full --show-reachable=yes ./$(TEST_OBJ)

palette:
	make -C ./lib/dev palette

cache-clear:
	rm -rf ./cache/*.cache
