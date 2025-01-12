#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

vector<string>
read_file(string_view filename)
{
	fstream input{ filename };

	vector<string> lines;

	for ( string line; getline(input, line); ) {
		lines.emplace_back(line);
	}

	return lines;
}

void
solve(const vector<string>& lines)
{
	string part1;
	string part2;

	for ( size_t col = 0; col != lines[0].length(); ++col ) {
		map<char, size_t> counts;
		for ( const auto& line: lines ) {
			++counts[line[col]];
		}

		part1 += max_element(counts.begin(), counts.end(), [](auto lhs, auto rhs) { return lhs.second < rhs.second; })->first;
		part2 += min_element(counts.begin(), counts.end(), [](auto lhs, auto rhs) { return lhs.second < rhs.second; })->first;
	}

	cout << part1 << '\n'
	     << part2 << endl;
}

int
main()
{
	auto lines = read_file("data/day06.txt");
	solve(lines);
}
