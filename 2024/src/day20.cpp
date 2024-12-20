#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <tuple>
#include <vector>
using namespace std;

using pos_type = tuple<size_t, size_t>;

tuple<pos_type, pos_type, set<pos_type>>
read_file(string_view filename)
{
	fstream       input{ filename };
	set<pos_type> walls;
	pos_type      start;
	pos_type      end;

	size_t yPos = 0;
	for ( string line; getline(input, line); ) {
		for ( size_t xPos = 0; xPos != line.size(); ++xPos ) {
			auto chr = line[xPos];
			if ( chr == '#' ) {
				walls.emplace(xPos, yPos);
			}
			else if ( chr == 'S' ) {
				start = { xPos, yPos };
			}
			else if ( chr == 'E' ) {
				end = { xPos, yPos };
			}
		}
		++yPos;
	}

	return { start, end, walls };
}

map<pos_type, long>
bfs(const tuple<pos_type, pos_type, set<pos_type>>& data)
{
	const auto& [start, end, walls] = data;

	map<pos_type, long> path;

	queue<tuple<pos_type, long>> queue;
	queue.push({ start, 0 });

	while ( !queue.empty() ) {
		auto [pos, dist] = queue.front();
		queue.pop();

		if ( path.contains(pos) ) {
			continue;
		}
		path[pos] = dist;

		if ( pos == end ) {
			break;
		}

		auto [x, y] = pos;
		for ( const auto& new_pos: vector<pos_type>{ { x - 1, y }, { x, y - 1 }, { x + 1, y }, { x, y + 1 } } ) {
			if ( !walls.contains(new_pos) ) {
				queue.push({ new_pos, dist + 1 });
			}
		}
	}
	return path;
}

void
part1(const tuple<pos_type, pos_type, set<pos_type>>& data)
{
	auto path = bfs(data);

	const auto& [start, end, walls] = data;

	long count = 0;
	for ( const auto& [pos, dist]: path ) {
		auto [x, y] = pos;

		for ( const auto& [dx, dy]: vector<pos_type>{ { -1, 0 }, { 0, -1 }, { 1, 0 }, { 0, 1 } } ) {
			if ( !walls.contains({ x + dx, y + dy }) ) {
				continue;
			}
			pos_type new_pos{ x + 2 * dx, y + 2 * dy };
			if ( path.contains(new_pos) && path.at(new_pos) - dist >= 100 + 2 ) {
				++count;
			}
		}
	}
	cout << count << endl;
}

void
part2(const tuple<pos_type, pos_type, set<pos_type>>& data)
{
	auto path = bfs(data);

	long count = 0;
	for ( const auto& [pos, dist]: path ) {
		auto [x, y] = pos;

		for ( size_t r = 2; r != 21; ++r ) {
			set<pos_type> candidates;
			for ( size_t dx = 0; dx != r + 1; ++dx ) {
				size_t dy = r - dx;

				candidates.insert({ x + dx, y + dy });
				candidates.insert({ x - dx, y + dy });
				candidates.insert({ x + dx, y - dy });
				candidates.insert({ x - dx, y - dy });
			}

			auto temp_dist = dist;
			count += ranges::count_if(candidates, [&](const auto& new_pos) {
				return path.contains(new_pos) && path.at(new_pos) - temp_dist >= 100 + long(r);
			});
		}
	}
	cout << count << endl;
}

int
main()
{
	auto data = read_file("data/day20.txt");
	part1(data);
	part2(data);
}
