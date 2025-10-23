#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>

using namespace std;

namespace {

using pos_type    = tuple<size_t, size_t>;
using grid_type   = map<pos_type, char>;
using points_type = map<char, pos_type>;
using dists_type  = map<tuple<char, char>, size_t>;

tuple<grid_type, points_type>
read_file(const filesystem::path& filename)
{
	ifstream file{ filename };

	grid_type   grid;
	points_type points;

	size_t row = 0;
	for ( string line; getline(file, line); ) {
		for ( size_t col = 0; col != line.size(); ++col ) {
			auto     chr = line[col];
			pos_type pos{ col, row };

			grid[pos] = chr;

			if ( isdigit(chr) != 0 ) {
				points[chr] = pos;
			}
		}

		++row;
	}
	return { grid, points };
}

size_t
bfs(const grid_type& grid, const pos_type src, const pos_type dest)
{
	set<pos_type>                  seen;
	queue<tuple<pos_type, size_t>> queue;

	queue.emplace(src, 0);
	seen.insert(src);

	while ( !queue.empty() ) {
		auto [pos, steps] = queue.front();
		queue.pop();

		if ( pos == dest ) {
			return steps;
		}

		static const pos_type dirs[]{ { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };
		const auto [x, y] = pos;

		for ( const auto [dx, dy]: dirs ) {
			const pos_type new_pos{ x + dx, y + dy };

			if ( grid.at(new_pos) == '#' || seen.contains(new_pos) ) {
				continue;
			}

			seen.insert(new_pos);
			queue.emplace(new_pos, steps + 1);
		}
	}

	return 0;
}

dists_type
build_dists(const grid_type& grid, const points_type& points)
{
	dists_type dists;

	for ( auto src = points.begin(); src != points.end(); ++src ) {
		for ( auto dest = next(src); dest != points.end(); ++dest ) {
			const auto dist = bfs(grid, src->second, dest->second);

			dists[{ src->first, dest->first }] = dist;
			dists[{ dest->first, src->first }] = dist;
		}
	}

	return dists;
}

size_t
shortest_route(const dists_type&  dists,
               const points_type& points,
               bool               return_to_start)
{
	vector<char> others;
	for ( const auto& [id, _]: points ) {
		if ( id != '0' ) {
			others.push_back(id);
		}
	}

	size_t best = numeric_limits<size_t>::max();

	do {
		size_t dist    = 0;
		char   current = '0';

		for ( char next: others ) {
			dist += dists.at({ current, next });
			current = next;
		}

		if ( return_to_start ) {
			dist += dists.at({ current, '0' });
		}

		best = min(best, dist);
	} while ( next_permutation(others.begin(), others.end()) );

	return best;
}

void
part1(const grid_type& grid, const points_type& points)
{
	auto dists = build_dists(grid, points);
	auto route = shortest_route(dists, points, false);
	cout << "Part1: " << route << '\n';
}

void
part2(const grid_type& grid, const points_type& points)
{
	auto dists = build_dists(grid, points);
	auto route = shortest_route(dists, points, true);
	cout << "Part2: " << route << '\n';
}

} // namespace

int
main()
{
	auto [grid, points] = read_file("data/day24.txt");
	part1(grid, points);
	part2(grid, points);
}
