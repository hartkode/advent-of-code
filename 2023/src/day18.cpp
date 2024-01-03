#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

using puzzle_t = vector<pair<long, long>>;

puzzle_t
read_file_part1(string_view filename, long& border_steps)
{
	static map<char, pair<int, int>> movement = {
		{ 'U', { 0, -1 } }, // up
		{ 'D', { 0, 1 } },  // down
		{ 'L', { -1, 0 } }, // left
		{ 'R', { 1, 0 } },  // right
	};

	border_steps = 0;

	fstream  input{ filename };
	puzzle_t data;

	data.emplace_back(0, 0);

	for ( string line; getline(input, line); ) {
		char         dir{};
		long         steps{};
		stringstream sstrm{ line };

		if ( sstrm >> dir >> steps ) {
			data.emplace_back(get<0>(data.back()) + get<0>(movement[dir]) * steps,
			                  get<1>(data.back()) + get<1>(movement[dir]) * steps);

			border_steps += steps;
		}
	}

	return data;
}

puzzle_t
read_file_part2(string_view filename, long& border_steps)
{
	static map<char, pair<int, int>> movement = {
		{ '0', { 1, 0 } },  // right
		{ '1', { 0, 1 } },  // down
		{ '2', { -1, 0 } }, // left
		{ '3', { 0, -1 } }, // up
	};

	border_steps = 0;

	fstream  input{ filename };
	puzzle_t data;

	data.emplace_back(0, 0);

	for ( string line; getline(input, line); ) {
		char         dir{};
		long         steps{};
		string       hexcode;
		stringstream sstrm{ line };

		if ( sstrm >> dir >> steps >> hexcode ) {
			steps = stol(hexcode.substr(2, 5), nullptr, 16);
			dir   = hexcode[7];

			data.emplace_back(get<0>(data.back()) + get<0>(movement[dir]) * steps,
			                  get<1>(data.back()) + get<1>(movement[dir]) * steps);

			border_steps += steps;
		}
	}

	return data;
}

long
shoelace(const puzzle_t& puzzle, const long border_steps)
{
	long area = 0;
	for ( size_t i = 1; i < puzzle.size(); ++i ) {
		const auto [x1, y1] = puzzle[i - 1];
		const auto [x2, y2] = puzzle[i];

		area += (x1 * y2 - y1 * x2);
	}
	return (border_steps + area) / 2 + 1;
}

void
part1()
{
	long border_steps = 0;
	auto puzzle       = read_file_part1("data/day18.txt", border_steps);
	cout << shoelace(puzzle, border_steps) << endl;
}

void
part2()
{
	long border_steps = 0;
	auto puzzle       = read_file_part2("data/day18.txt", border_steps);
	cout << shoelace(puzzle, border_steps) << endl;
}

int
main()
{
	part1();
	part2();
}
