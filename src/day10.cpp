#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

vector<string>
read_file(string_view filename)
{
	fstream        input{ filename };
	vector<string> data;

	for ( string line; getline(input, line); ) {
		data.emplace_back(line);
	}

	return data;
}

void
part1()
{
	typedef tuple<size_t, size_t> pos_t;

	const auto puzzle = read_file("data/day10.txt");

	const auto find_start_pos = [&]() -> pos_t {
		for ( size_t y = 0; y != puzzle.size(); ++y ) { // NOLINT
			auto x = puzzle[y].find('S');               // NOLINT
			if ( x != string::npos ) {
				return { x, y };
			}
		}

		return { 0, 0 };
	};

	const auto predict_direction = [&](pos_t& current, char& direction) -> bool {
		auto x = get<0>(current); // NOLINT
		auto y = get<1>(current); // NOLINT

		if ( direction == 'S' ) {
			++y;
			if ( y >= puzzle.size() ) {
				return false;
			}
			auto tile = puzzle[y][x];
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
			auto tile = puzzle[y][x];
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
			auto tile = puzzle[y][x];
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
			auto tile = puzzle[y][x];
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

	const auto start_pos = find_start_pos();

	auto max_steps = 0;

	for ( const auto direction: { 'N', 'S', 'E', 'W' } ) {
		auto current_direction = direction;
		auto current_pos       = start_pos;

		auto steps = 0;
		for ( ;; ) {
			++steps;

			if ( !predict_direction(current_pos, current_direction) ) {
				break;
			}
		}
		cout << "Richtung: " << direction << " - Schritte: " << steps << endl;

		max_steps = max(max_steps, steps);
	}

	cout << max_steps / 2 << endl;
}

void
part2()
{
	typedef tuple<size_t, size_t> pos_t;

	const auto puzzle = read_file("data/day10.txt");

	const auto find_start_pos = [&]() -> pos_t {
		for ( size_t y = 0; y != puzzle.size(); ++y ) { // NOLINT
			auto x = puzzle[y].find('S');               // NOLINT
			if ( x != string::npos ) {
				return { x, y };
			}
		}

		return { 0, 0 };
	};

	const auto predict_direction = [&](pos_t& current, char& direction) -> bool {
		auto x = get<0>(current); // NOLINT
		auto y = get<1>(current); // NOLINT

		if ( direction == 'S' ) {
			++y;
			if ( y >= puzzle.size() ) {
				return false;
			}
			auto tile = puzzle[y][x];
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
			auto tile = puzzle[y][x];
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
			auto tile = puzzle[y][x];
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
			auto tile = puzzle[y][x];
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

	vector<string> puzzle_copy(puzzle.size(), string(puzzle[0].size(), ' '));

	const auto start_pos = find_start_pos();

	for ( const auto direction: { 'N', 'S', 'E', 'W' } ) {
		auto current_direction = direction;
		auto current_pos       = start_pos;

		for ( ;; ) {
			auto x = get<0>(current_pos); // NOLINT
			auto y = get<1>(current_pos); // NOLINT

			puzzle_copy[y][x] = puzzle[y][x];

			if ( !predict_direction(current_pos, current_direction) ) {
				break;
			}
		}
	}

	function<bool(vector<string>&, pos_t)> flood_fill = [&](vector<string>& puzzle, pos_t position) -> bool {
		auto x = get<0>(position); // NOLINT
		auto y = get<1>(position); // NOLINT

		if ( y >= puzzle.size() || x >= puzzle[0].size() ) {
			return false;
		}

		if ( puzzle[y][x] == ' ' ) {
			puzzle[y][x] = 'o';
			if ( !flood_fill(puzzle, { x, y + 1 }) ||
			     !flood_fill(puzzle, { x, y - 1 }) ||
			     !flood_fill(puzzle, { x + 1, y }) ||
			     !flood_fill(puzzle, { x - 1, y }) ) {
				return false;
			}
		}

		return true;
	};

	for ( size_t y = 0; y != puzzle_copy.size(); ++y ) {
		for ( size_t x = 0; x != puzzle_copy[y].size(); ++x ) {
			auto test_puzzle = puzzle_copy;

			if ( flood_fill(test_puzzle, { x, y }) ) {
				puzzle_copy = test_puzzle;
			}
		}
	}

	int sum = 0;
	for ( auto& line: puzzle_copy ) {
		auto pipes = 0U;
		for ( char chr: line ) {
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
