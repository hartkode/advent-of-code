#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

auto
split(const string& line, char sep)
{
	vector<string> parts;
	stringstream   input{ line };

	for ( string part; getline(input, part, sep); ) {
		parts.emplace_back(part);
	}

	return parts;
}

auto
read_file(string_view filename)
{
	fstream                        input{ filename };
	map<string, map<string, long>> distances;

	for ( string line; getline(input, line); ) {
		const auto parts = split(line, ' ');

		distances[parts[0]][parts[2]] = stol(parts[4]);
		distances[parts[2]][parts[0]] = stol(parts[4]);
	}

	return distances;
}

void
part1(map<string, map<string, long>>& distances)
{
	vector<string> vertex;

	vertex.reserve(distances.size());
	for ( const auto& iter: distances ) {
		vertex.emplace_back(iter.first);
	}

	long min_path = numeric_limits<long>::max();

	do {
		long current_min_path = 0;

		auto start = vertex[0];
		for ( const auto& ver: vertex ) {
			current_min_path += distances[start][ver];
			start = ver;
		}

		min_path = min(min_path, current_min_path);
	} while ( next_permutation(begin(vertex), end(vertex)) );

	cout << min_path << endl;
}

void
part2(map<string, map<string, long>>& distances)
{
	vector<string> vertex;

	vertex.reserve(distances.size());
	for ( const auto& iter: distances ) {
		vertex.emplace_back(iter.first);
	}

	long max_path = numeric_limits<long>::min();

	do {
		long current_max_path = 0;

		auto start = vertex[0];
		for ( const auto& ver: vertex ) {
			current_max_path += distances[start][ver];
			start = ver;
		}

		max_path = max(max_path, current_max_path);
	} while ( next_permutation(begin(vertex), end(vertex)) );

	cout << max_path << endl;
}

int
main()
{
	auto distances = read_file("data/day09.txt");
	part1(distances);
	part2(distances);
}
