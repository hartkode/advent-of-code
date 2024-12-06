#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <tuple>
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
			data[{ xPos, yPos }] = line[xPos];
		}
		++yPos;
	}

	return data;
}

set<pos_type>
distinct_positions(const map<pos_type, char>& data)
{
	static const array<pos_type, 4> deltas = {
		make_tuple(0, -1),
		make_tuple(1, 0),
		make_tuple(0, 1),
		make_tuple(-1, 0)
	};

	size_t direction = 0;

	auto position = get<0>(*ranges::find_if(data, [](auto position) {
		return get<1>(position) == '^';
	}));

	set<pos_type> positions;
	while ( true ) {
		positions.emplace(position);

		auto next_position = make_tuple(get<0>(position) + get<0>(deltas.at(direction)),
		                                get<1>(position) + get<1>(deltas.at(direction)));

		if ( !data.contains(next_position) ) {
			break;
		}

		if ( data.at(next_position) == '#' ) {
			direction = (direction + 1) % deltas.size();
		}
		else {
			position = next_position;
		}
	}

	return positions;
}

void
part1(const map<pos_type, char>& data)
{
	auto positions = distinct_positions(data);

	cout << positions.size() << endl;
}

bool
would_loop(const map<pos_type, char>& data)
{
	static const array<pos_type, 4> deltas = {
		make_tuple(0, -1),
		make_tuple(1, 0),
		make_tuple(0, 1),
		make_tuple(-1, 0)
	};

	size_t direction = 0;

	auto position = get<0>(*ranges::find_if(data, [](auto position) {
		return get<1>(position) == '^';
	}));

	set<tuple<pos_type, size_t>> positions;

	while ( true ) {
		positions.emplace(position, direction);

		auto next_position = make_tuple(get<0>(position) + get<0>(deltas.at(direction)),
		                                get<1>(position) + get<1>(deltas.at(direction)));

		if ( !data.contains(next_position) ) {
			return false;
		}

		if ( data.at(next_position) == '#' ) {
			direction = (direction + 1) % deltas.size();
		}
		else {
			position = next_position;
		}

		if ( positions.contains({ position, direction }) ) {
			return true;
		}
	}
}

void
part2(map<pos_type, char> data)
{
	const auto positions = distinct_positions(data);

	long sum = 0;
	for ( const auto& position: positions ) {
		if ( data.at(position) == '.' ) {
			data.at(position) = '#';
			if ( would_loop(data) ) {
				++sum;
			}
			data.at(position) = '.';
		}
	}
	cout << sum << endl;
}

int
main()
{
	auto data = read_file("data/day06.txt");
	part1(data);
	part2(data);
}
