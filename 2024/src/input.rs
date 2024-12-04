use std::env; //::{self, VarError};
use std::error::Error;
use std::fs::read_to_string;
use std::result::Result;
use std::path::Path;

const ENV_VAR:&str = "AOC_INPUT";

pub fn get_input_as_string(year: u32, day: u32) -> Result<String, Box<dyn Error>> {
    let input_dir = env::var(ENV_VAR)?;
    let input_file = Path::new(&input_dir)
        .join(year.to_string())
        .join(format!("day{day:02}.txt"));

    Ok(read_to_string(input_file)?)
}
