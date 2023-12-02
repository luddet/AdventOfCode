package main

import (
	"bufio"
	"fmt"
	"os"
	"regexp"
	"strconv"
)

func main() {
	file, e := os.Open("input.txt")
	if e != nil {
		fmt.Println("Couldn't open input file.")
		os.Exit(1)
	}

	var sum int

	re := regexp.MustCompile(`^[^\d]*(\d).*?(\d)?[^\d]*$`)

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()
		match := re.FindStringSubmatch(line)
		if match == nil {
			fmt.Printf("No match in string: %s\n", line)
			os.Exit(1)
		}

		var numstr string
		if match[2] != "" {
			numstr = match[1] + match[2]
		} else {
			numstr = match[1] + match[1]
		}

		num, err := strconv.Atoi(numstr)
		if err != nil {
			fmt.Printf("Failed to convert %s to an integer", numstr)
			os.Exit(1)
		}

		sum += num

		fmt.Printf("Line: %s, %d\n", line, num)
	}

	fmt.Printf("Day01: %d", sum)
}
