#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <numeric>
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

long
solve(const vector<long>& line, int n)
{
	long value = 0;
	auto start = line.begin();

	while ( n-- > 0 ) {
		start = max_element(start, prev(line.end(), n));

		value *= 10;
		value += *start++;
	}
	return value;
}

long
solve(const vector<vector<long>>& lines, int n)
{
	return accumulate(lines.begin(), lines.end(),
	                  0L,
	                  [&](auto sum, const auto& line) { return sum + solve(line, n); });
}

void
part1(const vector<vector<long>>& lines)
{
	cout << "Part 1: " << solve(lines, 2) << '\n';
}

void
part2(const vector<vector<long>>& lines)
{
	cout << "Part 2: " << solve(lines, 12) << '\n';
}

} // namespace

int
main()
{
	auto lines = read_file("data/day03.txt");
	part1(lines);
	part2(lines);
}
