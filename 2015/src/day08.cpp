#include <fstream>
#include <iostream>
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
	auto count = [](const string& line) -> size_t {
		size_t length = 0;
		auto   iter   = begin(line);

		while ( iter != end(line) ) {
			if ( *iter == '\\' ) {
				++iter;
				if ( *iter == '\\' || *iter == '"' ) {
					++length;
					++iter;
				}
				else if ( *iter == 'x' ) {
					++length;
					iter += 3;
				}
			}
			else {
				++iter;
				++length;
			}
		}
		return length - 2;
	};

	size_t sum = 0;
	for ( const auto& line: lines ) {
		sum += line.size() - count(line);
	}
	cout << sum << endl;
}

void
part2(const vector<string>& lines)
{
	long sum = 0;
	for ( const auto& line: lines ) {
		sum += 2 + count_if(begin(line), end(line), [](auto chr) { return chr == '\\' || chr == '"'; });
	}
	cout << sum << endl;
}

int
main()
{
	auto lines = read_file("data/day08.txt");
	part1(lines);
	part2(lines);
}
