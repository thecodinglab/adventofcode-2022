package main

import (
	"encoding/json"
	"flag"
	"fmt"
	"io"
	"os"
	"sort"
	"time"
)

type Completion struct {
	Index     int   `json:"star_index"`
	Timestamp int64 `json:"get_star_ts"`
}

func (c Completion) Time() time.Time {
	return time.Unix(c.Timestamp, 0)
}

type Member struct {
	ID         int64  `json:"id"`
	Name       string `json:"name"`
	Stars      int    `json:"stars"`
	LocalScore int    `json:"local_score"`
	LastStarTS int64  `json:"last_star_ts"`

	CompletionDayLevel map[string]map[string]Completion `json:"completion_day_level"`
}

func (m Member) PrintableName() string {
	if len(m.Name) == 0 {
		return fmt.Sprintf("Anonymous #%d", m.ID)
	}

	return m.Name
}

func (m Member) FindCompletion(day, level string) Completion {
	return m.CompletionDayLevel[day][level]
}

func parse(reader io.Reader) (map[string]Member, error) {
	val := struct {
		Members map[string]Member
	}{}

	err := json.NewDecoder(reader).Decode(&val)
	return val.Members, err
}

var _ sort.Interface = LeaderboardSorter{}

type LeaderboardSorter struct {
	Members    []Member
	Day, Level string
}

func (s LeaderboardSorter) Len() int {
	return len(s.Members)
}

func (s LeaderboardSorter) Less(i int, j int) bool {
	a := s.Members[i].FindCompletion(s.Day, s.Level).Timestamp
	b := s.Members[j].FindCompletion(s.Day, s.Level).Timestamp
	return a < b
}

func (s LeaderboardSorter) Swap(i int, j int) {
	s.Members[i], s.Members[j] = s.Members[j], s.Members[i]
}

func filterIncomplete(members []Member, day, level string) (res []Member) {
	for _, member := range members {
		if member.FindCompletion(day, level).Timestamp != 0 {
			res = append(res, member)
		}
	}
	return
}

func main() {
	day := "1"
	level := "1"

	flag.StringVar(&day, "day", day, "challenge day")
	flag.StringVar(&level, "level", level, "challenge level")
	flag.Parse()

	file, err := os.Open("leaderboard.json")
	if err != nil {
		panic(err)
	}
	defer file.Close()

	members, err := parse(file)
	if err != nil {
		panic(err)
	}

	v := make([]Member, 0, len(members))
	for _, value := range members {
		v = append(v, value)
	}

	v = filterIncomplete(v, day, level)

	sorter := LeaderboardSorter{
		Members: v,
		Day:     day,
		Level:   level,
	}

	sort.Sort(sorter)

	firstTimestamp := sorter.Members[0].FindCompletion(day, level).Time()

	for idx, member := range sorter.Members {
		timestamp := member.FindCompletion(day, level).Time()
		difference := timestamp.Sub(firstTimestamp)

		points := 100 - idx
		if points < 0 {
			points = 0
		}

		fmt.Printf("%3d: %-20s   %3d   %s (%s)\n", idx+1, member.PrintableName(), points, timestamp.Format("15:04:05"), difference)
	}
}
