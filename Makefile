SRC = $(shell find src config -name '*.c')
LIB = $(shell find lib/*/src -name '*.c') lib/parson/parson.c
TEST_SRC = $(SRC) $(shell find test -name '*.c')
CFLAGS = -std=c11 -g -Wall -Wextra -ftrapv -Wshadow -Wundef -Wcast-align -Wunreachable-code -I lib/mem-pool/src -I lib/worker/src -I lib/collection/src -I lib/tinydir -I lib/parson -l ncursesw -l panel -l m -l rt -isystem lib -D _GNU_SOURCE

OBJ = rogue-craft
TEST_OBJ = rogue-craft-test
DEP_LIB = rogoue-craft-deps
DEP_OBJ = lib$(DEP_LIB).so

CONFIG_FILES=$(shell find config/* -type d)

INSTALLED_DIR_CACHE="${HOME}/.cache/rogue-craft"
INSTALLED_DIR_CONFIG="${HOME}/.config/rogue-craft"
INSTALLED_DIR_BIN=/usr/bin


.PHONY: test lib


build:
	make env-setup
	make dependency-update
	make compile cache=$(INSTALLED_DIR_CACHE) cfg=$(INSTALLED_DIR_CONFIG) o=$(OBJ)

install:
	cp -r $(CONFIG_FILES) $(INSTALLED_DIR_CONFIG)
	sudo cp $(OBJ) $(INSTALLED_DIR_BIN)

env-setup:
	mkdir -p $(INSTALLED_DIR_CACHE)
	mkdir -p $(INSTALLED_DIR_CONFIG)
	rm -rf $(INSTALLED_DIR_CACHE)/*.cache

dependency-update:
	git submodule update
	$(CC) -c -fpic $(CFLAGS) $(LIB)
	$(CC) *.o -shared -o $(DEP_OBJ)
	rm -f *.o
	sudo mv $(DEP_OBJ) /usr/lib

compile:
	$(CC) $(SRC) main.c $(CFLAGS) -l$(DEP_LIB) $(flags) -D DIR_CONFIG=\"$(cfg)\" -D DIR_CACHE=\"$(cache)\" -o $(o)

run-debug:
	make compile flags="-D DEBUG_MODE" cfg="./config" cache="./cache" o=$(OBJ)
	./$(OBJ)

new-test:
	rm -rf ./test/fixture/cache/*.cache
	$(CC) $(TEST_SRC) $(CFLAGS) -l$(DEP_LIB) -D UNIT_TEST -D DEBUG_MODE -D DIR_CONFIG=\"./test/fixture/config\" -D DIR_CACHE=\"./test/fixture/cache\" -o $(TEST_OBJ)

test:
	make new-test
	./$(TEST_OBJ)

test-valgrind:
	make new-test
	valgrind --track-origins=yes --leak-check=full --show-reachable=yes ./$(TEST_OBJ)

palette:
	make -C ./lib/dev palette

cache-clear:
	rm -rf ./cache/*.cache
