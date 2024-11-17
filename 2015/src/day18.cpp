#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <tuple>
using namespace std;

using puzzle_type = map<tuple<size_t, size_t>, bool>;

auto
read_file(string_view filename)
{
	fstream     input{ filename };
	puzzle_type puzzle;

	size_t line = 0;
	for ( string str; getline(input, str); ) {
		for ( size_t column = 0; column != str.size(); ++column ) {
			if ( str[column] == '#' ) {
				puzzle[{ column, line }] = true;
			}
		}

		++line;
	}

	return puzzle;
}

void
iterate(puzzle_type& puzzle, size_t width)
{
	puzzle_type next;

	for ( size_t line = 0; line != width; ++line ) {
		for ( size_t column = 0; column != width; ++column ) {
			static const vector<tuple<size_t, size_t>> offsets = {
				{ -1, -1 },
				{ 0, -1 },
				{ 1, -1 },
				{ -1, 0 },
				{ 1, 0 },
				{ -1, 1 },
				{ 0, 1 },
				{ 1, 1 }
			};

			size_t count = 0;
			for ( const auto& [delta_x, delta_y]: offsets ) {
				if ( puzzle[{ column + delta_x, line + delta_y }] ) {
					++count;
				}
			}

			if ( puzzle[{ column, line }] ) {
				if ( count == 2 || count == 3 ) {
					next[{ column, line }] = true;
				}
			}
			else {
				if ( count == 3 ) {
					next[{ column, line }] = true;
				}
			}
		}
	}

	puzzle.swap(next);
}

void
print(puzzle_type puzzle, size_t width)
{
	for ( size_t line = 0; line != width; ++line ) {
		for ( size_t column = 0; column != width; ++column ) {
			auto value = puzzle[{ column, line }];
			cout << (value ? '#' : '.');
		}
		cout << '\n';
	}
	cout << endl;
}

void
part1(puzzle_type puzzle)
{
	static const size_t width = 100;

	for ( int i = 0; i != 100; ++i ) {
		iterate(puzzle, width);
	}

	long sum = 0;
	for ( size_t line = 0; line != width; ++line ) {
		for ( size_t column = 0; column != width; ++column ) {
			auto value = puzzle[{ column, line }];
			if ( value ) {
				++sum;
			}
		}
	}
	cout << sum << endl;
}

void
enable_corners(puzzle_type& puzzle, size_t width)
{
	puzzle[{ 0, 0 }]                 = true;
	puzzle[{ width - 1, 0 }]         = true;
	puzzle[{ 0, width - 1 }]         = true;
	puzzle[{ width - 1, width - 1 }] = true;
}

void
part2(puzzle_type puzzle)
{
	static const size_t width = 100;

	for ( int i = 0; i != 100; ++i ) {
		enable_corners(puzzle, width);
		iterate(puzzle, width);
	}
	enable_corners(puzzle, width);

	long sum = 0;
	for ( size_t line = 0; line != width; ++line ) {
		for ( size_t column = 0; column != width; ++column ) {
			auto value = puzzle[{ column, line }];
			if ( value ) {
				++sum;
			}
		}
	}
	cout << sum << endl;
}

int
main()
{
	auto puzzle = read_file("data/day18.txt");
	part1(puzzle);
	part2(puzzle);
}
