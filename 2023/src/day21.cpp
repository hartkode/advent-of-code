#include <fstream>
#include <iostream>
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

	const auto height = lines.size();
	const auto width  = lines[0].size();

	set<position> neighbours;
	const auto [row, col] = pos;

	for ( const auto& [drow, dcol]: movements ) {
		const auto nrow = row + drow;
		const auto ncol = col + dcol;

		if ( nrow < height && ncol < width && lines[nrow][ncol] != '#' ) {
			neighbours.emplace(nrow, ncol);
		}
	}

	return neighbours;
}

size_t
count(const vector<string>& lines, position start, size_t rounds)
{
	set<position> positions;
	positions.emplace(start);

	for ( size_t round = 0; round != rounds; ++round ) {
		set<position> new_positions;
		for ( const auto& position: positions ) {
			const auto neighbours = find_neighbours(position, lines);
			new_positions.insert(neighbours.begin(), neighbours.end());
		}
		positions = std::move(new_positions);
	}
	return positions.size();
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

	const auto num_odd_tiles   = pow2(grid_width / 2 * 2 + 1);
	const auto num_even_tiles  = pow2((grid_width + 1) / 2 * 2);
	const auto num_odd_points  = count(lines, start, size * 2 + 1);
	const auto num_even_points = count(lines, start, size * 2);

	auto sum = num_odd_tiles * num_odd_points + num_even_tiles * num_even_points;

	const auto corner_top    = count(lines, { size - 1, col }, size - 1);
	const auto corner_right  = count(lines, { row, 0 }, size - 1);
	const auto corner_bottom = count(lines, { 0, col }, size - 1);
	const auto corner_left   = count(lines, { row, size - 1 }, size - 1);

	sum += corner_top + corner_right + corner_bottom + corner_left;

	const auto small_top_right    = count(lines, { size - 1, 0 }, size / 2 - 1);
	const auto small_top_left     = count(lines, { size - 1, size - 1 }, size / 2 - 1);
	const auto small_bottom_right = count(lines, { 0, 0 }, size / 2 - 1);
	const auto small_bottom_left  = count(lines, { 0, size - 1 }, size / 2 - 1);

	sum += (grid_width + 1) * (small_top_right + small_top_left + small_bottom_right + small_bottom_left);

	const auto large_top_right    = count(lines, { size - 1, 0 }, size * 3 / 2 - 1);
	const auto large_top_left     = count(lines, { size - 1, size - 1 }, size * 3 / 2 - 1);
	const auto large_bottom_right = count(lines, { 0, 0 }, size * 3 / 2 - 1);
	const auto large_bottom_left  = count(lines, { 0, size - 1 }, size * 3 / 2 - 1);

	sum += grid_width * (large_top_right + large_top_left + large_bottom_right + large_bottom_left);

	cout << sum << endl;
}

int
main()
{
	auto lines = read_file("data/day21.txt");
	part1(lines);
	part2(lines);
}
