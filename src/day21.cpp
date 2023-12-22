#include <cstddef>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <vector>
using namespace std;

using position = tuple<size_t, size_t>;

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

position
find_start_position(const vector<string>& lines)
{
	for ( size_t row = 0; row != lines.size(); ++row ) {
		for ( size_t col = 0; col != lines[row].size(); ++col ) {
			if ( lines[row][col] == 'S' ) {
				return { row, col };
			}
		}
	}
	return {};
}

set<position>
find_neighbours(position pos, const vector<string>& lines)
{
	static const vector<position> movements = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };

	set<position> neighbours;
	const auto [row, col] = pos;

	for ( const auto& [drow, dcol]: movements ) {
		const auto nrow = row + drow;
		const auto ncol = col + dcol;

		if ( nrow < lines.size() && ncol < lines[0].size() && (lines[nrow][ncol] == '.' || lines[nrow][ncol] == 'S') ) {
			neighbours.emplace(nrow, ncol);
		}
	}

	return neighbours;
}

size_t
count(vector<string> lines, position start, size_t rounds)
{
	queue<position> positions;
	positions.emplace(start);

	size_t sum = 0;
	for ( size_t round = 0; round != rounds; ++round ) {
		set<position> next_positions;

		sum = 0;
		while ( !positions.empty() ) {
			const auto [curr_row, curr_col] = positions.front();
			lines[curr_row][curr_col]       = '.';

			const auto neighbours = find_neighbours(positions.front(), lines);
			positions.pop();

			for ( const auto& [row, col]: neighbours ) {
				lines[row][col] = 'O';
				next_positions.emplace(row, col);
				++sum;
			}
		}

		for ( const auto& position: next_positions ) {
			positions.emplace(position);
		}
	}
	return sum;
}

void
part1(const vector<string>& lines)
{
	const auto start = find_start_position(lines);
	cout << count(lines, start, 64) << endl;
}

void
part2(const vector<string>& lines)
{
	const auto start      = find_start_position(lines);
	const auto [row, col] = start;

	const auto pow2 = [](size_t val) -> size_t {
		return val * val;
	};

	const auto   size       = lines.size();
	const size_t steps      = 26501365;
	const auto   grid_width = steps / size - 1;

	const auto odd  = pow2(grid_width / 2 * 2 + 1);
	const auto even = pow2((grid_width + 1) / 2 * 2);

	const auto odd_points  = count(lines, start, size * 2 + 1);
	const auto even_points = count(lines, start, size * 2);

	const auto corner_t = count(lines, { size - 1, col }, size - 1);
	const auto corner_r = count(lines, { row, 0 }, size - 1);
	const auto corner_b = count(lines, { 0, col }, size - 1);
	const auto corner_l = count(lines, { row, size - 1 }, size - 1);

	const auto small_tr = count(lines, { size - 1, 0 }, size / 2 - 1);
	const auto small_tl = count(lines, { size - 1, size - 1 }, size / 2 - 1);
	const auto small_br = count(lines, { 0, 0 }, size / 2 - 1);
	const auto small_bl = count(lines, { 0, size - 1 }, size / 2 - 1);

	const auto large_tr = count(lines, { size - 1, 0 }, size * 3 / 2 - 1);
	const auto large_tl = count(lines, { size - 1, size - 1 }, size * 3 / 2 - 1);
	const auto large_br = count(lines, { 0, 0 }, size * 3 / 2 - 1);
	const auto large_bl = count(lines, { 0, size - 1 }, size * 3 / 2 - 1);

	auto total = odd * odd_points;
	total += even * even_points;
	total += corner_t + corner_r + corner_b + corner_l;
	total += (grid_width + 1) * (small_tr + small_tl + small_br + small_bl);
	total += grid_width * (large_tr + large_tl + large_br + large_bl);

	cout << total << endl;
}

int
main()
{
	auto lines = read_file("data/day21.txt");
	part1(lines);
	part2(lines);
}
