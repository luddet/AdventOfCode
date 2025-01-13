use std::{collections::HashSet, ops::Add};

use year2024::input::get_input_as_string;

type Map = Vec<Vec<char>>;

#[derive(Default, Hash, Debug, Eq, PartialEq, Clone)]
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

#[derive(Default, Debug)]
enum Direction {
    #[default]
    UP,
    DOWN,
    LEFT,
    RIGHT,
}

#[derive(Default, Debug)]
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

    if map[new_pos.y as usize][new_pos.x as usize] == '.' {
        guard.pos = new_pos;
    } else {
        guard.dir = new_dir;
    }

    true
}

fn coord_in_bounds(coord: &Vector2, bounds: &Vector2) -> bool {
    coord.x >= 0 && coord.x <= bounds.x && coord.y >= 0 && coord.y <= bounds.y
}

fn main() {
    let mut input = get_input_as_string(2024, 6).unwrap();

    let map: Map = Vec::from_iter(input.lines().map(|line| Vec::from_iter(line.chars())));
    let map_size = Vector2::new(map[0].len() as i32, map.len() as i32);
    let mut guard = Guard::default();
    let mut visited: HashSet<Vector2> = HashSet::new();

    'outer: for (y, line) in map.iter().enumerate() {
        for (x, chr) in line.iter().enumerate() {
            if *chr == '^' {
                guard.pos = Vector2::new(x as i32, y as i32);
                break 'outer;
            }
        }
    }
    input = input.replace('^', ".");

    dbg!(&guard);
    visited.insert(guard.pos.clone());
    while move_guard(&mut guard, &map, &map_size) {
        visited.insert(guard.pos.clone());
    }

    println!("Day06 part 1: {}", visited.len());
}
