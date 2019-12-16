#include "pch.h"
#include <sstream>
#include <algorithm>
#include <tuple>
#include "..\Day10\Day10.h"
#include "..\Day10\CoordinateGenerator.h"

std::pair<int, int> input1_best(3, 4);
int input1_numberVisible = 8;
const char* input1 = 
".#..#\n"
".....\n"
"#####\n"
"....#\n"
"...##";

std::pair<int, int> input2_best(5, 8);
int input2_numberVisible = 33;
const char* input2 =
"......#.#.\n"
"#..#.#....\n"
"..#######.\n"
".#.#.###..\n"
".#..#.....\n"
"..#....#.#\n"
"#..#....#.\n"
".##.#..###\n"
"##...#..#.\n"
".#....####";

std::pair<int, int> input3_best(1,2);
int input3_numberVisible = 35;
const char* input3 =
"#.#...#.#.\n"
".###....#.\n"
".#....#...\n"
"##.#.#.#.#\n"
"....#.#.#.\n"
".##..###.#\n"
"..#...##..\n"
"..##....##\n"
"......#...\n"
".####.###.";

std::pair<int, int> input4_best(6,3);
int input4_numberVisible = 41;
const char* input4 =
".#..#..###\n"
"####.###.#\n"
"....###.#.\n"
"..###.##.#\n"
"##.##.#.#.\n"
"....###..#\n"
"..#.#..#.#\n"
"#..#.#.###\n"
".##...##.#\n"
".....#.#..";

std::pair<int, int> input5_best(11, 13);
int input5_numberVisible = 210;
const char* input5 =
".#..##.###...#######\n"
"##.############..##.\n"
".#.######.########.#\n"
".###.#######.####.#.\n"
"#####.##.#.##.###.##\n"
"..#####..#.#########\n"
"####################\n"
"#.####....###.#.#.##\n"
"##.#################\n"
"#####.##.###..####..\n"
"..######..##.#######\n"
"####.##.####...##..#\n"
".#####..#.######.###\n"
"##...#.##########...\n"
"#.##########.#######\n"
".####.#.###.###.#.##\n"
"....##.##.###..#####\n"
".#.#.###########.###\n"
"#.#.#.#####.####.###\n"
"###.##.####.##.#..##";


TEST(readAsteroidMapTest, HasCorrectAsteroids)
{

	std::istringstream ss(input1);
	std::set<Day10::point_t> asteroids = Day10::readAsteroidMap(ss);

	EXPECT_TRUE(asteroids.find({ 1, 0}) != asteroids.end());
	EXPECT_TRUE(asteroids.find({ 4, 0}) != asteroids.end());
	EXPECT_TRUE(asteroids.find({ 0, 2 }) != asteroids.end());
	EXPECT_TRUE(asteroids.find({ 1, 2 }) != asteroids.end());
	EXPECT_TRUE(asteroids.find({ 2, 2 }) != asteroids.end());
	EXPECT_TRUE(asteroids.find({ 3, 2 }) != asteroids.end());
	EXPECT_TRUE(asteroids.find({ 4, 2 }) != asteroids.end());
	EXPECT_TRUE(asteroids.find({ 4, 3 }) != asteroids.end());
	EXPECT_TRUE(asteroids.find({ 3, 4 }) != asteroids.end());
	EXPECT_TRUE(asteroids.find({ 4, 4 }) != asteroids.end());

}

class ReduceByLCDFixture : public ::testing::TestWithParam<std::tuple<Day10::point_t, Day10::point_t>>
{};

INSTANTIATE_TEST_CASE_P(reduceByLCD, ReduceByLCDFixture,
						::testing::Values(std::make_tuple(Day10::point_t{ 1,1 }, Day10::point_t{ 1,1 }),
										  std::make_tuple(Day10::point_t{ -1,1 }, Day10::point_t{ -1,1 }),
										  std::make_tuple(Day10::point_t{ 1,-1 }, Day10::point_t{ 1,-1 }),
										  std::make_tuple(Day10::point_t{ 2,2 }, Day10::point_t{ 1,1 }),
										  std::make_tuple(Day10::point_t{ 2,4 }, Day10::point_t{ 1,2 }),
										  std::make_tuple(Day10::point_t{ 9,6 }, Day10::point_t{ 3,2 }),
										  std::make_tuple(Day10::point_t{ -18,6 }, Day10::point_t{ -3,1})
						));

TEST_P(ReduceByLCDFixture, AllOfThem)
{
	auto expected = std::get<1>(GetParam());
	auto input = std::get<0>(GetParam());

	auto result = Day10::reduceByLCD(input);

	ASSERT_EQ(result, expected);
}

TEST(CoordinateGenerator, getNext_ZeroOrigin)
{
	Day10::CoordinateGenerator gen({ 0,0 });

	Day10::point_t p = gen.getNext();
	ASSERT_EQ(p, Day10::point_t( 1, 0 ));

	p = gen.getNext();
	ASSERT_EQ(p, Day10::point_t(1, 1));

	p = gen.getNext();
	ASSERT_EQ(p, Day10::point_t(0, 1));

	p = gen.getNext();
	ASSERT_EQ(p, Day10::point_t(-1, 1));

	p = gen.getNext();
	ASSERT_EQ(p, Day10::point_t(-1, 0));

	p = gen.getNext();
	ASSERT_EQ(p, Day10::point_t(-1, -1));

	p = gen.getNext();
	ASSERT_EQ(p, Day10::point_t(0, -1));

	p = gen.getNext();
	ASSERT_EQ(p, Day10::point_t(1, -1));

	p = gen.getNext();
	ASSERT_EQ(p, Day10::point_t(2, -1));

	p = gen.getNext();
	ASSERT_EQ(p, Day10::point_t(2, 0));

	p = gen.getNext();
	ASSERT_EQ(p, Day10::point_t(2, 1));

	p = gen.getNext();
	ASSERT_EQ(p, Day10::point_t(2, 2));

	p = gen.getNext();
	ASSERT_EQ(p, Day10::point_t(1, 2));
}

TEST(CoordinateGenerator, getNext_NonZeroOrigin)
{
	Day10::CoordinateGenerator gen({ 2, -3 });

	Day10::point_t p = gen.getNext();
	ASSERT_EQ(p, Day10::point_t(3, -3));

	p = gen.getNext();
	ASSERT_EQ(p, Day10::point_t(3, -2));

	p = gen.getNext();
	ASSERT_EQ(p, Day10::point_t(2, -2));
}

TEST(manhattanDistance, SamePoint)
{
	int result = Day10::manhattanDistance(Day10::point_t{ 1,1 }, Day10::point_t{ 1,1 });
	ASSERT_EQ(0, result);
}


TEST(manhattanDistance, DifferentPoints)
{
	int result = Day10::manhattanDistance(Day10::point_t{ 4,5 }, Day10::point_t{ 2,4 });
	ASSERT_EQ(3, result);

	result = Day10::manhattanDistance(Day10::point_t{ -4,-5 }, Day10::point_t{ 2,4 });
	ASSERT_EQ(15, result);
}

TEST(reduceToVisible, testInput1)
{
	std::istringstream ss(input1);
	std::set<Day10::point_t> asteroids = Day10::readAsteroidMap(ss);
	auto result = Day10::reduceToVisible(asteroids, { 3,4 });
	ASSERT_EQ(result.size(), input1_numberVisible);
}

TEST(reduceToVisible, testInput5)
{
	std::istringstream ss(input5);
	std::set<Day10::point_t> asteroids = Day10::readAsteroidMap(ss);
	auto result = Day10::reduceToVisible(asteroids, input5_best);
	ASSERT_EQ(result.size(), input5_numberVisible);
}