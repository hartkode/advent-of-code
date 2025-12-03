#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

namespace {

vector<vector<long>>
read_file(const filesystem::path& filename)
{
	ifstream             file{ filename };
	vector<vector<long>> lines;

	for ( string line; getline(file, line); ) {
		vector<long> numbers;
		for ( auto chr: line ) {
			if ( isdigit(chr) ) {
				numbers.emplace_back(chr - '0');
			}
		}
		lines.emplace_back(std::move(numbers));
	}

	return lines;
}

void
part1(const vector<vector<long>>& lines)
{
	long sum = 0;
	for ( const auto& line: lines ) {
		auto first  = max_element(line.begin(), prev(line.end()));
		auto second = max_element(next(first), line.end());
		sum += *first * 10 + *second;
	}
	cout << "Part 1: " << sum << '\n';
}

} // namespace

int
main()
{
	auto lines = read_file("data/day03.txt");
	part1(lines);
}
