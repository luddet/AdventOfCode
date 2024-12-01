// use std::io;
use std::fs;
use std::error::Error;
use std::path::Path;

fn main() -> Result<(), Box<dyn Error>> {
    let path = Path::new("./input/day01.txt");
    let input: String = fs::read_to_string(path)?;
    println!("{}", input);

    Ok(())
}
