use regex::Regex;
use year2024::input::get_input_as_string;

fn main() {
    let input = get_input_as_string(2024, 3).unwrap();
    let re = Regex::new(r"mul\((\d+),(\d+)\)").unwrap();
    let sum:i32 = re
        .captures_iter(&input)
        .map(|caps| {
            let (_, [n0, n1]) = caps.extract();
            n0.parse::<i32>().unwrap() * n1.parse::<i32>().unwrap()
        })
        .sum();

    println!("Day03 part 1: {sum}")
}
