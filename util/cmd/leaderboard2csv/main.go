package main

import (
	"encoding/csv"
	"fmt"
	"os"

	"github.com/thecodinglab/adventofcode-2022/util/pkg/leaderboard"
)

func main() {
	members, err := leaderboard.Parse(os.Stdin)
	if err != nil {
		panic(err)
	}

	writer := csv.NewWriter(os.Stdout)
	defer writer.Flush()

	header := []string{"id", "name"}

	for day := 0; day < 25; day++ {
		for level := 0; level < 2; level++ {
			key := fmt.Sprintf("completion_%d_%d", day+1, level+1)
			header = append(header, fmt.Sprint(key, "_index"), fmt.Sprint(key, "_time"))
		}
	}

	writer.Write(header)

	for _, value := range members {
		record := []string{
			fmt.Sprint(value.ID),
			value.PrintableName(),
		}

		for day := 0; day < 25; day++ {
			for level := 0; level < 2; level++ {
				completion := value.CompletionDayLevel[fmt.Sprint(day+1)][fmt.Sprint(level+1)]
				if completion.Timestamp == 0 {
					record = append(record, "", "")
				} else {
					record = append(record, fmt.Sprint(completion.Index), fmt.Sprint(completion.Timestamp))
				}
			}
		}

		writer.Write(record)
	}
}
