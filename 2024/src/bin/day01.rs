// use std::io;
use std::fs;
use std::error::Error;
use std::path::Path;

fn main() -> Result<(), Box<dyn Error>> {
    let path = Path::new("./input/day01.txt");

    let input = fs::read_to_string(path).expect("day01");

    let mut lists: Vec<Vec<i32>> = vec![Vec::new(), Vec::new()];

    for line in input.lines() {
        let parts:Vec<&str> = line.split_whitespace().take(2).collect();
        lists[0].push(parts[0].parse()?);
        lists[1].push(parts[1].parse()?);
    }

    for list in &mut lists {
        list.sort();
    }

    let sum: i32 = lists[0].iter().zip(&lists[1]).map(|(l, r)| (r - l).abs()).sum();
    println!("Day01 part 1: {sum}");
    Ok(())
}
