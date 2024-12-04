#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
using namespace std;

map<tuple<size_t, size_t>, char>
read_file(string_view filename)
{
	fstream                          input{ filename };
	map<tuple<size_t, size_t>, char> data;

	size_t yPos = 0;
	for ( string line; getline(input, line); ) {
		for ( size_t xPos = 0; xPos != line.size(); ++xPos ) {
			data[{ xPos, yPos }] = line[xPos];
		}
		++yPos;
	}

	return data;
}

void
part1(map<tuple<size_t, size_t>, char> data, size_t size)
{
	static const tuple<size_t, size_t> dirs[] = {
		{ -1, -1 },
		{ 0, -1 },
		{ 1, -1 },
		{ 1, 0 },
		{ 1, 1 },
		{ 0, 1 },
		{ -1, 1 },
		{ -1, 0 },
	};
	long count = 0;
	for ( size_t y = 0; y != size; ++y ) {
		for ( size_t x = 0; x != size; ++x ) {
			for ( const auto& [dx, dy]: dirs ) {
				if ( data[{ x, y }] == 'X' &&
				     data[{ x + 1 * dx, y + 1 * dy }] == 'M' &&
				     data[{ x + 2 * dx, y + 2 * dy }] == 'A' &&
				     data[{ x + 3 * dx, y + 3 * dy }] == 'S' ) {
					++count;
				}
			}
		}
	}
	cout << count << endl;
}

void
part2(map<tuple<size_t, size_t>, char> data, size_t size)
{
	long count = 0;
	for ( size_t y = 0; y != size; ++y ) {
		for ( size_t x = 0; x != size; ++x ) {
			if ( data[{ x, y }] == 'A' &&
			     data[{ x - 1, y - 1 }] == 'M' &&
			     data[{ x - 1, y + 1 }] == 'M' &&
			     data[{ x + 1, y - 1 }] == 'S' &&
			     data[{ x + 1, y + 1 }] == 'S' ) {
				++count;
			}
			if ( data[{ x, y }] == 'A' &&
			     data[{ x - 1, y - 1 }] == 'S' &&
			     data[{ x - 1, y + 1 }] == 'M' &&
			     data[{ x + 1, y - 1 }] == 'S' &&
			     data[{ x + 1, y + 1 }] == 'M' ) {
				++count;
			}
			if ( data[{ x, y }] == 'A' &&
			     data[{ x - 1, y - 1 }] == 'S' &&
			     data[{ x - 1, y + 1 }] == 'S' &&
			     data[{ x + 1, y - 1 }] == 'M' &&
			     data[{ x + 1, y + 1 }] == 'M' ) {
				++count;
			}
			if ( data[{ x, y }] == 'A' &&
			     data[{ x - 1, y - 1 }] == 'M' &&
			     data[{ x - 1, y + 1 }] == 'S' &&
			     data[{ x + 1, y - 1 }] == 'M' &&
			     data[{ x + 1, y + 1 }] == 'S' ) {
				++count;
			}
		}
	}
	cout << count << endl;
}

int
main()
{
	auto data = read_file("data/day04.txt");
	part1(data, 140);
	part2(data, 140);
}
