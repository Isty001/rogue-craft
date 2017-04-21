DIR_BUILD = build
DIR_ROOT = $(shell pwd)
DIR_CONFIG = $(DIR_ROOT)/config
DIR_CONFIG_ENV = $(DIR_CONFIG)/environments
DIR_INSTALLED_ENV = ${HOME}/.config/rogue-craft/environments
DIR_INSTALLED_CACHE=${HOME}/.cache/rogue-craft
DIR_INSTALLED_CONFIG=${HOME}/.config/rogue-craft
DIR_INSTALLED_BIN=/usr/bin

CC = gcc
LIBS = -l ncursesw -l panel -l m -l rt -ldl
DEFINITIONS = -DDIR_ENV=\"$(DIR_INSTALLED_ENV)\"
INCLUDES = -I lib/mem-pool/src -I lib/collection/src -I lib/tinydir -I lib/parson
CFLAGS = -std=gnu11 -g -Wall -Wextra -ftrapv -Wshadow -Wundef -Wcast-align -Wunreachable-code


TARGET = rogue-craft
TEST_TARGET = $(TARGET)-test


.PHONY: default all clean $(TARGET) $(TEST_TARGET) test


LIB_SOURCES = $(shell find lib -name "*.c" | grep -E -v "test|samples|dev")
COMMON_SOURCES = $(LIB_SOURCES) $(shell find src config -name "*.c" -not -path "config/environments/*")
SOURCES = $(COMMON_SOURCES) main.c
TEST_SOURCES = $(COMMON_SOURCES) $(shell find test -name "*.c")

OBJECTS = $(patsubst %.c, $(DIR_BUILD)/%.o, $(SOURCES))
TEST_OBJECTS = $(patsubst %.c, $(DIR_BUILD)/%.o, $(TEST_SOURCES))

HEADERS = $(shell find . -name "*.h")


default: $(TARGET)
all: default

-include $(shell find $(DIR_BUILD) -name "*.d")


$(DIR_BUILD)/%.o: %.c
	mkdir -p $(shell dirname $@)
	$(CC) -MMD $(CFLAGS) $(INCLUDES) $(DEFINITIONS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) $(LIBS) -o $@
	make build-environments

$(TEST_TARGET): $(TEST_OBJECTS)
	$(CC) $(TEST_OBJECTS) $(CFLAGS) $(LIBS) -o $@
	make build-environments

prepare-test:
	$(eval DEFINITIONS += -DDIR_FIXTURE="test/fixture")
	rm -f test/fixture/cache/*.cache

run-test:
	make prepare-test
	make $(TEST_TARGET)
	./$(TEST_TARGET) --env=test

run-test-valgrind:
	make prepare-test
	make $(TEST_TARGET)
	valgrind --track-origins=yes --leak-check=full --show-reachable=yes ./$(TEST_TARGET) --env=test

run-debug:
	make
	./$(TARGET) --env=dev

build-environments:
	mkdir -p $(DIR_INSTALLED_ENV)
	cd $(DIR_CONFIG_ENV) &&                                     \
	$(foreach file,$(shell ls $(DIR_CONFIG_ENV)),               \
	$(eval so=$(basename $(file)).so)                           \
		gcc -fPIC ./$(file) -shared -Wl,-soname,$(so) -o $(so)  \
	;)                                                          \
	mv ./*.so $(DIR_INSTALLED_ENV)

install:
	mkdir -p $(DIR_INSTALLED_CACHE)
	mkdir -p $(DIR_INSTALLED_CONFIG)
	cp -r $(shell find $(DIR_CONFIG)/* -type d -not -name "environments") $(DIR_INSTALLED_CONFIG)
	sudo cp $(TARGET) $(DIR_INSTALLED_BIN)
	rm -rf $(DIR_INSTALLED_CACHE)/*.cache

clean:
	rm -rf $(DIR_ROOT)/$(DIR_BUILD)
