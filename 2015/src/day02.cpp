#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

auto
read_file(string_view filename)
{
	fstream                      input{ filename };
	vector<tuple<int, int, int>> puzzle;
	for ( string line; getline(input, line); ) {
		int w, h, l;
		if ( sscanf(line.c_str(), "%dx%dx%d", &w, &h, &l) == 3 ) {
			puzzle.emplace_back(w, h, l);
		}
	}
	return puzzle;
}

void
part1(const vector<tuple<int, int, int>>& puzzle)
{
	int sum = 0;
	for ( const auto& [w, h, l]: puzzle ) {
		auto side1 = l * w;
		auto side2 = w * h;
		auto side3 = h * l;

		sum += 2 * side1 + 2 * side2 + 2 * side3 + min({ side1, side2, side3 });
	}
	cout << sum << endl;
}

void
part2(const vector<tuple<int, int, int>>& puzzle)
{
	int sum = 0;
	for ( const auto& [w, h, l]: puzzle ) {
		auto side1 = 2 * l;
		auto side2 = 2 * w;
		auto side3 = 2 * h;

		sum += side1 + side2 + side3 - max({ side1, side2, side3 }) + l * w * h;
	}
	cout << sum << endl;
}

int
main()
{
	auto puzzle = read_file("data/day02.txt");
	part1(puzzle);
	part2(puzzle);
}
