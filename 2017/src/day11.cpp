#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

// see: https://www.redblobgames.com/grids/hexagons/

using namespace std;

namespace {

string
read_file(const filesystem::path& filename)
{
	ifstream file{ filename };
	string   line;
	getline(file, line);
	return line;
}

vector<string>
split(const string& line)
{
	stringstream   strm{ line };
	vector<string> data;

	for ( string word; getline(strm, word, ','); ) {
		data.emplace_back(word);
	}

	return data;
}

void
part1(const vector<string>& steps)
{
	// string => (q, s, r)
	map<string, tuple<int, int, int>> dirs{
		{ "n", { 0, 1, -1 } },
		{ "nw", { -1, 1, 0 } },
		{ "sw", { -1, 0, 1 } },
		{ "s", { 0, -1, 1 } },
		{ "se", { 1, -1, 0 } },
		{ "ne", { 1, 0, -1 } }
	};

	tuple<int, int, int> current = { 0, 0, 0 };
	for ( const auto& step: steps ) {
		const auto [dq, ds, dr] = dirs.at(step);

		get<0>(current) += dq;
		get<1>(current) += ds;
		get<2>(current) += dr;
	}

	auto dist = (abs(get<0>(current)) + abs(get<1>(current)) + abs(get<2>(current))) / 2;

	cout << "Part1: " << dist << '\n';
}

void
part2(const vector<string>& steps)
{
	// string => (q, s, r)
	map<string, tuple<int, int, int>> dirs{
		{ "n", { 0, 1, -1 } },
		{ "nw", { -1, 1, 0 } },
		{ "sw", { -1, 0, 1 } },
		{ "s", { 0, -1, 1 } },
		{ "se", { 1, -1, 0 } },
		{ "ne", { 1, 0, -1 } }
	};

	int max_dist = 0;

	tuple<int, int, int> current = { 0, 0, 0 };
	for ( const auto& step: steps ) {
		const auto [dq, ds, dr] = dirs.at(step);

		get<0>(current) += dq;
		get<1>(current) += ds;
		get<2>(current) += dr;

		auto dist = (abs(get<0>(current)) + abs(get<1>(current)) + abs(get<2>(current))) / 2;

		max_dist = max(max_dist, dist);
	}

	cout << "Part2: " << max_dist << '\n';
}

} // namespace

int
main()
{
	auto line  = read_file("data/day11.txt");
	auto steps = split(line);
	part1(steps);
	part2(steps);
}
