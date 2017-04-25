DIR_BUILD = build
DIR_ROOT = $(shell pwd)
DIR_CONFIG = $(DIR_ROOT)/config

DIR_INSTALLED_CONFIG_BASE=.config/rogue-craft
DIR_INSTALLED_CONFIG=${HOME}/$(DIR_INSTALLED_CONFIG_BASE)

DIR_CONFIG_ENV = $(DIR_CONFIG)/environments
DIR_INSTALLED_ENV_BASE = $(DIR_INSTALLED_CONFIG_BASE)/environments
DIR_INSTALLED_ENV = $(DIR_INSTALLED_CONFIG)/environments

DIR_INSTALLED_CACHE_BASE = .cache/rogue-craft
DIR_INSTALLED_CACHE=${HOME}/$(DIR_INSTALLED_CACHE_BASE)

DIR_INSTALLED_BIN=/usr/bin
DIR_TAR_ROOT=$(DIR_ROOT)/$(VERSION_FULL)
DIR_TAR=$(DIR_TAR_ROOT)/$(TARGET)

CONFIG_FILES=$(shell find $(DIR_CONFIG)/* -type d -not -name "environments")

CC = gcc
LIBS = -l:libncursesw.so.5 -l:libpanelw.so.5 -l m -l rt -ldl
GLOBAL_DEFINITIONS = -DENV_DIR_USER=\"HOME\"
DEFINITIONS = -DDIR_ENV=\"$(DIR_INSTALLED_ENV_BASE)\" $(GLOBAL_DEFINITIONS) $(VERSION_DEFINITIONS)
INCLUDES = -I lib/mem-pool/src -I lib/collection/src -I lib/tinydir -I lib/parson -I lib
CFLAGS = -std=gnu11 -g -Wall -Wextra -ftrapv -Wshadow -Wundef -Wcast-align -Wunreachable-code


TARGET = rogue-craft
TEST_TARGET = $(TARGET)-test

VERSION_MAJOR=0
VERSION_MINOR=0
VERSION_PATCH=0
VERSION_LABEL=alpha
VERSION_FULL=$(VERSION_LABEL)-$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)

VERSION_DEFINITIONS=-DVERSION_MAJOR=$(VERSION_MAJOR) -DVERSION_MINOR=$(VERSION_MINOR) -DVERSION_PATCH=$(VERSION_PATCH) -DVERSION_FULL=\"$(VERSION_FULL)\"

TAR_NAME=$(TARGET)-$(VERSION_FULL)_$(shell uname -o -p | sed -e 's/\//_/g; s/ /_/g').tar.gz


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
	make install-environments

$(TEST_TARGET): $(TEST_OBJECTS)
	$(CC) $(TEST_OBJECTS) $(CFLAGS) $(LIBS) -o $@
	make install-environments

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

install-environments:
	mkdir -p $(DIR_INSTALLED_ENV)
	cd $(DIR_CONFIG_ENV) &&                                     \
	$(foreach file,$(shell ls $(DIR_CONFIG_ENV)),               \
	$(eval so=$(basename $(file)).so)                           \
		gcc $(GLOBAL_DEFINITIONS) -fPIC ./$(file) -shared -Wl,-soname,$(so) -o $(so)  \
	;)                                                          \
	mv ./*.so $(DIR_INSTALLED_ENV)

install:
	mkdir -p $(DIR_INSTALLED_CACHE)
	mkdir -p $(DIR_INSTALLED_CONFIG)
	cp -r $(CONFIG_FILES) $(DIR_INSTALLED_CONFIG)
	sudo cp $(TARGET) $(DIR_INSTALLED_BIN)
	rm -rf $(DIR_INSTALLED_CACHE)/*.cache

clean:
	rm -rf $(DIR_ROOT)/$(DIR_BUILD)

tar-installer:
	cp lib/dev/tar_install.sh install.sh
	sed -i -e 's#target#\"$(TARGET)\"#g; s#dir_bin#"$(DIR_INSTALLED_BIN)"#g; s#dir_config#$(DIR_INSTALLED_CONFIG_BASE)#g; s#dir_cache#$(DIR_INSTALLED_CACHE_BASE)#g' install.sh

tar:
	make
	make tar-installer
	mkdir -p $(DIR_TAR)/config/environments
	gcc $(GLOBAL_DEFINITIONS) -fPIC $(DIR_CONFIG_ENV)/production.c -shared -Wl,-soname,production.so -o production.so
	mv production.so $(DIR_TAR)/config/environments
	cp -r $(CONFIG_FILES) $(DIR_TAR)/config
	cp $(TARGET) $(DIR_TAR)
	cp install.sh $(DIR_TAR)
	cd $(DIR_TAR_ROOT) && \
	tar -cvf $(TAR_NAME) * && \
	mv $(TAR_NAME) $(DIR_ROOT)
	rm -rf $(DIR_TAR_ROOT)
