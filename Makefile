SRC_DIR = src
BIN_DIR = bin
TEMPLATE_DIR = $(SRC_DIR)/_template

CC=/usr/bin/g++
CFLAGS += -std=c++20 -I include/

ifeq ($(DEBUG), 1)
	CFLAGS += -g
else
	CFLAGS += -O3
endif

CHALLENGE_NR ?= $(shell date '+%d')
CHALLENGE_DIR = $(SRC_DIR)/day-$(CHALLENGE_NR)

-include .env

run.%: $(CHALLENGE_DIR)/solution.%.bin $(CHALLENGE_DIR)/input.txt
	$< < $(CHALLENGE_DIR)/input.txt

sample.%: $(CHALLENGE_DIR)/solution.%.bin $(CHALLENGE_DIR)/sample.txt
	$< < $(CHALLENGE_DIR)/sample.txt

clean:
	rm -f $(CHALLENGE_DIR)/solution.a.bin $(CHALLENGE_DIR)/solution.b.bin

%.bin: %.cpp
	$(CC) $(CFLAGS) -o $@ $<

%/solution.b.cpp: %/solution.a.cpp
	@[ ! -f $@ ] && cp $< $@ || true

%/solution.a.cpp:
	cp -r $(TEMPLATE_DIR) $(dir $@)

%/input.txt:
	$(eval DAY=$(patsubst 0%,%,$(patsubst $(SRC_DIR)/day-%/input.txt,%,$@)))
	curl 'https://adventofcode.com/2022/day/$(DAY)/input' -H 'cookie: session=$(SESSION)' --compressed > $@

%/sample.txt:
	touch $@

.PRECIOUS: %/input.txt %/sample.txt %/solution.a.cpp %/solution.b.cpp %.bin

include util/leaderboard.makefile
