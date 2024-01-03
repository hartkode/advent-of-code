#include <chrono>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <map>
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

void
part1(const vector<string>& lines)
{
	auto start = chrono::steady_clock::now();

	size_t sum = 0;
	for ( size_t col = 0; col != lines[0].size(); ++col ) {
		size_t counter = lines.size();

		for ( size_t row = 0; row != lines.size(); ++row ) {
			if ( lines[row][col] == 'O' ) {
				sum += counter;
				--counter;
			}
			if ( lines[row][col] == '#' ) {
				counter = lines.size() - row - 1;
			}
		}
	}

	auto duration = chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - start).count();

	cout << sum << " (" << duration << " μs)" << endl;
}

void
tilt_north(vector<string>& lines)
{
	for ( size_t col = 0; col != lines[0].size(); ++col ) {
		size_t row = 0;
		while ( row != lines.size() ) {
			size_t stones = 0;
			size_t empty  = 0;

			for ( size_t idx = row; idx != lines.size() && lines[idx][col] != '#'; ++idx ) {
				if ( lines[idx][col] == '.' ) {
					++empty;
				}
				if ( lines[idx][col] == 'O' ) {
					++stones;
				}
			}

			while ( stones-- != 0 ) {
				lines[row++][col] = 'O';
			}

			while ( empty-- != 0 ) {
				lines[row++][col] = '.';
			}

			while ( row != lines.size() && lines[row][col] == '#' ) {
				++row;
			}
		}
	}
}

void
tilt_south(vector<string>& lines)
{
	for ( size_t col = 0; col != lines[0].size(); ++col ) {
		size_t row = 0;
		while ( row != lines.size() ) {
			size_t stones = 0;
			size_t empty  = 0;

			for ( size_t idx = row; idx != lines.size() && lines[idx][col] != '#'; ++idx ) {
				if ( lines[idx][col] == '.' ) {
					++empty;
				}
				if ( lines[idx][col] == 'O' ) {
					++stones;
				}
			}

			while ( empty-- != 0 ) {
				lines[row++][col] = '.';
			}

			while ( stones-- != 0 ) {
				lines[row++][col] = 'O';
			}

			while ( row != lines.size() && lines[row][col] == '#' ) {
				++row;
			}
		}
	}
}

void
tilt_west(vector<string>& lines)
{
	for ( size_t row = 0; row != lines.size(); ++row ) {
		size_t col = 0;
		while ( col != lines[0].size() ) {
			size_t stones = 0;
			size_t empty  = 0;

			for ( size_t idx = col; idx != lines[0].size() && lines[row][idx] != '#'; ++idx ) {
				if ( lines[row][idx] == '.' ) {
					++empty;
				}
				if ( lines[row][idx] == 'O' ) {
					++stones;
				}
			}

			while ( stones-- != 0 ) {
				lines[row][col++] = 'O';
			}

			while ( empty-- != 0 ) {
				lines[row][col++] = '.';
			}

			while ( col != lines[0].size() && lines[row][col] == '#' ) {
				++col;
			}
		}
	}
}

void
tilt_east(vector<string>& lines)
{
	for ( size_t row = 0; row != lines.size(); ++row ) {
		size_t col = 0;
		while ( col != lines[0].size() ) {
			size_t stones = 0;
			size_t empty  = 0;

			for ( size_t idx = col; idx != lines[0].size() && lines[row][idx] != '#'; ++idx ) {
				if ( lines[row][idx] == '.' ) {
					++empty;
				}
				if ( lines[row][idx] == 'O' ) {
					++stones;
				}
			}

			while ( empty-- != 0 ) {
				lines[row][col++] = '.';
			}

			while ( stones-- != 0 ) {
				lines[row][col++] = 'O';
			}

			while ( col != lines[0].size() && lines[row][col] == '#' ) {
				++col;
			}
		}
	}
}

void
tilt(vector<string>& lines)
{
	for ( auto tilt: { tilt_north, tilt_west, tilt_south, tilt_east } ) {
		tilt(lines);
	}
}

size_t
calc(const vector<string>& lines)
{
	size_t sum     = 0;
	size_t counter = lines.size();
	for ( const auto& line: lines ) {
		sum += counter * static_cast<size_t>(count_if(line.begin(), line.end(), [](char chr) { return chr == 'O'; }));
		--counter;
	}
	return sum;
}

void
part2(vector<string> lines)
{
	auto                      start = chrono::steady_clock::now();
	map<vector<string>, long> cache;

	const long dest = 1'000'000'000;

	for ( long index = 0;; ++index ) {
		auto iter = cache.find(lines);
		if ( iter != cache.end() ) {
			auto offset       = iter->second;
			auto cycle_length = index - offset;

			auto moves_required = (dest - offset) % cycle_length;

			while ( moves_required-- != 0 ) {
				tilt(lines);
			}

			auto duration = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count();

			cout << calc(lines) << " (" << duration << " ms)" << endl;
			break;
		}
		else {
			cache[lines] = index;
		}

		tilt(lines);
	}
}

int
main()
{
	const auto lines = read_file("data/day14.txt");
	part1(lines);
	part2(lines);
}
