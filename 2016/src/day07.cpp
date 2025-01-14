#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
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
part1(const vector<string>& lines)
{
	long sum = 0;
	for ( const auto& line: lines ) {
		long found_inner = 0;
		long found_outer = 0;

		bool inner = false;
		for ( size_t i = 0; i + 3 < line.length(); ++i ) {
			if ( line[i] == '[' ) {
				inner = true;
			}
			if ( line[i] == ']' ) {
				inner = false;
			}

			if ( line[i + 0] == line[i + 3] && line[i + 0] != line[i + 1] && line[i + 1] == line[i + 2] ) {
				if ( inner ) {
					++found_inner;
				}
				else {
					++found_outer;
				}
			}
		}
		sum += (found_inner == 0 && found_outer != 0) ? 1 : 0;
	}
	cout << sum << endl;
}

void
part2(const vector<string>& lines)
{
	long sum = 0;
	for ( const auto& line: lines ) {
		set<tuple<char, char>> inners;
		set<tuple<char, char>> outers;

		bool inner = false;
		for ( size_t i = 0; i + 2 < line.length(); ++i ) {
			if ( line[i] == '[' ) {
				inner = true;
			}
			if ( line[i] == ']' ) {
				inner = false;
			}

			if ( line[i + 0] == line[i + 2] && line[i + 0] != line[i + 1] ) {
				if ( inner ) {
					inners.emplace(line[i + 0], line[i + 1]);
					if ( outers.contains({ line[i + 1], line[i + 0] }) ) {
						++sum;
						break;
					}
				}
				else {
					outers.emplace(line[i + 0], line[i + 1]);
					if ( inners.contains({ line[i + 1], line[i + 0] }) ) {
						++sum;
						break;
					}
				}
			}
		}
	}
	cout << sum << endl;
}

int
main()
{
	auto lines = read_file("data/day07.txt");
	part1(lines);
	part2(lines);
}
