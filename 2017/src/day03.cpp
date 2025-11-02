#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <map>

using namespace std;

namespace {

void
part1(int value)
{
	auto ring      = static_cast<int>(ceil((sqrt(value) - 1) / 2));
	auto side_len  = (2 * ring) + 1;
	auto max_value = side_len * side_len;

	array<int, 4> centers = {
		max_value - ring - (2 * ring * 0),
		max_value - ring - (2 * ring * 1),
		max_value - ring - (2 * ring * 2),
		max_value - ring - (2 * ring * 3)
	};

	auto* itr = ranges::min_element(centers, {}, [value](int rhs) { return abs(value - rhs); });

	auto dist_to_center = abs(value - *itr);

	cout << "Part1: " << ring + dist_to_center << '\n';
}

void
part2(int value)
{
	static const array<tuple<int, int>, 4> dirs{
		make_tuple(1, 0),  // right
		make_tuple(0, -1), // up
		make_tuple(-1, 0), // left
		make_tuple(0, 1),  // down
	};

	static const array<int, 3> offsets = { -1, 0, 1 };

	map<tuple<int, int>, int> grid; // (x,y) => value

	int x     = 0;
	int y     = 0;
	int steps = 1;

	grid[{ x, y }] = 1;

	while ( true ) {
		for ( const auto [dx, dy]: dirs ) {
			for ( int _ = 0; _ != steps; ++_ ) {
				x += dx;
				y += dy;

				int sum = 0;
				for ( const auto off_x: offsets ) {
					for ( const auto off_y: offsets ) {
						if ( off_x == 0 && off_y == 0 ) {
							continue;
						}

						const auto pos = make_tuple(x + off_x, y + off_y);
						if ( grid.contains(pos) ) {
							sum += grid.at(pos);
						}
					}
				}

				if ( sum > value ) {
					cout << "Part2: " << sum << '\n';
					return;
				}

				grid[{ x, y }] = sum;
			}
			if ( dx == 0 ) {
				++steps;
			}
		}
	}
}

} // namespace

int
main()
{
	static const auto puzzle = 265149;
	part1(puzzle);
	part2(puzzle);
}
