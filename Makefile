SRC_DIR = src
TEMPLATE_DIR = $(SRC_DIR)/_template

CC=/usr/bin/g++
CFLAGS += -I include/

CHALLENGE_NR ?= $(shell date '+%d')
CHALLENGE_DIR = $(SRC_DIR)/day-$(CHALLENGE_NR)
CHALLENGE_SRC = $(CHALLENGE_DIR)/solution.cpp
CHALLENGE_BIN = $(CHALLENGE_DIR)/solution.bin

run: build
	$(CHALLENGE_BIN) < $(CHALLENGE_DIR)/input.txt

test: build
	$(CHALLENGE_BIN) < $(CHALLENGE_DIR)/sample.txt

build: $(CHALLENGE_BIN)

setup: $(SRC_DIR)/day-$(TODAY)/solution.cpp

%.bin: %.cpp
	$(CC) $(CFLAGS) -o $@ $<

%/solution.cpp:
	cp -r $(TEMPLATE_DIR) $(dir $@)

.PHONY: setup build test run
.PRECIOUS: %/solution.cpp
