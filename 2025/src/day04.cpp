#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <tuple>

using namespace std;

namespace {

using Pos = tuple<size_t, size_t>;

set<Pos>
read_file(const filesystem::path& filename)
{
	ifstream file{ filename };
	set<Pos> grid;

	size_t row = 0;
	for ( string line; getline(file, line); ) {
		for ( size_t col = 0; col != line.length(); ++col ) {
			if ( line[col] == '@' ) {
				grid.emplace(col, row);
			}
		}
		++row;
	}

	return grid;
}

set<Pos>
get_neighbours(const Pos& pos)
{
	auto [col, row] = pos;

	return {
		Pos{ col - 1, row },
		Pos{ col - 1, row - 1 },
		Pos{ col, row - 1 },
		Pos{ col + 1, row - 1 },
		Pos{ col + 1, row },
		Pos{ col + 1, row + 1 },
		Pos{ col, row + 1 },
		Pos{ col - 1, row + 1 },
	};
}

bool
is_accessable(const set<Pos>& grid, const Pos& pos)
{
	long nneighbours = 0;
	for ( const auto neighbour: get_neighbours(pos) ) {
		if ( grid.contains(neighbour) ) {
			++nneighbours;
		}
	}
	return nneighbours < 4;
}

void
part1(const set<Pos>& grid)
{
	long count = 0;
	for ( const auto pos: grid ) {
		if ( is_accessable(grid, pos) ) {
			++count;
		}
	}
	cout << "Part 1: " << count << '\n';
}

void
part2(set<Pos> grid)
{
	auto old_size = grid.size();
	while ( true ) {
		set<Pos> remove;

		for ( const auto& pos: grid ) {
			if ( is_accessable(grid, pos) ) {
				remove.insert(pos);
			}
		}

		if ( remove.empty() ) {
			break;
		}

		for ( const auto& pos: remove ) {
			grid.erase(pos);
		}
	}
	cout << "Part 2: " << old_size - grid.size() << '\n';
}

} // namespace

int
main()
{
	auto grid = read_file("data/day04.txt");
	part1(grid);
	part2(grid);
}
