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

void
part1(const map<pos_type, char>& data)
{
	auto data_copy = data;

	auto flood_fill = [&](pos_type start) {
		set<pos_type>   area;
		queue<pos_type> queue;
		queue.push(start);

		while ( !queue.empty() ) {
			auto pos = queue.front();
			queue.pop();

			if ( !data_copy.contains(pos) ) {
				continue;
			}

			auto digit = data_copy.at(pos);

			for ( const auto& neighbor: neighbors(pos) ) {
				if ( !data_copy.contains(neighbor) ) {
					continue;
				}
				if ( data_copy.at(neighbor) != digit ) {
					continue;
				}
				queue.push(neighbor);
			}

			area.insert(pos);
			data_copy.erase(pos);
		}
		return area;
	};

	auto count_edges = [](const set<pos_type>& area) -> size_t {
		size_t sum = 0;

		for ( const auto& pos: area ) {
			for ( const auto& neighbor: neighbors(pos) ) {
				if ( !area.contains(neighbor) ) {
					++sum;
				}
			}
		}

		return sum;
	};

	size_t sum = 0;
	while ( !data_copy.empty() ) {
		auto area  = flood_fill(data_copy.begin()->first);
		auto edges = count_edges(area);

		sum += area.size() * edges;
	}
	cout << sum << endl;
}

int
main()
{
	auto data = read_file("data/day12.txt");
	part1(data);
}
