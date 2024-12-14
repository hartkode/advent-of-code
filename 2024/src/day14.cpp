#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <tuple>
#include <vector>
using namespace std;

using pos_type  = tuple<long, long>;
using velo_type = tuple<long, long>;

vector<tuple<pos_type, velo_type>>
read_file(string_view filename)
{
	static const regex pattern{ R"(^p=(-?\d+),(-?\d+) v=(-?\d+),(-?\d+)$)" };

	fstream input{ filename };

	vector<tuple<pos_type, velo_type>> data;
	for ( string line; getline(input, line); ) {
		smatch matches;

		if ( regex_search(line, matches, pattern) ) {
			data.push_back({ { stoi(matches[1]), stoi(matches[2]) },
			                 { stoi(matches[3]), stoi(matches[4]) } });
		}
	}
	return data;
}

/*
void
print(const vector<tuple<pos_type, velo_type>>& robots, long width, long height)
{
    map<pos_type, long> map;

    for ( const auto& [pos, velo]: robots ) {
        map[pos]++;
    }

    for ( long y = 0; y != height; ++y ) {
        for ( long x = 0; x != width; ++x ) {
            auto pos = map.find({ x, y });
            if ( pos != map.end() ) {
                cout << pos->second;
            }
            else {
                cout << '.';
            }
        }
        cout << '\n';
    }
    cout << endl;
}
*/

/*
void
print_pic(const vector<tuple<pos_type, velo_type>>& robots, long width, long height)
{
    map<pos_type, long> map;

    for ( const auto& [pos, velo]: robots ) {
        map[pos]++;
    }

    for ( long y = 0; y != height; ++y ) {
        for ( long x = 0; x != width; ++x ) {
            auto pos = map.find({ x, y });
            cout << (pos != map.end() ? 'X' : ' ');
        }
        cout << '\n';
    }
    cout << endl;
}
*/

long
count(const vector<tuple<pos_type, velo_type>>& robots, long width, long height)
{
	map<pos_type, long> map;

	const auto width_half  = width / 2;
	const auto height_half = height / 2;

	for ( const auto& [pos, velo]: robots ) {
		const auto [x, y] = pos;
		if ( x == width_half || y == height_half ) {
			continue;
		}
		map[{ x / (width_half + 1), y / (height_half + 1) }]++;
	}

	long result = 1;
	for ( const auto& [pos, count]: map ) {
		result *= count;
	}
	return result;
}

void
move(vector<tuple<pos_type, velo_type>>& robots, long width, long height)
{
	for ( auto& [pos, velo]: robots ) {
		auto& [x, y]  = pos;
		auto [vx, vy] = velo;

		x = (x + width + vx) % width;
		y = (y + height + vy) % height;
	}
}

void
part1(vector<tuple<pos_type, velo_type>> robots, long width, long height)
{
	for ( long second = 0; second != 100; ++second ) {
		move(robots, width, height);
	}

	cout << count(robots, width, height) << endl;
}

void
part2(vector<tuple<pos_type, velo_type>> robots, long width, long height)
{
	auto min_result   = numeric_limits<long>::max();
	long second_found = 0;

	for ( long second = 0; second != 100000; ++second ) {
		auto result = count(robots, width, height);

		if ( result < min_result ) {
			second_found = second;
			min_result   = result;
		}

		move(robots, width, height);
	}
	cout << second_found << endl;
}

void
part2_alternative(vector<tuple<pos_type, velo_type>> robots, long width, long height)
{
	set<pos_type> seen;

	for ( long second = 0; second != 100000; ++second ) {
		seen.clear();

		bool overlap = false;
		for ( const auto& [pos, velo]: robots ) {
			if ( seen.contains(pos) ) {
				overlap = true;
				break;
			}
			seen.insert(pos);
		}

		if ( !overlap ) {
			cout << second << endl;
			break;
		}

		move(robots, width, height);
	}
}

int
main()
{
#if 0
	auto robots = read_file("data/day14-sample1.txt");
	part1(robots, 11, 7);
#else
	auto robots = read_file("data/day14.txt");
	part1(robots, 101, 103);
	part2(robots, 101, 103);
	part2_alternative(robots, 101, 103);
#endif
}
