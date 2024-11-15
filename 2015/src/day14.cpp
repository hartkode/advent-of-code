#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
using namespace std;

using record_type = tuple<string, long, long, long>;

auto
split(const string& line, char sep = ' ')
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
	fstream             input{ filename };
	vector<record_type> records;

	for ( string line; getline(input, line); ) {
		auto parts = split(line);
		records.emplace_back(parts[0], stol(parts[3]), stol(parts[6]), stol(parts[13])); // NOLINT
	}
	return records;
}

static const size_t max_seconds = 2503;

map<string, vector<long>>
calculate_distances(const vector<record_type>& records)
{
	map<string, vector<long>> distances;

	for ( const auto& [name, speed, sec, sec2]: records ) {
		long distance = 0;
		for ( size_t second = 0; second != max_seconds; ) {
			for ( size_t i = 0; second != max_seconds && i != (size_t) sec; ++i ) {
				distance += speed;
				++second;
				distances[name].emplace_back(distance);
			}
			for ( long i = 0; second != max_seconds && i != sec2; ++i ) {
				++second;
				distances[name].emplace_back(distance);
			}
		}
	}
	return distances;
}

void
part1(const vector<record_type>& records)
{
	auto distances = calculate_distances(records);
	long distance  = numeric_limits<long>::min();

	for ( const auto& record: records ) {
		auto name           = get<0>(record);
		auto final_distance = distances[name].back();
		distance            = max(distance, final_distance);
	}

	cout << distance << endl;
}

void
part2(const vector<record_type>& records)
{
	auto distances = calculate_distances(records);

	map<string, long> points;
	for ( size_t second = 0; second != max_seconds; ++second ) {
		auto max_distance = numeric_limits<long>::min();
		for ( const auto& record: records ) {
			auto name    = get<0>(record);
			max_distance = max(max_distance, distances[name][second]);
		}

		for ( const auto& record: records ) {
			auto name = get<0>(record);
			if ( distances[name][second] == max_distance ) {
				++points[name];
			}
		}
	}

	auto max_points = numeric_limits<long>::min();
	for ( const auto& [name, points]: points ) {
		max_points = max(max_points, points);
	}
	cout << max_points << endl;
}

int
main()
{
	auto records = read_file("data/day14.txt");
	part1(records);
	part2(records);
}
