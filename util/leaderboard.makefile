CACHE_FILE := leaderboard.cache

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
	TOUCH_CMD := gtouch
else
	TOUCH_CMD := touch
endif

fetch-leaderboard: update-leaderboard-ts leaderboard.json

update-leaderboard-ts: $(CACHE_FILE)
	$(TOUCH_CMD) -md '-1 hour' $(CACHE_FILE)

leaderboard.json: $(CACHE_FILE)
	curl 'https://adventofcode.com/2022/leaderboard/private/view/1483124.json' -H 'cookie: session=$(SESSION)' --compressed > $@

$(CACHE_FILE):
	touch $@

.PHONY: update-leaderboard-ts
