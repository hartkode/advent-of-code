#include <cstddef>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
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

constexpr vector<pos_type>
neighbors(pos_type pos)
{
	const auto [x, y] = pos;
	return { { x - 1, y }, { x, y - 1 }, { x + 1, y }, { x, y + 1 } };
}

size_t
flood_fill(const vector<tuple<pos_type, velo_type>>& robots)
{
	set<pos_type> positions;
	for ( const auto& [pos, velo]: robots ) {
		positions.insert(pos);
	}

	size_t max_seen = 0;
	for ( const auto& start: positions ) {
		set<pos_type> seen;

		queue<pos_type> queue;
		queue.push(start);

		while ( !queue.empty() ) {
			auto pos = queue.front();
			queue.pop();

			if ( seen.contains(pos) ) {
				continue;
			}

			seen.insert(pos);

			for ( const auto& neighbor: neighbors(pos) ) {
				if ( positions.contains(neighbor) ) {
					queue.push(neighbor);
				}
			}
		}
		max_seen = max(max_seen, seen.size());
	}

	return max_seen;
}

void
part2_flood_fill(vector<tuple<pos_type, velo_type>> robots, long width, long height)
{
	size_t max_count    = 0;
	long   second_found = 0;
	for ( long second = 0; second != 100000; ++second ) {
		auto count = flood_fill(robots);
		if ( count > max_count ) {
			max_count    = count;
			second_found = second;
		}
		move(robots, width, height);
	}
	cout << second_found << endl;
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
	part2_flood_fill(robots, 101, 103);
#endif
}
