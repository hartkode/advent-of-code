#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace {

vector<vector<int>>
read_file(const filesystem::path& filename)
{
	ifstream            file{ filename };
	vector<vector<int>> grid;

	for ( string line; getline(file, line); ) {
		stringstream strm{ line };

		vector<int> row;

		for ( int value{}; strm >> value; ) {
			row.emplace_back(value);
		}

		grid.emplace_back(row);
	}

	return grid;
}

void
part1(const vector<vector<int>>& grid)
{
	int checksum = 0;
	for ( const auto& row: grid ) {
		auto [min, max] = ranges::minmax_element(row);
		checksum += *max - *min;
	}
	cout << "Part1: " << checksum << '\n';
}

void
part2(const vector<vector<int>>& grid)
{
	int checksum = 0;
	for ( auto row: grid ) {
		ranges::sort(row, ranges::greater());

		for ( auto it = row.begin(); it != row.end(); ++it ) {
			for ( auto it2 = next(it); it2 != row.end(); ++it2 ) {
				if ( *it % *it2 == 0 ) {
					checksum += *it / *it2;
				}
			}
		}
	}
	cout << "Part2: " << checksum << '\n';
}

} // namespace

int
main()
{
	auto grid = read_file("data/day02.txt");
	part1(grid);
	part2(grid);
}
