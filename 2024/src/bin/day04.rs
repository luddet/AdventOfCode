use year2024::input::get_input_as_string;

fn main() {
    let input = get_input_as_string(2024, 4).unwrap();
    let lines: Vec<Vec<char>> = input.lines().map(|line| line.chars().collect()).collect();

    println!("Day04 part 1: {}", part1(&lines));
}

fn part1(data: &Vec<Vec<char>>) -> u32 {
    let mut sum: u32 = 0;
    for y in 0..data.len() {
        for x in 0..data[y].len() {
            sum += check_position1(&data, x as i32, y as i32);
        }
    }
    sum
}

fn check_position1(data: &Vec<Vec<char>>, x: i32, y: i32) -> u32 {
    let mut xmas_count: u32 = 0;
    
    let steps = vec![
        (-1i32, 0i32),  // W
        (-1, -1), // NW
        (0, -1),  // N
        (1, -1),  // NE
        (1, 0),   // E
        (1, 1),   // SE
        (0, 1),   // S
        (-1, 1),  // SW
    ];

    let xmas = vec!['X', 'M', 'A', 'S'];
    if data[y as usize][x as usize] != 'X' {
        return 0;
    }

    for dir in steps {
        let mut cur_x = x;
        let mut cur_y = y;
        let mut xmas_index = 0;
        for i in 1..xmas.len() {
            cur_x += dir.0;
            cur_y += dir.1;
            if cur_x < 0 || cur_y < 0
            || cur_x >= data[0].len() as i32 || cur_y >= data.len() as i32
            || data[cur_y as usize][cur_x as usize] != xmas[i] {
                break;
            }
            xmas_index = i;
        }
        if xmas_index == xmas.len()-1 {
            xmas_count += 1;
        }
    }
    xmas_count
}