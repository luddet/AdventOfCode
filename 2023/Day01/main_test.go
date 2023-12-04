package main

import (
	"io"
	"strings"
	"testing"

	"github.com/google/go-cmp/cmp"
)

func TestReadLinesTableDriven(t *testing.T) {

	stringWithEmptyLine := "a\n\nb c\n"

	tt := []struct {
		name        string
		reader      io.Reader
		ignoreEmpty bool
		expected    []string
	}{
		{
			"ignoreEmpty false retains empty line",
			strings.NewReader(stringWithEmptyLine), false,
			[]string{"a", "", "b c"},
		},
		{
			"ignoreEmpty true strips empty line",
			strings.NewReader(stringWithEmptyLine), true,
			[]string{"a", "b c"},
		},
	}

	for _, test := range tt {
		t.Run(test.name, func(t *testing.T) {
			result, err := readLines(test.reader, test.ignoreEmpty)

			if err != nil {
				t.Fatalf("unexpected error: %v", err)
			}

			diff := cmp.Diff(test.expected, result)
			if diff != "" {
				t.Fatal(diff)
			}

		})
	}
}

func TestExtractNumbers(t *testing.T) {
	tests := []struct {
		input    string
		expected int
	}{
		{"two1nine", 29},
		{"eightwothree", 83},
		{"abcone2threexyz", 13},
		{"xtwone3four", 24},
		{"4nineeightseven2", 42},
		{"zoneight234", 14},
		{"7pqrstsixteen", 76},
		{"3twone", 31},
		{"twone", 21},
	}

	for _, test := range tests {
		t.Run(test.input, func(t *testing.T) {
			result, err := extractNumbers(test.input)
			if err != nil {
				t.Fatal(err)
			}

			if result != test.expected {
				t.Fatalf("expected: %v, got %v", test.expected, result)
			}
		})
	}
}

func TestPart2(t *testing.T) {
	input := []string{
		"two1nine",
		"eightwothree",
		"abcone2threexyz",
		"xtwone3four",
		"4nineeightseven2",
		"zoneight234",
		"7pqrstsixteen",
	}
	var expected string = "281"

	result, err := part2(input)

	if err != nil {
		t.Fatalf("error: %v", err)
	}
	if result != expected {
		t.Fatalf("Expected %v but got %v", expected, result)
	}
}
