package leaderboard

import (
	"encoding/json"
	"fmt"
	"io"
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

func Parse(reader io.Reader) (map[string]Member, error) {
	val := struct {
		Members map[string]Member
	}{}

	err := json.NewDecoder(reader).Decode(&val)
	return val.Members, err
}
