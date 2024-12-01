package main

import (
	"bufio"
	"os"
	"regexp"
	"strconv"

	utilities "github.com/luddet/AdventOfCode/main/2023/Utilities"
)

type Coord struct {
	x int
	y int
}

type Number struct {
	value  int
	length int
	coord  Coord
}

type Symbol struct {
	value byte
	coord Coord
}

func main() {
	file, err := os.Open("input.txt")
	utilities.PanicOnError(err)

	numPattern := regexp.MustCompile(`\d+`)
	symbolPattern := regexp.MustCompile(`[^0-9.]`)

	numbers := []Number{}
	symbols := []Symbol{}

	scanner := bufio.NewScanner(file)
	for i := 0; scanner.Scan(); i++ {
		line := scanner.Text()

		matches := numPattern.FindAllStringIndex(line, -1)
		for _, match := range matches {
			num, err := strconv.Atoi(line[match[0]:match[1]])
			utilities.PanicOnError(err)
			numbers = append(numbers, Number{num, match[1] - match[0], Coord{match[0], i}})
		}
		matches = symbolPattern.FindAllStringIndex(line, -1)
		for _, match := range matches {
			symbols = append(symbols, Symbol{line[match[0]], Coord{match[0], i}})
		}
	}

	var sum int
	for _, sym := range symbols {

	}

}
