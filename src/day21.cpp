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

		if ( nrow < lines.size() && ncol < lines[0].size() && lines[nrow][ncol] == '.' ) {
			neighbours.emplace(nrow, ncol);
		}
	}

	return neighbours;
}

void
part1()
{
	auto lines          = read_file("data/day21.txt");
	auto start_position = find_start_position(lines);

	queue<position> positions;
	positions.emplace(start_position);

	long sum = 0;
	for ( int round = 0; round != 64; ++round ) {
		set<position> next_positions;

		sum = 0;
		while ( !positions.empty() ) {
			const auto [curr_row, curr_col] = positions.front();
			lines[curr_row][curr_col]       = '.';

			auto neighbours = find_neighbours(positions.front(), lines);
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
	cout << sum << endl;
}

int
main()
{
	part1();
}
