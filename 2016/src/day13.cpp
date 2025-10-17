#include <bit>
#include <iostream>
#include <queue>
#include <set>

using namespace std;

namespace {

using pos_type = tuple<long, long>;

bool
is_wall(long x, long y, long number)
{
	auto result = (x * x) + (3 * x) + (2 * x * y) + y + (y * y);
	result += number;
	auto num = popcount(static_cast<unsigned long>(result));

	return (num % 2) != 0;
}

int
part1(long initial_value, pos_type dest)
{
	pos_type start_pos = { 1, 1 };

	set<pos_type>               seen;
	queue<tuple<pos_type, int>> queue; // pos, #steps

	seen.emplace(start_pos);
	queue.emplace(start_pos, 0);

	while ( !queue.empty() ) {
		auto [pos, steps] = queue.front();
		queue.pop();

		if ( pos == dest ) {
			return steps;
		}

		static const pos_type dirs[] = { { -1, 0 }, { 0, -1 }, { 1, 0 }, { 0, 1 } };

		for ( const auto [dx, dy]: dirs ) {
			auto [x, y] = pos;
			x += dx;
			y += dy;

			if ( x < 0 || y < 0 ) {
				continue;
			}

			if ( seen.contains({ x, y }) ) {
				continue;
			}

			if ( !is_wall(x, y, initial_value) ) {
				seen.emplace(x, y);
				queue.emplace(pos_type{ x, y }, steps + 1);
			}
		}
	}
	return -1; // Error
}

int
part2(long initial_value, int max_steps)
{
	pos_type start_pos = { 1, 1 };

	int count = 0;

	set<pos_type>               seen;
	queue<tuple<pos_type, int>> queue; // pos, #steps

	seen.emplace(start_pos);
	queue.emplace(start_pos, 0);

	while ( !queue.empty() ) {
		auto [pos, steps] = queue.front();
		queue.pop();

		if ( steps > max_steps ) {
			continue;
		}

		++count;

		static const pos_type dirs[] = { { -1, 0 }, { 0, -1 }, { 1, 0 }, { 0, 1 } };

		for ( const auto [dx, dy]: dirs ) {
			auto [x, y] = pos;
			x += dx;
			y += dy;

			if ( x < 0 || y < 0 ) {
				continue;
			}

			if ( seen.contains({ x, y }) ) {
				continue;
			}

			if ( !is_wall(x, y, initial_value) ) {
				seen.emplace(x, y);
				queue.emplace(pos_type{ x, y }, steps + 1);
			}
		}
	}
	return count;
}

} // namespace

int
main()
{
#if 0
	static const long     number = 10;
	static const pos_type dest   = { 7, 4 };
#else
	static const long     number = 1352;
	static const pos_type dest   = { 31, 39 };
#endif

	cout << part1(number, dest) << '\n';
	cout << part2(number, 50) << '\n';
}
