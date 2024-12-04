use regex::Regex;
use year2024::input::get_input_as_string;

fn main() {
    let input = get_input_as_string(2024, 3).unwrap();

    println!("Day03 part 1: {}", part1(&input));
    println!("Day03 part 2: {}", part2(&input));
}

fn part1(input: &str) -> i32 {
    let re = Regex::new(r"mul\((\d+),(\d+)\)").unwrap();
    re.captures_iter(&input)
        .map(|caps| {
            let (_, [n0, n1]) = caps.extract();
            n0.parse::<i32>().unwrap() * n1.parse::<i32>().unwrap()
        })
        .sum::<i32>()
}

fn part2(input: &str) -> i32 {
    let re = Regex::new(r"(?P<op>do|don't|mul)\((?:(?P<n0>\d+),(?P<n1>\d+))?\)").unwrap();

    let mut enabled = true;
    re.captures_iter(&input)
        .map(|caps| -> i32 {
            let Some(m) = caps.name("op") else { panic!("poop") };
            match m.as_str() {
                "mul" => {
                    if enabled {
                        caps.name("n0").unwrap().as_str().parse::<i32>().unwrap()
                            * caps.name("n1").unwrap().as_str().parse::<i32>().unwrap()
                    } else {
                        0
                    }
                }
                "do" => {
                    enabled = true;
                    0
                }
                "don't" => {
                    enabled = false;
                    0
                }
                _ => panic!("Should not happen."),
            }
        })
        .sum()
}
