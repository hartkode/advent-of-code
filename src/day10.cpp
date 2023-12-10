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

int
main()
{
	part1();
}
