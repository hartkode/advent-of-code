#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
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

void
part2(const vector<string>& grid)
{
	using Pos = tuple<size_t, size_t>;

	map<Pos, long>             cache;
	function<long(const Pos&)> walk = [&](const Pos& pos) {
		if ( cache.contains(pos) ) {
			return cache.at(pos);
		}

		const auto [col, row] = pos;

		if ( row == grid.size() ) {
			return (cache[pos] = 1L);
		}

		if ( grid[row][col] == '^' ) {
			const auto left  = walk({ col - 1, row });
			const auto right = walk({ col + 1, row });

			return (cache[pos] = left + right);
		}

		return (cache[pos] = walk({ col, row + 1 }));
	};

	const auto col = grid[0].find('S');
	cout << "Part 2: " << walk(Pos{ col, 0 }) << '\n';
}

} // namespace

int
main()
{
	auto grid = read_file("data/day07.txt");
	part1(grid);
	part2(grid);
}
