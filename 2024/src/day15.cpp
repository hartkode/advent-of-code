#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <optional>
#include <set>
#include <string>
#include <tuple>
using namespace std;

using pos_type     = tuple<size_t, size_t>;
using room_type    = set<pos_type>;
using boxes_type   = set<pos_type>;
using boxes2_type  = map<pos_type, bool>;
using puzzle_type  = tuple<room_type, boxes_type, string, pos_type>;
using puzzle2_type = tuple<room_type, boxes2_type, string, pos_type>;

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

puzzle2_type
read_file2(string_view filename)
{
	fstream     input{ filename };
	room_type   room;
	boxes2_type boxes;
	string      movements;
	pos_type    start;

	size_t yPos = 0;
	for ( string line; getline(input, line); ) {
		if ( line.empty() ) {
			continue;
		}

		if ( line[0] == '#' ) {
			for ( size_t xPos = 0; xPos != line.size(); ++xPos ) {
				auto chr = line.at(xPos);
				if ( chr == '#' ) {
					room.emplace(xPos * 2, yPos);
					room.emplace(xPos * 2 + 1, yPos);
				}
				else if ( chr == 'O' ) {
					boxes.insert({ pos_type(xPos * 2, yPos), true });
					boxes.insert({ pos_type(xPos * 2 + 1, yPos), false });
				}
				else if ( chr == '@' ) {
					start = { xPos * 2, yPos };
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
	size_t width  = 0;
	size_t height = 0;

	for ( const auto& [x, y]: room ) {
		width  = max(width, x + 1);
		height = max(height, y + 1);
	}

	for ( size_t yPos = 0; yPos != height; ++yPos ) {
		for ( size_t xPos = 0; xPos != width; ++xPos ) {
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
				cout << '.';
			}
		}
		cout << '\n';
	}
	cout << endl;
}

void
print(const room_type& room, const boxes2_type& boxes, const pos_type& pos)
{
	size_t width  = 0;
	size_t height = 0;

	for ( const auto& [x, y]: room ) {
		width  = max(width, x + 1);
		height = max(height, y + 1);
	}

	for ( size_t yPos = 0; yPos != height; ++yPos ) {
		for ( size_t xPos = 0; xPos != width; ++xPos ) {
			if ( pos == pos_type{ xPos, yPos } ) {
				cout << '@';
			}
			else if ( room.contains({ xPos, yPos }) ) {
				cout << '#';
			}
			else if ( boxes.contains({ xPos, yPos }) ) {
				auto value = boxes.at({ xPos, yPos });
				cout << (value ? '[' : ']');
			}
			else {
				cout << '.';
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

			for ( const auto& box: bitw.value() ) {
				boxes.erase(box);
			}

			for ( auto [x, y]: bitw.value() ) {
				boxes.emplace(x + dx, y + dy);
			}
		}
	}

	cout << accumulate(boxes.begin(), boxes.end(), 0UL, [](auto init, const auto& box) {
		return init + get<0>(box) + 100 * get<1>(box);
	}) << endl;
}

tuple<pos_type, pos_type>
get_both_parts(const boxes2_type& boxes, pos_type pos)
{
	const auto [x, y] = pos;
	if ( boxes.at(pos) ) {
		return { pos, { x + 1, y } };
	}
	else {
		return { { x - 1, y }, pos };
	}
}

optional<boxes2_type>
can_move(const room_type& room, const boxes2_type& all_boxes, pos_type start, pos_type direction) // NOLINT
{
	function<optional<boxes2_type>(const boxes2_type&, size_t)> can_move_up_down = [&room, &all_boxes, &can_move_up_down](const boxes2_type& boxes, size_t dy) -> optional<boxes2_type> {
		boxes2_type candidates;

		for ( const auto& [pos, b]: boxes ) {
			const auto [x, y] = pos;
			pos_type try_pos(x, y + dy);

			if ( room.contains(try_pos) ) {
				return nullopt;
			}

			if ( all_boxes.contains(try_pos) ) {
				const auto [lhs, rhs] = get_both_parts(all_boxes, try_pos);

				assert(all_boxes.contains(lhs) && all_boxes.at(lhs) == true);  // NOLINT
				assert(all_boxes.contains(rhs) && all_boxes.at(rhs) == false); // NOLINT

				candidates.emplace(lhs, true);
				candidates.emplace(rhs, false);
			}
		}

		if ( candidates.empty() ) {
			return boxes2_type{};
		}

		if ( auto results = can_move_up_down(candidates, dy) ) {
			for ( const auto& result: results.value() ) {
				candidates.insert(result);
			}
			return candidates;
		}

		return nullopt;
	};

	const auto [x, y]   = start;
	const auto [dx, dy] = direction;

	if ( dy != 0 ) {
		return can_move_up_down({ { start, false } }, dy);
	}

	boxes2_type candidates;

	size_t steps = 1;
	while ( all_boxes.contains({ x + dx * steps, y + dy * steps }) ) {
		auto foo = all_boxes.at({ x + dx * steps, y + dy * steps });
		candidates.insert({ { x + dx * steps, y + dy * steps }, foo });
		++steps;
	}

	if ( room.contains({ x + dx * steps, y + dy * steps }) ) {
		return nullopt;
	}

	return candidates;
}

void
part2(const puzzle2_type& puzzle)
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

			for ( const auto& [box, b]: bitw.value() ) {
				boxes.erase(box);
			}

			for ( auto [pos, b]: bitw.value() ) {
				const auto [x, y] = pos;
				boxes.emplace(pos_type{ x + dx, y + dy }, b);
			}
		}
	}

	cout << accumulate(boxes.begin(), boxes.end(), 0UL, [](auto init, const auto& box) {
		const auto& [pos, b] = box;
		if ( !b ) {
			return init;
		}
		return init + get<0>(pos) + 100 * get<1>(pos);
	}) << endl;
}
int
main()
{
	part1(read_file("data/day15.txt"));
	part2(read_file2("data/day15.txt"));
}
