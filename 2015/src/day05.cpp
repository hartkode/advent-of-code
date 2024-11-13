#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

using namespace std;

auto
read_file(string_view filename)
{
	fstream        input{ filename };
	vector<string> lines;

	for ( string line; getline(input, line); ) {
		lines.emplace_back(line);
	}

	return lines;
}

void
part1(const vector<string>& lines)
{
	const regex re1("(.*[aeiou]){3}");
	const regex re2("(.)\\1");
	const regex re3("(ab|cd|pq|xy)");

	auto count = count_if(begin(lines), end(lines), [&](auto line) {
		return regex_search(line, re1) && regex_search(line, re2) && !regex_search(line, re3);
	});

	cout << count << endl;
}

void
part2(const vector<string>& lines)
{
	const regex re1("(..).*\\1");
	const regex re2("(.).\\1");

	auto count = count_if(begin(lines), end(lines), [&](auto line) {
		return regex_search(line, re1) && regex_search(line, re2);
	});

	cout << count << endl;
}

int
main()
{
	auto puzzle = read_file("data/day05.txt");
	part1(puzzle);
	part2(puzzle);
}
