CACHE_FILE := leaderboard.cache

fetch-leaderboard: update-leaderboard-ts leaderboard.json

update-leaderboard-ts:
	touch -d "-1 hour" $(CACHE_FILE)

leaderboard.json: $(CACHE_FILE)
	curl 'https://adventofcode.com/2022/leaderboard/private/view/1483124.json' -H 'cookie: session=$(SESSION)' --compressed > $@

$(CACHE_FILE):
	touch $@

.PHONY: update-leaderboard-ts
