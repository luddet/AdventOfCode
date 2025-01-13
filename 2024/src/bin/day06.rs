use std::{collections::HashSet, ops::Add};
use year2024::input::get_input_as_string;

type Map = Vec<Vec<char>>;

#[derive(Default, Hash, Debug, Eq, PartialEq, Clone, Copy)]
struct Vector2 {
    x: i32,
    y: i32,
}

impl Vector2 {
    fn new(x: i32, y: i32) -> Self {
        Self { x: x, y: y }
    }
}

impl Add for Vector2 {
    type Output = Vector2;

    fn add(self: Vector2, rhs: Vector2) -> Self::Output {
        Self::Output {
            x: self.x + rhs.x,
            y: self.y + rhs.y,
        }
    }
}

impl<'a> Add for &'a Vector2 {
    type Output = Vector2;

    fn add(self: &'a Vector2, rhs: &'a Vector2) -> Self::Output {
        Self::Output {
            x: self.x + rhs.x,
            y: self.y + rhs.y,
        }
    }
}

#[derive(Default, Debug, Hash, Clone, Eq, PartialEq)]
enum Direction {
    #[default]
    UP,
    DOWN,
    LEFT,
    RIGHT,
}

#[derive(Default, Debug, Hash, Clone, Eq, PartialEq)]
struct Guard {
    pos: Vector2,
    dir: Direction,
}

fn move_guard(guard: &mut Guard, map: &Map, map_size: &Vector2) -> bool {
    let (delta, new_dir) = match guard.dir {
        Direction::UP => (Vector2::new(0, -1), Direction::RIGHT),
        Direction::DOWN => (Vector2::new(0, 1), Direction::LEFT),
        Direction::LEFT => (Vector2::new(-1, 0), Direction::UP),
        Direction::RIGHT => (Vector2::new(1, 0), Direction::DOWN),
    };

    // Check if guard left map
    let new_pos = &guard.pos + &delta;
    if !coord_in_bounds(&new_pos, &map_size) {
        return false;
    }

    match map[new_pos.y as usize][new_pos.x as usize] {
        '.' | '^' => guard.pos = new_pos,
        _ => guard.dir = new_dir,
    }

    true
}

fn coord_in_bounds(coord: &Vector2, bounds: &Vector2) -> bool {
    coord.x >= 0 && coord.x <= bounds.x - 1 && coord.y >= 0 && coord.y <= bounds.y - 1
}

fn main() {
    let input = get_input_as_string(2024, 6).unwrap();

    let mut map: Map = Vec::from_iter(input.lines().map(|line| Vec::from_iter(line.chars())));
    let map_size = Vector2::new(map[0].len() as i32, map.len() as i32);
    let mut guard = Guard::default();
    let mut part1_visited: HashSet<Vector2> = HashSet::new();

    'outer: for (y, line) in map.iter().enumerate() {
        for (x, chr) in line.iter().enumerate() {
            if *chr == '^' {
                guard.pos = Vector2::new(x as i32, y as i32);
                break 'outer;
            }
        }
    }

    let start_pos = guard.pos;

    // Part 1
    while move_guard(&mut guard, &map, &map_size) {
        part1_visited.insert(guard.pos);
    }
    let part1_num_pos = 1 /* start pos */ + part1_visited.len();

    // Part 2
    let mut part2_visited: HashSet<Guard> = HashSet::new();
    let mut loop_count = 0;
    for state in part1_visited.iter() {
        map[state.y as usize][state.x as usize] = '#';

        guard.dir = Direction::UP;
        guard.pos = start_pos;

        // // Check for loop
        part2_visited.clear();
        while move_guard(&mut guard, &map, &map_size) {
            if !part2_visited.insert(guard.clone()) {
                loop_count += 1;
                break;
            }
        }

        map[state.y as usize][state.x as usize] = '.';
    }

    println!("Day06 part 1: {}", part1_num_pos);
    println!("Day06 part 2: {loop_count}");
}
