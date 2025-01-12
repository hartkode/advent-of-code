#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

vector<string>
read_file(string_view filename)
{
	fstream        input{ filename };
	vector<string> lines;

	vector<tuple<char, long>> result;
	for ( string line; getline(input, line); ) {
		lines.emplace_back(line);
	}
	return lines;
}

void
part1(const vector<string>& lines)
{
	int pos_x = 1;
	int pos_y = 1;

	for ( const auto& line: lines ) {
		for ( const auto chr: line ) {
			switch ( chr ) {
			case 'U':
				--pos_y;
				break;
			case 'R':
				++pos_x;
				break;
			case 'L':
				--pos_x;
				break;
			case 'D':
				++pos_y;
				break;
			default:
				cerr << "unknown char: " << chr << endl;
				return;
			}

			pos_x = clamp(pos_x, 0, 2);
			pos_y = clamp(pos_y, 0, 2);
		}
		cout << pos_y * 3 + pos_x + 1;
	}
	cout << endl;
}

void
part2(const vector<string>& lines)
{
	const map<tuple<int, int>, char> keypad = {
		{ { 2, 0 }, '1' },
		{ { 1, 1 }, '2' },
		{ { 2, 1 }, '3' },
		{ { 3, 1 }, '4' },
		{ { 0, 2 }, '5' },
		{ { 1, 2 }, '6' },
		{ { 2, 2 }, '7' },
		{ { 3, 2 }, '8' },
		{ { 4, 2 }, '9' },
		{ { 1, 3 }, 'A' },
		{ { 2, 3 }, 'B' },
		{ { 3, 3 }, 'C' },
		{ { 2, 4 }, 'D' }
	};

	tuple<int, int> pos = { 0, 2 };

	for ( const auto& line: lines ) {
		for ( const auto chr: line ) {
			auto pos2 = pos;
			switch ( chr ) {
			case 'U':
				--get<1>(pos2);
				break;
			case 'R':
				++get<0>(pos2);
				break;
			case 'L':
				--get<0>(pos2);
				break;
			case 'D':
				++get<1>(pos2);
				break;
			default:
				cerr << "unknown char: " << chr << endl;
				return;
			}

			if ( keypad.contains(pos2) ) {
				pos = pos2;
			}
		}
		cout << keypad.at(pos);
	}
	cout << endl;
}

int
main()
{
	auto lines = read_file("data/day02.txt");
	part1(lines);
	part2(lines);
}
