#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
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

const unsigned DIR_UP    = 0;
const unsigned DIR_RIGHT = 1;
const unsigned DIR_DOWN  = 2;
const unsigned DIR_LEFT  = 3;

size_t
solve(const vector<string>& lines, tuple<size_t, size_t, unsigned int> start)
{
	static const array<tuple<size_t, size_t>, 4> movement{
		make_tuple(0, -1),
		make_tuple(1, 0),
		make_tuple(0, 1),
		make_tuple(-1, 0),
	};

	map<tuple<size_t, size_t, unsigned int>, bool> visited;

	queue<tuple<size_t, size_t, unsigned int>> positions;

	positions.emplace(start);

	while ( !positions.empty() ) {
		auto [row, col, dir] = positions.front();
		positions.pop();

		while ( true ) {
			col += get<0>(movement.at(dir % 4));
			row += get<1>(movement.at(dir % 4));

			if ( row >= lines.size() || col >= lines[0].size() ) {
				break;
			}

			if ( visited[{ row, col, dir }] ) {
				break;
			}

			visited[{ row, col, dir }] = true;

			const auto chr = lines[row][col];

			if ( (chr == '|' && (dir == DIR_LEFT || dir == DIR_RIGHT)) ||
			     (chr == '-' && (dir == DIR_UP || dir == DIR_DOWN)) ) {
				dir = dir + 1;
				positions.emplace(row, col, dir + 2);
			}
			else if ( (chr == '/' && (dir == DIR_LEFT || dir == DIR_RIGHT)) ||
			          (chr == '\\' && (dir == DIR_UP || dir == DIR_DOWN)) ) {
				dir = dir + 3;
			}
			else if ( (chr == '\\' && (dir == DIR_LEFT || dir == DIR_RIGHT)) ||
			          (chr == '/' && (dir == DIR_UP || dir == DIR_DOWN)) ) {
				dir = dir + 1;
			}

			dir %= 4;
		}
	}

	set<tuple<size_t, size_t>> foo;
	for ( const auto& bar: visited ) {
		foo.emplace(get<0>(bar.first), get<1>(bar.first));
	}
	return foo.size();
}

void
part1(const vector<string>& lines)
{
	auto start = chrono::steady_clock::now();

	auto sum = solve(lines, { 0, -1, DIR_RIGHT });

	auto duration = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count();

	cout << sum << " (" << duration << "ms)" << endl;
}

void
part2(const vector<string>& lines)
{
	const auto rows = lines.size();
	const auto cols = lines[0].size();

	auto start = chrono::steady_clock::now();

	size_t sum = 0;
	for ( size_t row = 0; row != rows; ++row ) {
		sum = max(sum, solve(lines, { row, -1, DIR_RIGHT }));
		sum = max(sum, solve(lines, { row, cols, DIR_LEFT }));
	}
	for ( size_t col = 0; col != cols; ++col ) {
		sum = max(sum, solve(lines, { -1, col, DIR_DOWN }));
		sum = max(sum, solve(lines, { rows, col, DIR_UP }));
	}

	auto duration = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count();

	cout << sum << " (" << duration << "ms)" << endl;
}

int
main()
{
	const auto lines = read_file("data/day16.txt");
	part1(lines);
	part2(lines);
}
