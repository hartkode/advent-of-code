#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

using pos_t    = tuple<size_t, size_t>;
using puzzle_t = vector<string>;

puzzle_t
read_file(string_view filename)
{
	fstream  input{ filename };
	puzzle_t data;

	for ( string line; getline(input, line); ) {
		data.emplace_back(line);
	}

	return data;
}

pos_t
find_start_pos(const puzzle_t& puzzle)
{
	for ( size_t y = 0; y != puzzle.size(); ++y ) { // NOLINT
		auto x = puzzle[y].find('S');               // NOLINT
		if ( x != string::npos ) {
			return { x, y };
		}
	}

	return { 0, 0 };
};

bool
predict_direction(const puzzle_t& puzzle, pos_t& current, char& direction)
{
	auto x = get<0>(current); // NOLINT
	auto y = get<1>(current); // NOLINT

	if ( direction == 'S' ) {
		++y;
		if ( y >= puzzle.size() ) {
			return false;
		}
		const auto tile = puzzle[y][x];
		if ( tile != 'J' && tile != '|' && tile != 'L' ) {
			return false;
		}
		if ( tile == 'J' ) {
			direction = 'W';
		}
		else if ( tile == 'L' ) {
			direction = 'E';
		}
	}
	else if ( direction == 'N' ) {
		--y;
		if ( y >= puzzle.size() ) {
			return false;
		}
		const auto tile = puzzle[y][x];
		if ( tile != '7' && tile != '|' && tile != 'F' ) {
			return false;
		}
		if ( tile == '7' ) {
			direction = 'W';
		}
		else if ( tile == 'F' ) {
			direction = 'E';
		}
	}
	else if ( direction == 'E' ) {
		++x;
		if ( x >= puzzle[y].size() ) {
			return false;
		}
		const auto tile = puzzle[y][x];
		if ( tile != 'J' && tile != '-' && tile != '7' ) {
			return false;
		}
		if ( tile == 'J' ) {
			direction = 'N';
		}
		else if ( tile == '7' ) {
			direction = 'S';
		}
	}
	else if ( direction == 'W' ) {
		--x;
		if ( x >= puzzle[y].size() ) {
			return false;
		}
		const auto tile = puzzle[y][x];
		if ( tile != 'L' && tile != '-' && tile != 'F' ) {
			return false;
		}
		if ( tile == 'L' ) {
			direction = 'N';
		}
		else if ( tile == 'F' ) {
			direction = 'S';
		}
	}
	else {
		cerr << "invalid direction " << direction << ")!" << endl;
		return false;
	}

	current = { x, y };
	return true;
};

void
part1()
{
	const auto puzzle = read_file("data/day10.txt");

	const auto start_pos = find_start_pos(puzzle);

	auto max_steps = 0;

	for ( const auto direction: { 'N', 'S', 'E', 'W' } ) {
		auto current_direction = direction;
		auto current_pos       = start_pos;

		auto steps = 0;
		for ( ;; ) {
			++steps;

			if ( !predict_direction(puzzle, current_pos, current_direction) ) {
				break;
			}
		}

		max_steps = max(max_steps, steps);
	}

	cout << max_steps / 2 << endl;
}

bool
flood_fill(puzzle_t& puzzle, size_t x, size_t y)
{
	if ( y >= puzzle.size() || x >= puzzle[0].size() ) {
		return false;
	}

	if ( puzzle[y][x] == ' ' ) {
		puzzle[y][x] = 'o';
		if ( !flood_fill(puzzle, x, y + 1) ||
		     !flood_fill(puzzle, x, y - 1) ||
		     !flood_fill(puzzle, x + 1, y) ||
		     !flood_fill(puzzle, x - 1, y) ) {
			return false;
		}
	}

	return true;
};

void
part2()
{
	const auto puzzle_original = read_file("data/day10.txt");

	puzzle_t puzzle{ puzzle_original.size(), string(puzzle_original[0].size(), ' ') };

	const auto start_pos = find_start_pos(puzzle_original);

	for ( const auto direction: { 'N', 'S', 'E', 'W' } ) {
		auto current_direction = direction;
		auto current_pos       = start_pos;

		for ( ;; ) {
			auto x = get<0>(current_pos); // NOLINT
			auto y = get<1>(current_pos); // NOLINT

			puzzle[y][x] = puzzle_original[y][x];

			if ( !predict_direction(puzzle_original, current_pos, current_direction) ) {
				break;
			}
		}
	}

	for ( size_t y = 0; y != puzzle.size(); ++y ) {        // NOLINT
		for ( size_t x = 0; x != puzzle[y].size(); ++x ) { // NOLINT
			auto test_puzzle{ puzzle };

			if ( flood_fill(test_puzzle, x, y) ) {
				puzzle = test_puzzle;
			}
		}
	}

	auto sum = 0;
	for ( auto& line: puzzle ) {
		auto pipes = 0U;
		for ( auto chr: line ) {
			if ( chr == '|' || chr == 'L' || chr == 'J' ) {
				++pipes;
			}
			if ( chr == 'o' && (pipes & 1U) == 1U ) {
				++sum;
			}
		}
	}

	cout << sum << endl;
}

int
main()
{
	part1();
	part2();
}
