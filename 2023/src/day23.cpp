#include <algorithm>
#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>
using namespace std;

using position_t = tuple<size_t, size_t>;

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

set<position_t>
get_neighbours(const vector<string>& maze, position_t possition)
{
	static const position_t deltas[] = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } }; // NOLINT

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

	set<position_t> positions;

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

set<position_t>
get_neighbours2(const vector<string>& maze, position_t possition)
{
	static const position_t deltas[] = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } }; // NOLINT

	const auto [row, col] = possition;

	set<position_t> positions;

	for ( const auto& delta: deltas ) {
		const auto new_row = row + get<0>(delta);
		const auto new_col = col + get<1>(delta);

		if ( new_row >= maze.size() || new_col >= maze[0].size() ) {
			continue;
		}

		if ( maze[new_row][new_col] == '#' ) {
			continue;
		}

		positions.emplace(new_row, new_col);
	}

	return positions;
}

void
solve(const vector<string>& maze, function<set<position_t>(const vector<string>&, position_t)> neighbours)
{
	const position_t start = { 0, maze[0].find('.') };
	const position_t end   = { maze.size() - 1, maze[maze.size() - 1].find('.') };

	vector<vector<bool>> visited(maze.size(), vector<bool>(maze[0].size()));

	function<long(position_t, long)> find_longest_path = [&](position_t position, long current) -> long {
		const auto [row, col] = position;

		if ( visited[row][col] ) {
			return 0;
		}

		if ( position == end ) {
			return current;
		}

		long value = 0;

		visited[row][col] = true;
		for ( const auto& neighbour: neighbours(maze, position) ) {
			value = max(value, find_longest_path(neighbour, current + 1));
		}
		visited[row][col] = false;

		return value;
	};

	auto value = find_longest_path(start, 0);
	cout << value << endl;
}

int
main()
{
	const auto maze = read_file("data/day23.txt");
	solve(maze, get_neighbours);
	solve(maze, get_neighbours2);
}
