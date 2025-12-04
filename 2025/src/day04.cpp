#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
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

array<Pos, 8>
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

size_t
get_nneighbours(const set<Pos>& grid, const Pos& pos)
{
	return (size_t) ranges::count_if(get_neighbours(pos), [&](const auto& neighbour) { return grid.contains(neighbour); });
}

bool
is_accessable(const set<Pos>& grid, const Pos& pos)
{
	return get_nneighbours(grid, pos) < 4;
}

void
part1(const set<Pos>& grid)
{
	auto count = ranges::count_if(grid, [&](const auto& pos) { return is_accessable(grid, pos); });
	cout << "Part 1: " << count << '\n';
}

void
part2(set<Pos> grid)
{
	const auto old_size = grid.size();
	while ( true ) {
		vector<Pos> remove;

		for ( const auto& pos: grid ) {
			if ( is_accessable(grid, pos) ) {
				remove.emplace_back(pos);
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

void
part2_bfs(set<Pos> grid)
{
	const auto old_size = grid.size();

	map<Pos, size_t> counts;
	for ( const auto& pos: grid ) {
		counts[pos] = get_nneighbours(grid, pos);
	}

	queue<Pos> queue;
	for ( const auto& [pos, count]: counts ) {
		if ( count < 4 ) {
			queue.emplace(pos);
		}
	}

	while ( !queue.empty() ) {
		const auto pos = queue.front();
		queue.pop();

		if ( !grid.contains(pos) ) {
			continue;
		}

		grid.erase(pos);

		for ( const auto& neighbour: get_neighbours(pos) ) {
			if ( !grid.contains(neighbour) ) {
				continue;
			}
			if ( --counts[neighbour] == 3 ) {
				queue.emplace(neighbour);
			}
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
	part2_bfs(grid);
}
