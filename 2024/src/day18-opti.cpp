#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
using namespace std;

using pos_type = tuple<long, long>;

vector<pos_type>
read_file(string_view filename)
{
	fstream          input{ filename };
	vector<pos_type> data;

	long lhs = 0;
	long rhs = 0;
	char chr = 0;

	while ( input >> lhs >> chr >> rhs ) {
		data.emplace_back(lhs, rhs);
	}
	return data;
}

optional<long>
bfs(const vector<pos_type>& data, long size)
{
	set<pos_type> stones{ data.begin(), data.end() };

	set<pos_type> seen;

	// x, y, distance
	queue<tuple<pos_type, long>> queue;
	queue.push({ { 0, 0 }, 0 });

	while ( !queue.empty() ) {
		const auto& [pos, distance] = queue.front();
		queue.pop();

		const auto [x, y] = pos;

		if ( x == size && y == size ) {
			return distance;
		}

		if ( x < 0 || y < 0 || x > size || y > size ) {
			continue;
		}

		if ( stones.contains(pos) ) {
			continue;
		}

		if ( seen.contains(pos) ) {
			continue;
		}
		seen.insert(pos);

		for ( const auto& [nx, ny]: vector<pos_type>{ { x - 1, y }, { x + 1, y }, { x, y - 1 }, { x, y + 1 } } ) {
			queue.push({ { nx, ny }, distance + 1 });
		}
	}
	return nullopt;
}

void
part1(const vector<pos_type>& data, long size)
{
	if ( auto result = bfs(data, size) ) {
		cout << *result << endl;
	}
}

void
part2(const vector<pos_type>& data, long size)
{
	size_t lhs = 0;
	size_t rhs = data.size() - 1;

	while ( lhs < rhs ) {
		auto mid = lhs + (rhs - lhs) / 2;

		if ( bfs({ data.begin(), data.begin() + ptrdiff_t(mid + 1) }, size) ) {
			lhs = mid + 1;
		}
		else {
			rhs = mid;
		}
	}

	const auto& [x, y] = data[lhs];
	cout << x << "," << y << endl;
}

int
main()
{
#if 0
	const auto data = read_file("data/day18-sample1.txt");
	const long size = 6;

	part1({ data.begin(), data.begin() + 12 }, size);
	part2(data, size);
#else
	const auto data = read_file("data/day18.txt");
	const long size = 70;

	part1({ data.begin(), data.begin() + 1024 }, size);
	part2(data, size);
#endif
}
