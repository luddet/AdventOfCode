use super::*;
use const_format;

const TEST_RULES: &str = r"47|53
97|13
97|61
97|47
75|29
61|13
75|53
29|13
97|29
53|29
61|53
97|53
61|29
47|13
75|47
97|75
47|61
75|61
47|29
75|13
53|13";

const TEST_UPDATES: &str = r"75,47,61,53,29
97,61,53,29,13
75,29,13
75,97,47,61,53
61,13,29
97,13,75,29,47";

const TEST_INPUT: &str = const_format::concatcp!(TEST_RULES, "\n\n", TEST_UPDATES);

#[test]
fn test_parse_rules() {
    let rules = parse_rules(&TEST_RULES);

    assert_eq!(rules.len(), 100);
    assert!(rules[13].contains(&75));
    assert!(rules[13].contains(&29));
    assert!(rules[29].len() == 5);
}

#[test]
fn test_parse_updates() {
    let updates = parse_updates(&TEST_UPDATES);

    assert_eq!(updates.len(), 6);
    assert_eq!(updates[2], vec![75, 29, 13]);
}

#[test]
fn test_parse_input() {
    let full_input = TEST_RULES.to_owned() + "\n\n" + TEST_UPDATES;
    let (result_rules, result_updates) = parse_input(&full_input);

    let rules = parse_rules(&TEST_RULES);
    let updates = parse_updates(&TEST_UPDATES);

    assert_eq!(result_rules, rules);
    assert_eq!(result_updates, updates);
}

#[test]
fn test_validate_update() {
    let (rules, updates) = parse_input(TEST_INPUT);

    let validation_result:Vec<bool> = updates.iter().map(|update| validate_order(update, &rules)).collect();

    assert_eq!(validation_result, vec![true, true, true, false, false, false]);
}