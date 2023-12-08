package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

func main() {
	file, err := os.Open("input.txt")
	panicOnError(err)

	maxCubes := map[string]int{"red": 12, "green": 13, "blue": 14}

	var sum, powerSum int
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()
		line, found := strings.CutPrefix(line, "Game ")
		panicOnFalse(found, "Invalid line")

		gameNrStr, line, found := strings.Cut(line, ":")
		panicOnFalse(found, "Invalid line")

		gameNr, err := strconv.Atoi(gameNrStr)
		panicOnError(err)

		minCubes := map[string]int{}

		sets := strings.Split(line, ";")
		for _, set := range sets {
			colors := strings.Split(set, ",")
			for _, color := range colors {
				colorAndNumber := strings.Split(strings.TrimSpace(color), " ")
				colorN, err := strconv.Atoi(colorAndNumber[0])
				colorName := colorAndNumber[1]
				panicOnError(err)
				if colorN > maxCubes[colorName] {
					gameNr = 0
				}

				minCubes[colorName] = max(colorN, minCubes[colorName])
			}
		}
		power := 1
		for _, num := range minCubes {
			power *= num
		}
		powerSum += power
		sum += gameNr
	}
	fmt.Printf("Day 02 Part 1: %d\n", sum)
	fmt.Printf("Day 02 Part 2: %d\n", powerSum)
}

func panicOnError(err error) {
	if err != nil {
		panic(err)
	}
}

func panicOnFalse(exp bool, msg string) {
	if !exp {
		panic(msg)
	}
}
