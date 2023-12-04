package main

import (
	"bufio"
	"fmt"
	"io"
	"math"
	"os"
	"regexp"
	"strconv"
	"strings"
)

func main() {
	file, err := os.Open("input.txt")
	panicOnError(err)

	lines, err := readLines(file, true)
	panicOnError(err)

	sum1, err := part1(lines)
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	sum2, err := part2(lines)
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	fmt.Printf("Day01 Part 1: %s\n", sum1)
	fmt.Printf("Day01 Part 2: %s\n", sum2)
}

func part1(input []string) (string, error) {
	var sum int

	re := regexp.MustCompile(`^[^\d]*(\d).*?(\d)?[^\d]*$`)

	for _, line := range input {
		if line == "" {
			continue
		}

		match := re.FindStringSubmatch(line)
		if match == nil {
			return "", fmt.Errorf("no match in string: %s", line)
		}

		var numstr string
		if match[2] != "" {
			numstr = match[1] + match[2]
		} else {
			numstr = match[1] + match[1]
		}

		num, err := strconv.Atoi(numstr)
		if err != nil {
			return "", fmt.Errorf("failed to convert %s to an integer", numstr)
		}

		sum += num
	}

	return fmt.Sprint(sum), nil
}

func part2(lines []string) (string, error) {

	var sum int
	for _, line := range lines {
		if line == "" {
			continue
		}

		num, err := extractNumbers(line)
		if err != nil {
			return "", err
		}

		sum += num
	}
	return fmt.Sprint(sum), nil
}

func extractNumbers(str string) (int, error) {
	numStrings := []string{
		"one", "two", "three", "four", "five",
		"six", "seven", "eight", "nine",
		"1", "2", "3", "4", "5", "6", "7", "8", "9",
	}

	numberMap := map[string]string{
		"one": "1", "1": "1",
		"two": "2", "2": "2",
		"three": "3", "3": "3",
		"four": "4", "4": "4",
		"five": "5", "5": "5",
		"six": "6", "6": "6",
		"seven": "7", "7": "7",
		"eight": "8", "8": "8",
		"nine": "9", "9": "9",
	}

	var first, last string
	var firstIdx int = math.MaxInt
	var lastIdx int = -1
	for _, numStr := range numStrings {
		if i := strings.Index(str, numStr); i > -1 && i < firstIdx {
			firstIdx = i
			first = numStr
		}

		if i := strings.LastIndex(str, numStr); i > lastIdx {
			lastIdx = i
			last = numStr
		}
	}

	numstr := numberMap[first] + numberMap[last]
	num, err := strconv.Atoi(numstr)
	if err != nil {
		return 0, fmt.Errorf("failed to convert %s to an integer", numstr)
	}

	return num, nil
}

func panicOnError(err error) {
	if err == nil {
		return
	}

	panic(err)
}

func readLines(reader io.Reader, ignoreEmptyLines bool) ([]string, error) {
	s := bufio.NewScanner(reader)
	lines := []string{}
	for s.Scan() {
		if ignoreEmptyLines && s.Text() == "" {
			continue
		}
		lines = append(lines, s.Text())
	}
	return lines, nil
}
