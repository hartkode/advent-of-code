#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

namespace {

vector<int>
read_lines(const filesystem::path& filename)
{
	ifstream    file{ filename };
	vector<int> lines;

	for ( int value{}; file >> value; ) {
		lines.emplace_back(value);
	}

	return lines;
}

void
distribute(vector<int>& values)
{
	auto iter      = ranges::max_element(values);
	auto max_value = *iter;

	*iter = 0;

	while ( max_value-- > 0 ) {
		++iter;
		if ( iter == values.end() ) {
			iter = values.begin();
		}
		*iter += 1;
	}
}

void
part1(vector<int> values)
{
	set<vector<int>> seen;
	for ( int step = 0;; ++step ) {
		if ( seen.contains(values) ) {
			cout << "Part1: " << step << '\n';
			return;
		}

		seen.insert(values);

		distribute(values);
	}
}

void
part2(vector<int> values)
{
	map<vector<int>, int> seen;
	for ( int step = 0;; ++step ) {
		if ( seen.contains(values) ) {
			cout << "Part2: " << step - seen.at(values) << '\n';
			return;
		}

		seen.emplace(values, step);

		distribute(values);
	}
}

} // namespace

int
main()
{
	auto values = read_lines("data/day06.txt");
	part1(values);
	part2(values);
}
