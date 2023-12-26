#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <vector>
using namespace std;

struct piece {
	int x1, y1, z1;
	int x2, y2, z2;
};

vector<piece>
read_file(string_view filename)
{
	static const regex pattern{ R"((\d+),(\d+),(\d+)~(\d+),(\d+),(\d+))" };

	fstream       input{ filename };
	vector<piece> data;

	for ( string line; getline(input, line); ) {
		smatch matches;
		if ( regex_search(line, matches, pattern) ) {
			data.emplace_back(piece{
			    stoi(matches[1]),
			    stoi(matches[2]),
			    stoi(matches[3]),
			    stoi(matches[4]),
			    stoi(matches[5]),
			    stoi(matches[6]) });
		}
	}
	sort(data.begin(), data.end(), [](const auto& lhs, const auto& rhs) { return lhs.z1 < rhs.z1; });

	return data;
}

void
part1()
{
	auto puzzle = read_file("data/day22.txt");

	map<int, map<int, int>> grid;

	map<size_t, vector<size_t>> supported_by;
	map<size_t, vector<size_t>> supports;

	for ( size_t idx = 0; idx != puzzle.size(); ++idx ) {
		auto& piece = puzzle[idx];

		int max_z = 0;
		for ( auto x = piece.x1; x <= piece.x2; ++x ) {
			for ( auto y = piece.y1; y <= piece.y2; ++y ) {
				max_z = max(max_z, grid[x][y]);
			}
		}

		auto height = piece.z2 - piece.z1 + 1;

		for ( auto x = piece.x1; x <= piece.x2; ++x ) {
			for ( auto y = piece.y1; y <= piece.y2; ++y ) {
				grid[x][y] = max_z + height;
			}
		}

		piece.z1 = max_z + 1;
		piece.z2 = max_z + height;

		for ( size_t idx2 = idx; idx2-- > 0; ) {
			if ( piece.x1 > puzzle[idx2].x2 || puzzle[idx2].x1 > piece.x2 ) {
				continue;
			}
			if ( piece.y1 > puzzle[idx2].y2 || puzzle[idx2].y1 > piece.y2 ) {
				continue;
			}
			if ( puzzle[idx].z1 == puzzle[idx2].z2 + 1 ) {
				supports[idx2].emplace_back(idx);
				supported_by[idx].emplace_back(idx2);
			}
		}
	}

	long number = 0;
	for ( size_t idx = 0; idx != puzzle.size(); ++idx ) {
		if ( all_of(supports[idx].begin(), supports[idx].end(), [&](const auto& idx2) { return supported_by[idx2].size() > 1; }) ) {
			++number;
		}
	}
	cout << number << endl;
}

int
main()
{
	part1();
}
