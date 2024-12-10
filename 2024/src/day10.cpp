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

map<pos_type, int>
read_file(string_view filename)
{
	fstream            input{ filename };
	map<pos_type, int> data;

	size_t yPos = 0;
	for ( string line; getline(input, line); ) {
		for ( size_t xPos = 0; xPos != line.size(); ++xPos ) {
			data[{ xPos, yPos }] = line.at(xPos) - '0';
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

size_t
bfs(const map<pos_type, int>& data, pos_type start_pos)
{
	size_t peaks = 0;

	set<pos_type>   seen;
	queue<pos_type> queue;

	queue.emplace(start_pos);
	seen.emplace(start_pos);

	while ( !queue.empty() ) {
		auto pos = queue.front();
		queue.pop();

		if ( data.at(pos) == 9 ) {
			++peaks;
			continue;
		}

		for ( const auto& neighbor: neighbors(pos) ) {
			if ( !data.contains(neighbor) ||
			     seen.contains(neighbor) ||
			     data.at(neighbor) != data.at(pos) + 1 ) {
				continue;
			}

			queue.emplace(neighbor);
			seen.insert(neighbor);
		}
	}
	return peaks;
}

void
part1(const map<pos_type, int>& data)
{
	vector<pos_type> starts;
	for ( const auto& [pos, value]: data ) {
		if ( value == 0 ) {
			starts.push_back(pos);
		}
	}

	size_t sum = 0;
	for ( const auto& start: starts ) {
		sum += bfs(data, start);
	}
	cout << sum << endl;
}

size_t
bfs2(const map<pos_type, int>& data, pos_type start_pos)
{
	size_t paths = 0;

	queue<pos_type> queue;

	queue.push(start_pos);

	while ( !queue.empty() ) {
		auto pos = queue.front();
		queue.pop();

		if ( data.at(pos) == 9 ) {
			++paths;
			continue;
		}

		for ( const auto& neighbor: neighbors(pos) ) {
			if ( !data.contains(neighbor) ||
			     data.at(neighbor) != data.at(pos) + 1 ) {
				continue;
			}

			queue.emplace(neighbor);
		}
	}
	return paths;
}

void
part2(const map<pos_type, int>& data)
{
	vector<pos_type> starts;
	for ( const auto& [pos, value]: data ) {
		if ( value == 0 ) {
			starts.push_back(pos);
		}
	}

	size_t sum = 0;
	for ( const auto& start: starts ) {
		sum += bfs2(data, start);
	}
	cout << sum << endl;
}

int
main()
{
	auto data = read_file("data/day10.txt");
	part1(data);
	part2(data);
}
