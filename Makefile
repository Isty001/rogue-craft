DIR_BUILD = build
DIR_ROOT = $(shell pwd)
DIR_RESOURCES = $(DIR_ROOT)/resources
DIR_APP = .rogue-craft

DIR_INSTALLED_RESOURCES_BASE=$(DIR_APP)/resources
DIR_INSTALLED_RESOURCES=${HOME}/$(DIR_INSTALLED_RESOURCES_BASE)

DIR_INSTALLED_ENV_BASE = $(DIR_APP)/environments
DIR_INSTALLED_ENV = ${HOME}/$(DIR_INSTALLED_ENV_BASE)

DIR_INSTALLED_CACHE_BASE = $(DIR_APP)/cache
DIR_INSTALLED_CACHE=${HOME}/$(DIR_INSTALLED_CACHE_BASE)

DIR_INSTALLED_BIN=/usr/bin
DIR_TAR_ROOT=$(DIR_ROOT)/$(VERSION_FULL)
DIR_TAR=$(DIR_TAR_ROOT)/$(TARGET)

CC = gcc
#Debian/Ubuntu: Ncurses: To compile: ncursesw-dev, Run: ncursesw
#VLC: Compile: libvlccore-dev libvlc-dev Run: vlc
LIBS = -l ncursesw -l panelw -l menuw -l m -lvlc
DEFINITIONS = -DDIR_APP_RELATIVE=\"$(DIR_APP)\" $(VERSION_DEFINITIONS)

#This way we can avoid nasty includes like #include "../../../config/config.h"
NAMESPACES = -I config -I src
INCLUDES = $(NAMESPACES) -I lib/mem-pool/src -I lib/collection/src -I lib/tinydir -I lib/parson -I lib/quadtree/src -I lib/dotenv/src -I lib/rimraf/src -I lib
CFLAGS = -std=gnu11 -g -Wall -Wextra -ftrapv -Wshadow -Wundef -Wcast-align -Wunreachable-code


TARGET = rogue-craft
TEST_TARGET = $(TARGET)-test

VERSION_MAJOR=0
VERSION_MINOR=0
VERSION_PATCH=0
VERSION_LABEL=beta
VERSION_FULL=$(VERSION_LABEL)-$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)

VERSION_DEFINITIONS=-DVERSION_MAJOR=$(VERSION_MAJOR) -DVERSION_MINOR=$(VERSION_MINOR) -DVERSION_PATCH=$(VERSION_PATCH) -DVERSION_FULL=\"$(VERSION_FULL)\"

TAR_NAME=$(TARGET)-$(VERSION_FULL)_$(shell uname -o -p | sed -e 's/\//_/g; s/ /_/g').tar.gz


.PHONY: default all clean $(TARGET) $(TEST_TARGET) test


LIB_SOURCES = $(shell find lib -name "*.c" | grep -E -v "test|samples|dev|benchmark")
COMMON_SOURCES = $(LIB_SOURCES) $(shell find src config -name "*.c")
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
	make install-environments

$(TEST_TARGET): $(TEST_OBJECTS)
	$(CC) $(TEST_OBJECTS) $(CFLAGS) $(LIBS) -o $@
	make install-environments

prepare-test:
	rm -f test/fixture/cache/*.cache
	rm -rf test/fixture/saved_games/*

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

install-environments:
	mkdir -p $(DIR_INSTALLED_ENV)
	cp ./config/environments/.env.* $(DIR_INSTALLED_ENV)

install:
	mkdir -p $(DIR_INSTALLED_CACHE)
	mkdir -p $(DIR_INSTALLED_RESOURCES)
	cp -r $(DIR_RESOURCES)/* $(DIR_INSTALLED_RESOURCES)
	sudo cp $(TARGET) $(DIR_INSTALLED_BIN)
	rm -rf $(DIR_INSTALLED_CACHE)/*.cache

clean-cache:
	rm -rf ./cache/*.cache

clean:
	rm -rf $(DIR_ROOT)/$(DIR_BUILD)/*
	make clean-cache

tar-installer:
	cp lib/dev/tar_install.sh install.sh
	sed -i -e 's#target#\"$(TARGET)\"#g; s#dir_bin#"$(DIR_INSTALLED_BIN)"#g; s#dir_resources#$(DIR_INSTALLED_RESOURCES_BASE)#g; s#dir_cache#$(DIR_INSTALLED_CACHE_BASE)#g' install.sh

tar:
	make
	make tar-installer
	mkdir -p $(DIR_TAR)/environments
	mkdir -p $(DIR_TAR)/resources
	cp config/environments/.env.prod $(DIR_TAR)/environments
	cp -r $(DIR_RESOURCES)/* $(DIR_TAR)/resources
	mv $(TARGET) $(DIR_TAR)
	chmod 777 install.sh
	mv install.sh $(DIR_TAR)
	cd $(DIR_TAR_ROOT) && \
	tar -cvpf $(TAR_NAME) * && \
	mv $(TAR_NAME) $(DIR_ROOT)
	rm -rf $(DIR_TAR_ROOT)
