#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

namespace {

vector<string>
read_file(const filesystem::path& filename)
{
	ifstream       file{ filename };
	vector<string> grid;

	for ( string line; getline(file, line); ) {
		grid.emplace_back(line);
	}

	return grid;
}

void
part1(const vector<string>& grid)
{
	set<size_t> curr;
	curr.insert(grid[0].find('S'));

	int count = 0;
	for ( size_t i = 1; i < grid.size(); ++i ) {
		const auto& row = grid[i];

		auto next = curr;

		for ( const auto pos: curr ) {
			if ( row[pos] == '^' ) {
				next.erase(pos);
				next.insert(pos - 1);
				next.insert(pos + 1);
				++count;
			}
		}

		curr = next;
	}
	cout << "Part 1: " << count << '\n';
}

} // namespace

int
main()
{
	auto grid = read_file("data/day07.txt");
	part1(grid);
}
