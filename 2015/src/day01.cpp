#include <fstream>
#include <iostream>
#include <string>

using namespace std;

string
read_file(string_view filename)
{
	fstream input{ filename };
	return { istreambuf_iterator<char>{ input }, {} };
}

void
part1(string_view puzzle)
{
	int floor = 0;
	for (const auto& chr : puzzle) {
		if (chr == '(') {
			++floor;
		}
		else if (chr == ')') {
			--floor;
		}
	}
	cout << floor << endl;
}

void
part2(string_view puzzle)
{
	int floor = 0;
	for (string_view::size_type pos = 0; pos != puzzle.length(); ++pos) {
		const auto chr = puzzle[pos];
		if (chr == '(') {
			++floor;
		}
		else if (chr == ')') {
			--floor;
		}
		if (floor == -1) {
			cout << pos+1 << endl;
			break;
		}
	}
}

int
main()
{
	auto puzzle = read_file("data/day01.txt");
	part1(puzzle);
	part2(puzzle);
}
