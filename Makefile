SRC = $(shell find src config -name '*.c' -not -path "config/environments/*")
LIB = $(shell find lib/*/src -name '*.c') lib/parson/parson.c
TEST_SRC = $(SRC) $(shell find test -name '*.c')
CFLAGS = -std=c11 -g -Wall -Wextra -ftrapv -Wshadow -Wundef -Wcast-align -Wunreachable-code -I lib/mem-pool/src -I lib/worker/src -I lib/collection/src -I lib/tinydir -I lib/parson -DDIR_ENV_CONFIG=\"$(INSTALLED_DIR_ENV_CONFIG)\" -DENV_SETUP_ENTRY_POINT=load_env_setup -l ncursesw -l panel -l m -l rt -ldl -rdynamic -isystem lib -D _GNU_SOURCE

OBJ = rogue-craft
TEST_OBJ = rogue-craft-test
DEP_LIB = rogoue-craft-deps
DEP_OBJ = lib$(DEP_LIB).so

CONFIG_FILES=$(shell find config/* -type d)

DIR_ENV_CONFIG=./config/environments

INSTALLED_DIR_CACHE=${HOME}/.cache/rogue-craft
INSTALLED_DIR_CONFIG=${HOME}/.config/rogue-craft
INSTALLED_DIR_ENV_CONFIG=$(INSTALLED_DIR_CONFIG)/environments
INSTALLED_DIR_BIN=/usr/bin


.PHONY: test lib


build:
	make env-setup
	make dependency-update
	make compile o=$(OBJ)

install:
	cp -r $(CONFIG_FILES) $(INSTALLED_DIR_CONFIG)
	sudo cp $(OBJ) $(INSTALLED_DIR_BIN)
	rm -rf $(INSTALLED_DIR_CACHE)/*.cache

build-environments:
	rm -f $(INSTALLED_DIR_ENV_CONFIG)/*.so
	cd $(DIR_ENV_CONFIG) &&                                     \
	$(foreach file,$(shell ls $(DIR_ENV_CONFIG)),               \
	$(eval so=$(basename $(file)).so)                           \
		gcc -fPIC ./$(file) -shared -Wl,-soname,$(so) -o $(so)  \
	;)                                                          \
	mv ./*.so $(INSTALLED_DIR_ENV_CONFIG)

env-setup:
	mkdir -p $(INSTALLED_DIR_ENV_CONFIG)
	make build-environments

dependency-update:
	git submodule update
	$(CC) -c -fpic $(CFLAGS) $(LIB)
	$(CC) *.o -shared -o $(DEP_OBJ)
	rm -f *.o
	sudo mv $(DEP_OBJ) /usr/lib

compile:
	$(CC) $(SRC) main.c $(CFLAGS) -l$(DEP_LIB) $(flags) -o $(o)

run-debug:
	make env-setup
	make compile o=$(OBJ) flags=-DDEBUG_MODE
	./$(OBJ) --env=debug

new-test:
	rm -rf ./test/fixture/cache/*.cache
	$(CC) $(TEST_SRC) $(CFLAGS) -l$(DEP_LIB) -D UNIT_TEST -D DEBUG_MODE -o $(TEST_OBJ)

test:
	make env-setup
	make new-test
	./$(TEST_OBJ) --env=test

test-valgrind:
	make env-setup
	make new-test
	valgrind --track-origins=yes --leak-check=full --show-reachable=yes ./$(TEST_OBJ) --env=test

palette:
	make -C ./lib/dev palette

cache-clear:
	rm -rf ./cache/*.cache
