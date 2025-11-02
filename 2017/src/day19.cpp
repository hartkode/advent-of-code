#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <tuple>

using namespace std;

namespace {

using dir_type    = tuple<size_t, size_t>;
using pos_type    = tuple<size_t, size_t>;
using puzzle_type = map<pos_type, char>;

puzzle_type
read_file(const filesystem::path& filename)
{
	ifstream    file{ filename };
	puzzle_type data;

	size_t row = 0;
	for ( string line; getline(file, line); ) {
		for ( size_t col = 0; col != line.size(); ++col ) {
			if ( line.at(col) != ' ' ) {
				data.emplace(pos_type{ col, row }, line.at(col));
			}
		}
		++row;
	}

	return data;
}

pos_type
find_start(const puzzle_type& puzzle)
{
	for ( size_t col = 0;; ++col ) {
		const pos_type pos{ col, 0 };
		if ( puzzle.contains(pos) && puzzle.at(pos) == '|' ) {
			return pos;
		}
	}
}

void
solve(const puzzle_type& puzzle)
{
	string result;
	size_t steps = 0;

	pos_type pos = find_start(puzzle);
	dir_type dir{ 0, 1 };

	while ( puzzle.contains(pos) ) {
		auto chr = puzzle.at(pos);

		if ( chr >= 'A' && chr <= 'Z' ) {
			result += chr;
		}
		else if ( chr == '+' ) {
			auto [dx, dy] = dir;
			auto left     = dir_type{ -dy, dx };
			auto right    = dir_type{ dy, -dx };

			pos_type next_left  = { get<0>(pos) + get<0>(left), get<1>(pos) + get<1>(left) };
			pos_type next_right = { get<0>(pos) + get<0>(right), get<1>(pos) + get<1>(right) };

			if ( puzzle.contains(next_left) ) {
				dir = left;
			}
			else if ( puzzle.contains(next_right) ) {
				dir = right;
			}
			else {
				cerr << "Error!\n";
				return;
			}
		}

		get<0>(pos) += get<0>(dir);
		get<1>(pos) += get<1>(dir);
		++steps;
	}

	cout << "Part1: " << result << '\n';
	cout << "Part2: " << steps << '\n';
}

} // namespace

int
main()
{
	auto puzzle = read_file("data/day19.txt");
	solve(puzzle);
}
