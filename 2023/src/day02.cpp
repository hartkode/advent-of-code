#include <cctype>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

vector<string>
split(const string& line, char sep)
{
	vector<string> parts;
	stringstream   input{ line };

	for ( string part; getline(input, part, sep); ) {
		parts.emplace_back(part);
	}

	return parts;
}

void
part1()
{
	fstream input{ "data/day02.txt" };

	auto sum = 0;
	for ( string line; getline(input, line); ) {
		auto game = split(line, ':');

		auto gameId = 0;
		sscanf(game[0].data(), "Game %d", &gameId); // NOLINT

		auto subsets = split(game[1], ';');

		auto fail = false;

		for ( const auto& subset: subsets ) {
			auto cubes = split(subset, ',');

			map<string, int> counts{};

			for ( const auto& cube: cubes ) {
				auto data = split(cube, ' ');

				auto count = stoi(data[1]);
				auto color = data[2];

				counts[color] += count;
			}

			fail |= (counts["red"] > 12) || (counts["green"] > 13) || (counts["blue"] > 14); // NOLINT
		}

		if ( !fail ) {
			sum += gameId;
		}
	}
	cout << sum << endl;
}

void
part2()
{
	fstream input{ "data/day02.txt" };

	auto sum = 0;

	for ( string line; getline(input, line); ) {
		auto game = split(line, ':');

		auto subsets = split(game[1], ';');

		map<string, int> colors{};

		for ( const auto& subset: subsets ) {
			auto cubes = split(subset, ',');

			for ( const auto& cube: cubes ) {
				auto data = split(cube, ' ');

				auto count = stoi(data[1]);
				auto color = data[2];

				colors[color] = max(colors[color], count);
			}
		}
		sum += colors["red"] * colors["green"] * colors["blue"];
	}
	cout << sum << endl;
}

int
main()
{
	part1();
	part2();
}
