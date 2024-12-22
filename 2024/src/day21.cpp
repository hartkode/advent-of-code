#undef NDEBUG
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <tuple>
#include <vector>
using namespace std;

using pos_type = tuple<int, int>;
using map_type = map<char, pos_type>;

vector<string>
read_file(string_view filename)
{
	fstream        input{ filename };
	vector<string> lines;

	for ( string line; getline(input, line); ) {
		lines.emplace_back(line);
	}

	return lines;
}

bool
is_valid_movement(pos_type start, const pos_type dest, string_view movement, const pos_type avoid)
{
	static const map_type directions = {
		{ '^', { 0, -1 } },
		{ 'v', { 0, 1 } },
		{ '<', { -1, 0 } },
		{ '>', { 1, 0 } }
	};

	for ( const auto key: movement ) {
		const auto [dx, dy] = directions.at(key);

		get<0>(start) += dx;
		get<1>(start) += dy;

		if ( start == avoid ) {
			return false;
		}
	}

	assert(start == dest); // NOLINT

	return true;
}

vector<string>
get_movements(const pos_type start, const pos_type end, const pos_type avoid)
{
	if ( start == end ) {
		return { "A" };
	}

	const auto [start_x, start_y] = start;
	const auto [end_x, end_y]     = end;

	string movement;

	const auto delta_y = end_y - start_y;
	if ( delta_y < 0 ) {
		movement.append(size_t(-delta_y), '^');
	}
	else if ( delta_y > 0 ) {
		movement.append(size_t(delta_y), 'v');
	}

	const auto delta_x = end_x - start_x;
	if ( delta_x < 0 ) {
		movement.append(size_t(-delta_x), '<');
	}
	else if ( delta_x > 0 ) {
		movement.append(size_t(delta_x), '>');
	}

	assert(!movement.empty()); // NOLINT

	vector<string> movements;

	ranges::sort(movement);
	do {
		if ( is_valid_movement(start, end, movement, avoid) ) {
			movements.emplace_back(movement + 'A');
		}
	} while ( next_permutation(movement.begin(), movement.end()) );

	assert(!movements.empty()); // NOLINT

	return movements;
}

size_t
find_rec(string_view keys, const int depth, const int max_depth)
{
	static const map_type keypad_door{
		{ '7', { 0, 0 } },
		{ '8', { 1, 0 } },
		{ '9', { 2, 0 } },

		{ '4', { 0, 1 } },
		{ '5', { 1, 1 } },
		{ '6', { 2, 1 } },

		{ '1', { 0, 2 } },
		{ '2', { 1, 2 } },
		{ '3', { 2, 2 } },

		{ '0', { 1, 3 } },
		{ 'A', { 2, 3 } },
	};

	static const map_type keypad_robot{
		{ '^', { 1, 0 } },
		{ 'A', { 2, 0 } },

		{ '<', { 0, 1 } },
		{ 'v', { 1, 1 } },
		{ '>', { 2, 1 } }
	};

	static map<tuple<string, int, int>, size_t> cache;

	if ( cache.contains({ string(keys), depth, max_depth }) ) {
		return cache.at({ string(keys), depth, max_depth });
	}

	const auto& keypad = depth == 0 ? keypad_door : keypad_robot;
	const auto& avoid  = depth == 0 ? pos_type{ 0, 3 } : pos_type{ 0, 0 };

	auto pos = keypad.at('A');

	size_t len = 0;
	for ( const auto key: keys ) {
		const auto end = keypad.at(key);

		const auto movements = get_movements(pos, end, avoid);

		if ( depth == max_depth ) {
			len += ranges::min_element(
			           movements, [](const auto& lhs, const auto& rhs) -> bool { return lhs.size() < rhs.size(); })
			           ->size();
		}
		else {
			auto curr_min = numeric_limits<size_t>::max();
			for ( const auto& movement: movements ) {
				curr_min = min(curr_min, find_rec(movement, depth + 1, max_depth));
			}
			len += curr_min;
		}

		pos = end;
	}

	return cache[{ string(keys), depth, max_depth }] = len;
}

size_t
find(string_view keys, const int depth)
{
	return find_rec(keys, 0, depth);
}

void
part1(const vector<string>& lines)
{
	cout << accumulate(lines.begin(), lines.end(), 0UL, [](auto init, const auto& line) {
		return init + find(line, 2) * strtoul(line.c_str(), nullptr, 10);
	}) << endl;
}

void
part2(const vector<string>& lines)
{
	cout << accumulate(lines.begin(), lines.end(), 0UL, [](auto init, const auto& line) {
		return init + find(line, 25) * strtoul(line.c_str(), nullptr, 10);
	}) << endl;
}

int
main()
{
	const auto lines = read_file("data/day21.txt");
	part1(lines);
	part2(lines);
}
