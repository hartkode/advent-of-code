#include <cmath>
#include <fstream>
#include <iostream>
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

vector<tuple<long, long>>
find_points(const vector<string>& input)
{
	vector<tuple<long, long>> result;

	for ( size_t row = 0; row != input.size(); ++row ) {
		const auto& line = input[row];

		for ( size_t col = 0; col != line.size(); ++col ) {
			if ( line[col] == '#' ) {
				result.emplace_back(long(row), long(col));
			}
		}
	}

	return result;
}

void
find_empty_rows_and_cols(const vector<string>& input, set<long>& cols, set<long>& rows)
{
	for ( size_t row = 0; row != input.size(); ++row ) {
		const auto& line = input[row];
		if ( line.find('#') == line.npos ) {
			rows.insert(long(row));
		}
	}

	for ( size_t col = 0; col != input[0].size(); ++col ) {
		bool empty_col = true;
		for ( const auto& line: input ) {
			if ( line[col] == '#' ) {
				empty_col = false;
			}
		}
		if ( empty_col ) {
			cols.insert(long(col));
		}
	}
}

long
solve(const vector<string>& input, long scale)
{
	auto points = find_points(input);

	set<long> cols;
	set<long> rows;

	find_empty_rows_and_cols(input, cols, rows);

	for ( auto& point: points ) {
		const auto row = get<0>(point) + count_if(rows.begin(), rows.end(), [&](long row) { return row < get<0>(point); }) * (scale - 1);
		const auto col = get<1>(point) + count_if(cols.begin(), cols.end(), [&](long col) { return col < get<1>(point); }) * (scale - 1);

		point = { row, col };
	}

	auto sum = 0L;
	for ( size_t i = 0; i != points.size(); ++i ) {
		for ( size_t j = i + 1; j != points.size(); ++j ) {
			const auto& from = points[i];
			const auto& to   = points[j];

			sum += abs(get<0>(from) - get<0>(to)) + abs(get<1>(from) - get<1>(to));
		}
	}

	return sum;
}

int
main()
{
	const auto input = read_file("data/day11.txt");

	cout << "Part1: " << solve(input, 2) << endl;
	cout << "Part2: " << solve(input, 1000000) << endl;
}
