#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <queue>
#include <set>
#include <string>
#include <vector>
using namespace std;

vector<string>
read_file(string_view filename)
{
	fstream        input{ filename };
	vector<string> data;

	for ( string line; getline(input, line); ) {
		data.emplace_back(line);
	}

	return data;
}

set<tuple<size_t, size_t>>
get_neighbours(const vector<string>& maze, tuple<size_t, size_t> possition)
{
	static const tuple<size_t, size_t> deltas[] = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } }; // NOLINT

	static const auto SYM_UP    = '^';
	static const auto SYM_RIGHT = '>';
	static const auto SYM_DOWN  = 'v';
	static const auto SYM_LEFT  = '<';

	const auto [row, col] = possition;

	switch ( maze[row][col] ) {
	case SYM_UP:
		return { make_tuple(row - 1, col) };
	case SYM_RIGHT:
		return { make_tuple(row, col + 1) };
	case SYM_DOWN:
		return { make_tuple(row + 1, col) };
	case SYM_LEFT:
		return { make_tuple(row, col - 1) };
	}

	set<tuple<size_t, size_t>> positions;

	for ( const auto& delta: deltas ) {
		const auto new_row = row + get<0>(delta);
		const auto new_col = col + get<1>(delta);

		if ( new_row >= maze.size() || new_col >= maze[0].size() ) {
			continue;
		}

		if ( maze[new_row][new_col] == '#' ) {
			continue;
		}

		if ( new_row < row && maze[new_row][new_col] == SYM_DOWN ) {
			continue;
		}
		if ( new_row > row && maze[new_row][new_col] == SYM_UP ) {
			continue;
		}
		if ( new_col < col && maze[new_row][new_col] == SYM_RIGHT ) {
			continue;
		}
		if ( new_col > col && maze[new_row][new_col] == SYM_LEFT ) {
			continue;
		}

		positions.emplace(new_row, new_col);
	}

	return positions;
}

void
part1(const vector<string>& maze)
{
	const tuple<size_t, size_t> start = { 0, maze[0].find('.') };
	const tuple<size_t, size_t> end   = { maze.size() - 1, maze[maze.size() - 1].find('.') };

	queue<tuple<tuple<size_t, size_t>, set<tuple<size_t, size_t>>>> queue;

	queue.emplace(start, set<tuple<size_t, size_t>>{});

	size_t longest = 0;
	while ( !queue.empty() ) {
		auto [position, seen] = queue.front();
		queue.pop();

		while ( position != end ) {
			seen.emplace(position);

			const auto neighbours = get_neighbours(maze, position);

			vector<tuple<size_t, size_t>> next_positions;

			// { neighbours \ seen }
			set_difference(neighbours.begin(), neighbours.end(),
			               seen.begin(), seen.end(),
			               back_inserter(next_positions));

			if ( next_positions.empty() ) {
				break;
			}

			position = next_positions[0];

			for ( size_t idx = 1; idx != next_positions.size(); ++idx ) {
				queue.emplace(next_positions[idx], seen);
			}
		}
		if ( position == end ) {
			longest = max(longest, seen.size());
		}
	}
	cout << longest << endl;
}

int
main()
{
	const auto maze = read_file("data/day23.txt");
	part1(maze);
}
