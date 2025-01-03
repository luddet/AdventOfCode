use std::collections::HashSet;

use year2024::input::get_input_as_string;

#[cfg(test)]
#[path = "./tests/day05_test.rs"]
mod day05_test;

type Dependencies = HashSet<u8>;
type Rules = Vec<Dependencies>;
type Update = Vec<u8>;
type Updates = Vec<Update>;

fn main() {
    let input = get_input_as_string(2024, 5).unwrap();
    let (rules, updates) = parse_input(&input);

    let part1: i32 = updates
        .iter()
        .filter(|update| validate_order(&update, &rules))
        .map(|update| update[(update.len() - 1) / 2] as i32)
        .sum();
    println!("Day05 part 1: {}", part1);
}

fn validate_order(update: &Update, rules: &Rules) -> bool {
    let mut it = update.iter();
    while let Some(i) = it.next() {
        if it.clone().any(|d| rules[*i as usize].contains(&d)) {
            return false;
        }
    }
    true
}

fn parse_input(input: &str) -> (Rules, Updates) {
    let parts = input.split_once("\n\n").unwrap();
    (parse_rules(parts.0), parse_updates(parts.1))
}

fn parse_rules(input: &str) -> Rules {
    let mut rules = (0..100).map(|_| HashSet::new()).collect::<Rules>();

    input.lines().for_each(|line| {
        let (l, r) = line.split_once("|").unwrap();
        let dep = l.parse::<u8>().unwrap();
        let index = r.parse::<usize>().unwrap();
        rules[index].insert(dep);
    });

    rules
}

fn parse_updates(input: &str) -> Updates {
    input
        .lines()
        .map(|line| {
            line.split(",")
                // .inspect(|token| println!("{:?}", token))
                .map(|token| token.parse::<u8>().unwrap())
                .collect()
        })
        .collect()
}
