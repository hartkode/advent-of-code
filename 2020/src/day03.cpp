#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>
using namespace std;

auto
read_file(string_view filename)
{
	fstream        input{ filename };
	vector<string> data;

	for ( string line; getline(input, line); ) {
		data.emplace_back(line);
	}

	return data;
}

auto
solve(const vector<string>& puzzle, size_t delta_col, size_t delta_row)
{
	size_t col = 0;
	size_t row = 0;

	auto sum = 0L;
	while ( row < puzzle.size() ) {
		if ( puzzle[row][col] == '#' ) {
			++sum;
		}

		row += delta_row;

		col += delta_col;
		col %= puzzle[0].size();
	}
	return sum;
}

void
part1(const vector<string>& puzzle)
{
	cout << solve(puzzle, 3, 1) << endl;
}

void
part2(const vector<string>& puzzle)
{
	cout << solve(puzzle, 1, 1) * solve(puzzle, 3, 1) * solve(puzzle, 5, 1) * solve(puzzle, 7, 1) * solve(puzzle, 1, 2) << endl;
}

int
main()
{
	const auto puzzle = read_file("data/day03.txt");
	part1(puzzle);
	part2(puzzle);
}

