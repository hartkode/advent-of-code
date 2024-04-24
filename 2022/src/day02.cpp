#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

vector<tuple<int, int>>
read_file(string_view filename)
{
	fstream                 input{ filename };
	vector<tuple<int, int>> data;

	for ( string line; getline(input, line); ) {
		data.emplace_back(line[0] - 'A' + 1, line[2] - 'X' + 1);
	}

	return data;
}

void
part1(const vector<tuple<int, int>>& lines)
{
	map<int, int> wins{
		{ 1, 2 },
		{ 2, 3 },
		{ 3, 1 }
	};

	static const int INC_DRAW = 3;
	static const int INC_WIN  = 6;

	int score = 0;
	for ( auto [a, b]: lines ) {
		score += b;
		if ( a == b ) {
			score += INC_DRAW;
		}
		else if ( wins[a] == b ) {
			score += INC_WIN;
		}
	}
	cout << score << endl;
}

void
part2(const vector<tuple<int, int>>& lines)
{
	map<int, int> wins{
		{ 1, 2 },
		{ 2, 3 },
		{ 3, 1 }
	};

	map<int, int> loose{
		{ 2, 1 },
		{ 3, 2 },
		{ 1, 3 }
	};

	static const int INC_DRAW = 3;
	static const int INC_WIN  = 6;

	int score = 0;
	for ( auto [a, b]: lines ) {
		if (b == 2) {
			score += a + INC_DRAW;
		}
		else if (b == 3) {
			score += wins[a] + INC_WIN;
		}
		else {
			score += loose[a];
		}
	}
	cout << score << endl;
}

int
main()
{
	auto lines = read_file("data/day02.txt");
	part1(lines);
	part2(lines);
}
