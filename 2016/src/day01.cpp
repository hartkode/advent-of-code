#include <array>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

vector<string>
split(string_view line, string_view delimiter)
{
	size_t pos_start = 0;
	size_t pos_end   = 0;

	vector<string> res;

	while ( (pos_end = line.find(delimiter, pos_start)) != string::npos ) {
		auto token = line.substr(pos_start, pos_end - pos_start);
		pos_start  = pos_end + delimiter.length();

		res.emplace_back(token);
	}

	if ( pos_start != line.size() ) {
		res.emplace_back(line.substr(pos_start));
	}
	return res;
}

vector<tuple<char, long>>
read_file(const string& filename)
{
	fstream input{ filename };
	string  content{ istreambuf_iterator<char>{ input }, istreambuf_iterator<char>{} };

	vector<tuple<char, long>> result;
	for ( const auto& part: split(content, ", ") ) {
		result.emplace_back(part[0], stol(part.substr(1)));
	}
	return result;
}

void
part1(const vector<tuple<char, long>>& input)
{
	array<tuple<long, long>, 4> dirs = {
		make_tuple(1L, 0L),
		make_tuple(0L, 1L),
		make_tuple(-1L, 0L),
		make_tuple(0L, -1L)
	};

	size_t dir   = 0;
	long   pos_x = 0;
	long   pos_y = 0;

	for ( const auto& [change, length]: input ) {
		if ( change == 'L' ) {
			dir = (dir + 1) % 4;
		}
		else {
			dir = (dir + 3) % 4;
		}

		auto [delta_x, delta_y] = dirs.at(dir);

		pos_x += length * delta_x;
		pos_y += length * delta_y;
	}

	cout << abs(pos_x) + abs(pos_y) << endl;
}

void
part2(const vector<tuple<char, long>>& input)
{
	array<tuple<long, long>, 4> dirs = {
		make_tuple(1L, 0L),
		make_tuple(0L, 1L),
		make_tuple(-1L, 0L),
		make_tuple(0L, -1L)
	};

	size_t dir   = 0;
	long   pos_x = 0;
	long   pos_y = 0;

	set<tuple<long, long>> visited;

	for ( auto [change, length]: input ) {
		if ( change == 'L' ) {
			dir = (dir + 1) % 4;
		}
		else {
			dir = (dir + 3) % 4;
		}

		auto [delta_x, delta_y] = dirs.at(dir);

		while ( length-- > 0 ) {
			pos_x += delta_x;
			pos_y += delta_y;

			if ( visited.contains({ pos_x, pos_y }) ) {
				cout << abs(pos_x) + abs(pos_y) << endl;
				return;
			}

			visited.emplace(pos_x, pos_y);
		}
	}
}

int
main()
{
	auto input = read_file("data/day01.txt");
	part1(input);
	part2(input);
}
