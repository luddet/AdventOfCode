// use std::io;
use std::fs;
use std::error::Error;
use std::path::Path;

fn main() -> Result<(), Box<dyn Error>> {
    let path = Path::new("./input/day01.txt");

    let input = fs::read_to_string(path).expect("day01");

    let mut left: Vec<i32> = Vec::new();
    let mut right: Vec<i32> = Vec::new();

    for line in input.lines() {
        let parts:Vec<&str> = line.split_whitespace().take(2).collect();
        left.push(parts[0].parse()?);
        right.push(parts[1].parse()?);
    }

    left.sort();
    right.sort();

    println!("Day01 part 1: {}", part1(&left, &right));
    println!("Day01 part 2: {}", part2(&left, &right));

    Ok(())
}

fn part1(l: &Vec<i32>, r: &Vec<i32>) -> i32 {
    l.iter().zip(r).map(|(l, r)| (r - l).abs()).sum()
}

fn part2(l: &Vec<i32>, r: &Vec<i32>) -> i32 {
    let mut sum: usize = 0;
    for n in l {
        sum += *n as usize * r.into_iter().filter(|r_n| n == *r_n).count();
    }
    assert!(sum <= i32::MAX as usize);
    sum as i32
}   