#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>
using namespace std;

vector<tuple<char, int>>
read_file(string_view filename)
{
	fstream                  input{ filename };
	vector<tuple<char, int>> result;

	char dir   = 0;
	int  count = 0;

	while ( input >> dir >> count ) {
		result.emplace_back(dir, count);
	}

	return result;
}

int
distance(const tuple<int, int>& head, const tuple<int, int>& tail)
{
	auto dist_x = abs(get<0>(head) - get<0>(tail));
	auto dist_y = abs(get<1>(head) - get<1>(tail));
	return max(dist_x, dist_y);
};

static map<char, tuple<int, int>> direction = {
	{ 'R', { +1, 0 } },
	{ 'L', { -1, 0 } },
	{ 'U', { 0, +1 } },
	{ 'D', { 0, -1 } }
};

void
part1(const vector<tuple<char, int>>& data)
{
	tuple<int, int> head = { 0, 0 };
	tuple<int, int> tail = head;

	set<tuple<int, int>> positions;

	for ( auto [dir, count]: data ) {
		while ( count-- > 0 ) {
			const auto [delta_x, delta_y] = direction[dir];
			const auto old_head           = head;

			get<0>(head) += delta_x;
			get<1>(head) += delta_y;

			if ( distance(head, tail) > 1 ) {
				tail = old_head;
			}

			positions.insert(tail);
		}
	}

	cout << positions.size() << endl;
}

void
part2(const vector<tuple<char, int>>& data)
{
	static const auto rope_size = 10;

	array<tuple<int, int>, rope_size> rope;

	set<tuple<int, int>> positions;

	for ( auto [dir, count]: data ) {
		while ( count-- > 0 ) {
			auto [delta_x, delta_y] = direction[dir];

			get<0>(rope[0]) += delta_x;
			get<1>(rope[0]) += delta_y;

			for ( size_t i = 1; i < rope.size(); ++i ) {
				auto& head = rope.at(i - 1);
				auto& tail = rope.at(i);

				if ( distance(head, tail) > 1 ) {
					delta_x = get<0>(head) - get<0>(tail);
					delta_y = get<1>(head) - get<1>(tail);

					if ( abs(delta_x) > 1 ) {
						delta_x /= abs(delta_x);
					}
					if ( abs(delta_y) > 1 ) {
						delta_y /= abs(delta_y);
					}

					get<0>(tail) += delta_x;
					get<1>(tail) += delta_y;
				}
			}

			positions.insert(rope[9]);
		}
	}

	cout << positions.size() << endl;
}

int
main()
{
	const auto data = read_file("data/day09.txt");
	part1(data);
	part2(data);
}
