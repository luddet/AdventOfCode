use std::{error::Error, path::Path};

fn main() -> Result<(), Box<dyn Error>> {
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

    let part1 = input.iter()
        .filter(|line| is_report_safe_1(*line))
        .count();

    let part2 = input.iter()
        .filter(|line| is_report_safe_2(*line))
        .count();
    
    println!("Day02 part 1: {part1}");
    println!("Day02 part 2: {part2}");

    Ok(())
}

fn compare_inc(a:&i32, b:&i32) -> bool {
    a < b && b - a <= 3
}

fn compare_dec(a:&i32, b:&i32) -> bool {
    a > b && a - b <= 3
}

fn is_report_safe_1(report: &Vec<i32>) -> bool {
    report.iter().is_sorted_by(|a,b| compare_inc(a, b)) ||
    report.iter().is_sorted_by(|a,b| compare_dec(a, b))
}

fn is_report_safe_2(report: &Vec<i32>) -> bool {
    if is_report_safe_1(&report) {
        return true;
    }

    for i in 0..report.len() {
        let excl_iter = report.iter().take(i).chain(report.iter().skip(i+1));
        if excl_iter.to_owned().is_sorted_by(|a,b| compare_inc(a, b))
            || excl_iter.is_sorted_by(|a,b| compare_dec(a, b))
        {
            return true;
        }
    }

    false
}