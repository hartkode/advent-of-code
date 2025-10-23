#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

vector<tuple<char, size_t, size_t>>
read_file(const string& filename)
{
	fstream input{ filename };

	vector<tuple<char, size_t, size_t>> instr;

	for ( string line; getline(input, line); ) {
		size_t value_a = 0;
		size_t value_b = 0;

		if ( sscanf(line.c_str(), "rect %zu x %zu", &value_a, &value_b) == 2 ) {
			instr.emplace_back('r', value_a, value_b);
		}
		else if ( sscanf(line.c_str(), "rotate column x=%zu by %zu", &value_a, &value_b) == 2 ) {
			instr.emplace_back('x', value_a, value_b);
		}
		else if ( sscanf(line.c_str(), "rotate row y=%zu by %zu", &value_a, &value_b) == 2 ) {
			instr.emplace_back('y', value_a, value_b);
		}
	}

	return instr;
}

void
solve(const vector<tuple<char, size_t, size_t>>& instr)
{
	static const size_t width  = 50;
	static const size_t height = 6;

	map<tuple<size_t, size_t>, char> disp;

	auto reverse_y = [&](const size_t y, size_t lhs, size_t rhs) {
		while ( lhs < rhs ) {
			swap(disp[{ lhs++, y }], disp[{ --rhs, y }]);
		}
	};

	auto rotate_y = [&](const size_t y, const size_t num) {
		reverse_y(y, 0, width);
		reverse_y(y, 0, num);
		reverse_y(y, num, width);
	};

	auto reverse_x = [&](const size_t x, size_t lhs, size_t rhs) {
		while ( lhs < rhs ) {
			swap(disp[{ x, lhs++ }], disp[{ x, --rhs }]);
		}
	};

	auto rotate_x = [&](const size_t x, const size_t num) {
		reverse_x(x, 0, height);
		reverse_x(x, 0, num);
		reverse_x(x, num, height);
	};

	for ( const auto& [chr, val_a, val_b]: instr ) {
		if ( chr == 'r' ) {
			for ( size_t x = 0; x != val_a; ++x ) {
				for ( size_t y = 0; y != val_b; ++y ) {
					disp[{ x, y }] = '#';
				}
			}
		}
		else if ( chr == 'x' ) {
			rotate_x(val_a, val_b);
		}
		else if ( chr == 'y' ) {
			rotate_y(val_a, val_b);
		}
	}

	// part1
	cout << count_if(disp.begin(), disp.end(), [](auto pos) { return pos.second == '#'; }) << endl;

	// part2
	for ( size_t y = 0; y != height; ++y ) {
		for ( size_t x = 0; x != width; ++x ) {
			cout << ((disp[{ x, y }] == '#') ? '#' : ' ');
		}
		cout << endl;
	}
	cout << endl;
}

int
main()
{
	auto instr = read_file("data/day08.txt");
	solve(instr);
}
