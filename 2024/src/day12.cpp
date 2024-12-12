#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <tuple>
#include <vector>
using namespace std;

using pos_type   = tuple<size_t, size_t>;
using area_type  = set<pos_type>;
using areas_type = vector<area_type>;

map<pos_type, char>
read_file(string_view filename)
{
	fstream             input{ filename };
	map<pos_type, char> data;

	size_t yPos = 0;
	for ( string line; getline(input, line); ) {
		for ( size_t xPos = 0; xPos != line.size(); ++xPos ) {
			data[{ xPos, yPos }] = line.at(xPos);
		}
		++yPos;
	}
	return data;
}

constexpr vector<pos_type>
neighbors(pos_type pos)
{
	const auto [x, y] = pos;
	return { { x - 1, y }, { x, y - 1 }, { x + 1, y }, { x, y + 1 } };
}

areas_type
areas(map<pos_type, char> data)
{
	auto find_area = [&](pos_type start) {
		set<pos_type>   area;
		queue<pos_type> queue;
		queue.push(start);

		const auto digit = data.at(start);

		while ( !queue.empty() ) {
			auto pos = queue.front();
			queue.pop();

			if ( !data.contains(pos) ) {
				continue;
			}

			for ( const auto& neighbor: neighbors(pos) ) {
				if ( !data.contains(neighbor) ) {
					continue;
				}
				if ( data.at(neighbor) != digit ) {
					continue;
				}
				queue.push(neighbor);
			}

			area.insert(pos);
			data.erase(pos);
		}
		return area;
	};

	areas_type areas;
	while ( !data.empty() ) {
		auto area = find_area(data.begin()->first);
		areas.push_back(area);
	}
	return areas;
}

size_t
count_edges(const set<pos_type>& area)
{
	size_t sum = 0;
	for ( const auto& pos: area ) {
		for ( const auto& neighbor: neighbors(pos) ) {
			if ( !area.contains(neighbor) ) {
				++sum;
			}
		}
	}
	return sum;
}

size_t
count_common_sides(const set<pos_type>& area)
{
	size_t count = 0;
	for ( const auto& [x, y]: area ) {
		if ( area.contains({ x - 1, y }) ) {
			for ( const auto y2: { y - 1, y + 1 } ) {
				if ( !area.contains({ x, y2 }) && !area.contains({ x - 1, y2 }) ) {
					++count;
				}
			}
		}
		if ( area.contains({ x, y - 1 }) ) {
			for ( const auto x2: { x - 1, x + 1 } ) {
				if ( !area.contains({ x2, y }) && !area.contains({ x2, y - 1 }) ) {
					++count;
				}
			}
		}
	}
	return count;
}

void
part1(const map<pos_type, char>& data)
{
	size_t sum = 0;
	for ( const auto& area: areas(data) ) {
		auto edges = count_edges(area);
		sum += area.size() * edges;
	}
	cout << sum << endl;
}

void
part2(const map<pos_type, char>& data)
{
	size_t sum = 0;
	for ( const auto& area: areas(data) ) {
		auto edges        = count_edges(area);
		auto common_sides = count_common_sides(area);
		sum += area.size() * (edges - common_sides);
	}
	cout << sum << endl;
}

int
main()
{
	auto data = read_file("data/day12.txt");
	part1(data);
	part2(data);
}
