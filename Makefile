# Top level declarations
.PHONY: help
help: help_all

.PHONY: build
build: airlite

.PHONY: clean
# clean: clean_all

CC = g++
CFLAGS = -std=c++17 -g -Wall -fPIC
INC_HEADERS = -I.
LIB = -L. -L /usr/local/lib
LINK = -lortools -lprotobuf -lglog
INSTALL = /usr/local/lib

BUILD_CACHE = lib
TEST_CACHE = bin
INTERFACE_CACHE = src/sharing/algo
DATA_CACHE = data


PROJECT_NAME = sharing_manager


# @echo check, check_all:	Check OR-Tools for all available languages.
# @echo test, test_all:	Test OR-Tools for all available languages.
# @echo detect, detect_all:	Show variables used to build OR-Tools for all available languages.
.PHONY: help_usage
help_usage:
	@echo Use one of the following targets:
	@echo
	@echo help, help_all: Print this help.
	@echo build, build_all: Build code for all available library.
	@echo clean, clean_all: Clean output from previous build for all available library \(won\'t clean third party\).
	@echo

.PHONY: help_all
help_all: help_usage

make_build_dir:
	mkdir -p $(BUILD_CACHE)

build_share: make_build_dir sharing.o sharing.so

sharing.o:
	$(CC) $(CFLAGS) $(INC_HEADERS) $(LIB) -c $(INTERFACE_CACHE)/$(PROJECT_NAME).cc -o $(BUILD_CACHE)/$(PROJECT_NAME).o

sharing.so: sharing.o
	$(CC) $(CFLAGS) $(INC_HEADERS) $(LIB) -shared -o $(BUILD_CACHE)/lib$(PROJECT_NAME).so $(BUILD_CACHE)/$(PROJECT_NAME).o $(LINK)
	rm $(BUILD_CACHE)/$(PROJECT_NAME).o



clean:
	rm -rf $(BUILD_CACHE)/*
	rm -rf $(TEST_CACHE)/*

install:
	if [ -f $(BUILD_CACHE)/lib$(PROJECT_NAME).so ]; then cp $(BUILD_CACHE)/lib$(PROJECT_NAME).so $(INSTALL)/lib$(PROJECT_NAME).so; fi

uninstall:
	rm -f $(INSTALL)/lib$(PROJECT_NAME).so

clear_all: clean uninstall


test_build:
	mkdir -p $(TEST_CACHE)
	$(CC) $(CFLAGS) $(INC_HEADERS) $(LIB) -o $(TEST_CACHE)/test src/test/main.cc  -l$(PROJECT_NAME) $(LINK)

test_run:
	mkdir -p $(TEST_CACHE)
	$(TEST_CACHE)/test

all: clear_all build_share install test_build test_run

build_all: clear_all build_share install test_build