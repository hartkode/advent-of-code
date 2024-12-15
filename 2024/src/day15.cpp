#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <tuple>
using namespace std;

using pos_type    = tuple<size_t, size_t>;
using room_type   = set<pos_type>;
using boxes_type  = set<pos_type>;
using puzzle_type = tuple<room_type, boxes_type, string, pos_type>;

puzzle_type
read_file(string_view filename)
{
	fstream    input{ filename };
	room_type  room;
	boxes_type boxes;
	string     movements;
	pos_type   start;

	size_t yPos = 0;
	for ( string line; getline(input, line); ) {
		if ( line.empty() ) {
			continue;
		}

		if ( line[0] == '#' ) {
			for ( size_t xPos = 0; xPos != line.size(); ++xPos ) {
				auto chr = line.at(xPos);
				if ( chr == '#' ) {
					room.emplace(xPos, yPos);
				}
				else if ( chr == 'O' ) {
					boxes.emplace(xPos, yPos);
				}
				else if ( chr == '@' ) {
					start = { xPos, yPos };
				}
			}
			++yPos;
		}
		else {
			movements += line;
		}
	}

	return { room, boxes, movements, start };
}

void
print(const room_type& room, const boxes_type& boxes, const pos_type& pos)
{
	for ( size_t yPos = 0; yPos != 10; ++yPos ) {
		for ( size_t xPos = 0; xPos != 10; ++xPos ) {
			if ( pos == pos_type{ xPos, yPos } ) {
				cout << '@';
			}
			else if ( room.contains({ xPos, yPos }) ) {
				cout << '#';
			}
			else if ( boxes.contains({ xPos, yPos }) ) {
				cout << 'O';
			}
			else {
				cout << ' ';
			}
		}
		cout << '\n';
	}
	cout << endl;
}

optional<boxes_type>
can_move(const room_type& room, const boxes_type& boxes, pos_type start, pos_type direction) // NOLINT
{
	const auto [x, y]   = start;
	const auto [dx, dy] = direction;

	boxes_type candidates;

	size_t steps = 1;
	while ( boxes.contains({ x + dx * steps, y + dy * steps }) ) {
		candidates.insert({ x + dx * steps, y + dy * steps });
		++steps;
	}

	if ( room.contains({ x + dx * steps, y + dy * steps }) ) {
		return nullopt;
	}

	return candidates;
}

void
part1(const puzzle_type& puzzle)
{
	map<char, pos_type> deltas = {
		{ '<', { -1, 0 } }, // Left
		{ '^', { 0, -1 } }, // Up
		{ '>', { 1, 0 } },  // Right
		{ 'v', { 0, 1 } }   // Down
	};

	auto [room, boxes, movements, start] = puzzle;

	for ( auto chr: movements ) {
		const auto delta = deltas[chr];
		if ( auto bitw = can_move(room, boxes, start, delta) ) {
			const auto [dx, dy] = delta;

			get<0>(start) += dx;
			get<1>(start) += dy;

			boxes_type new_boxes;
			for ( const auto& box: boxes ) {
				if ( bitw->contains(box) ) {
					const auto [x, y] = box;
					new_boxes.emplace(x + dx, y + dy);
					bitw->erase(box);
				}
				else {
					new_boxes.insert(box);
				}
			}
			boxes = new_boxes;
		}
	}

	size_t sum = 0;
	for (const auto& [x, y]: boxes) {
		sum += x + 100 * y;
	}
	cout << sum << endl;
}

int
main()
{
	const auto data = read_file("data/day15.txt");
	part1(data);
}
