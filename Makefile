SRC_DIR = src
BIN_DIR = bin
TEMPLATE_DIR = $(SRC_DIR)/_template

CC=/usr/bin/g++
CFLAGS += -I include/

CHALLENGE_NR ?= $(shell date '+%d')
CHALLENGE_DIR = $(SRC_DIR)/day-$(CHALLENGE_NR)

run.%: $(CHALLENGE_DIR)/solution.%.bin
	$< < $(CHALLENGE_DIR)/input.txt

sample.%: $(CHALLENGE_DIR)/solution.%.bin
	$< < $(CHALLENGE_DIR)/sample.txt

%.bin: %.cpp
	$(CC) $(CFLAGS) -o $@ $<

%/solution.c.cpp: %/solution.a.cpp
	[ ! -f $@ ] && cp $< $@ || true

%/solution.a.cpp:
	cp -r $(TEMPLATE_DIR) $(dir $@)

.PRECIOUS: %/solution.a.cpp %/solution.b.cpp %.bin
