package main

import (
	"flag"
	"fmt"
	"os"
	"sort"

	"github.com/thecodinglab/adventofcode-2022/util/pkg/leaderboard"
)

var _ sort.Interface = LeaderboardSorter{}

type LeaderboardSorter struct {
	Members    []leaderboard.Member
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

func filterIncomplete(members []leaderboard.Member, day, level string) (res []leaderboard.Member) {
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

	members, err := leaderboard.Parse(os.Stdin)
	if err != nil {
		panic(err)
	}

	v := make([]leaderboard.Member, 0, len(members))
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
