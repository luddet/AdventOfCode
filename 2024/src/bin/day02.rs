use std::path::Path;

fn main() {
    let input_dir = std::env::var("AOC_INPUT").expect("Failed to find env var AOC_INPUT");
    let input_dir = Path::new(&input_dir);
    let input_file = input_dir.join("2024").join("day02.txt");
    let input = std::fs::read_to_string(input_file).expect("Could not read file");

    let input:Vec<Vec<i32>> = input.lines()
    .map(|line| line
        .split_whitespace()
        .map(|n| n.parse::<i32>().unwrap())
        .collect()
    )
    .collect();

    let count = input.iter()
        .filter(|line|
            line.is_sorted_by(|a, b| b > a && b - a <= 3)
            || line.is_sorted_by(|a, b| b < a && a - b <= 3)
        ).count();

    println!("Day02 part 1: {count}");
}
