#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <vector>
using namespace std;

using puzzle_t = vector<vector<int>>;

puzzle_t
read_file(string_view filename)
{
	fstream  input{ filename };
	puzzle_t data;

	for ( string line; getline(input, line); ) {
		puzzle_t::value_type numbers;
		for ( auto chr: line ) {
			numbers.emplace_back(chr - '0');
		}
		data.emplace_back(numbers);
	}

	return data;
}

int
solve(const puzzle_t& puzzle, size_t min_steps, size_t max_steps)
{
	using pos_t = tuple<size_t, size_t>;
	using dir_t = tuple<size_t, size_t>;
	using entry = tuple<int, pos_t, dir_t>;

	static const array<pos_t, 4> directions = {
		make_tuple(0, 1),
		make_tuple(0, -1),
		make_tuple(1, 0),
		make_tuple(-1, 0)
	};

	priority_queue<entry, vector<entry>, greater<>> queue;

	set<tuple<pos_t, dir_t>> seen;

	const pos_t target = { puzzle.size() - 1, puzzle[0].size() - 1 };

	queue.emplace(0, pos_t(0, 0), dir_t(0, 0));

	while ( !queue.empty() ) {
		const auto [heat, pos, dir] = queue.top();
		queue.pop();

		if ( pos == target ) {
			return heat;
		}

		const auto key = make_tuple(pos, dir);
		if ( seen.contains(key) ) {
			continue;
		}
		seen.emplace(key);

		const dir_t inv_dir = { -get<0>(dir), -get<1>(dir) };

		for ( const auto& next_dir: directions ) {
			if ( next_dir == dir || next_dir == inv_dir ) {
				continue;
			}

			auto heat_so_far = heat;

			for ( size_t steps = 1; steps <= max_steps; ++steps ) {
				const pos_t next_pos = { get<0>(pos) + get<0>(next_dir) * steps,
					                     get<1>(pos) + get<1>(next_dir) * steps };

				if ( get<0>(next_pos) >= puzzle.size() || get<1>(next_pos) >= puzzle[0].size() ) {
					continue;
				}

				heat_so_far += puzzle[get<0>(next_pos)][get<1>(next_pos)];

				if ( steps >= min_steps ) {
					queue.emplace(heat_so_far, next_pos, next_dir);
				}
			}
		}
	}
	return -1;
}

void
part1(const puzzle_t& puzzle)
{
	cout << solve(puzzle, 1, 3) << endl;
}

void
part2(const puzzle_t& puzzle)
{
	cout << solve(puzzle, 4, 10) << endl;
}

int
main()
{
	const auto puzzle = read_file("data/day17.txt");
	part1(puzzle);
	part2(puzzle);
}
