#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <tuple>

using namespace std;

namespace {

set<tuple<int, int>>
read_file(const filesystem::path& filename)
{
	ifstream             file{ filename };
	set<tuple<int, int>> data;

	int row = 0;
	for ( string line; getline(file, line); ) {
		int col = 0;

		for ( char chr: line ) {
			if ( chr == '#' ) {
				data.emplace(col, row);
			}
			++col;
		}

		++row;
	}

	return data;
}

#if 0
void
print(const set<tuple<int, int>>& grid, tuple<int, int> pos)
{
	for ( int y = -5; y < 5; ++y ) {
		for ( int x = -5; x < 5; ++x ) {
			auto coord = make_tuple(x, y);

			if ( pos == coord ) {
				cout << '[';
			}
			else {
				cout << ' ';
			}

			auto chr = grid.contains(coord) ? '#' : '.';
			cout << chr;

			if ( pos == coord ) {
				cout << ']';
			}
			else {
				cout << ' ';
			}
		}
		cout << '\n';
	}
	cout << '\n';
}
#endif

void
part1(set<tuple<int, int>> grid, tuple<int, int> pos)
{
	const array<tuple<int, int>, 4> dirs{
		make_tuple(-1, 0),
		make_tuple(0, -1),
		make_tuple(1, 0),
		make_tuple(0, 1)
	};
	unsigned dir = 1; // up

	int infections = 0;
	for ( int i = 0; i != 10000; ++i ) {
		if ( grid.contains(pos) ) {
			dir = (dir + 1) % 4;
			grid.erase(pos);
		}
		else {
			dir = (dir + 3) % 4;
			grid.insert(pos);
			++infections;
		}

		auto [dx, dy] = dirs.at(dir);
		get<0>(pos) += dx;
		get<1>(pos) += dy;
	}
	cout << "Part1: " << infections << '\n';
}

void
part2(set<tuple<int, int>> grid, tuple<int, int> pos)
{
	const array<tuple<int, int>, 4> dirs{
		make_tuple(-1, 0),
		make_tuple(0, -1),
		make_tuple(1, 0),
		make_tuple(0, 1)
	};
	unsigned dir = 1; // up

	static const int weak     = 0;
	static const int infected = 1;
	static const int flagged  = 2;

	map<tuple<int, int>, int> grid_;
	for ( const auto& value: grid ) {
		grid_[value] = infected;
	}

	int infections = 0;
	for ( int i = 0; i != 10000000; ++i ) {
		if ( !grid_.contains(pos) ) {
			grid_[pos] = weak;
			dir        = (dir + 3) % 4;
		}
		else {
			switch ( grid_[pos] ) {
			case weak:
				grid_[pos] = infected;
				++infections;
				break;
			case infected:
				grid_[pos] = flagged;
				dir        = (dir + 1) % 4;
				break;
			case flagged:
				grid_.erase(pos);
				dir = (dir + 2) % 4;
				break;
			default:
				cerr << "Wrong state!!" << '\n';
				return;
			}
		}

		auto [dx, dy] = dirs.at(dir);
		get<0>(pos) += dx;
		get<1>(pos) += dy;
	}
	cout << "Part2: " << infections << '\n';
}

} // namespace

int
main()
{
	auto grid = read_file("data/day22.txt");
	part1(grid, { 12, 12 });
	part2(grid, { 12, 12 });
}
