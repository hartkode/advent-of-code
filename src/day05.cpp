#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

struct Entry {
	Entry(long destination, long source, long amount) // NOLINT
	    : destination_(destination)
	    , source_(source)
	    , amount_(amount)
	{
	}

	[[nodiscard]] bool in_range(long value) const
	{
		return source_ <= value && value < (source_ + amount_);
	}

	[[nodiscard]] long get_destination(long value) const
	{
		auto delta = value - source_;
		return destination_ + delta;
	}

	long destination_;
	long source_;
	long amount_;
};

vector<string>
read_file(string_view filename)
{
	fstream        input{ filename };
	vector<string> data;

	for ( string line; getline(input, line); ) {
		data.emplace_back(line);
	}

	return data;
}

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
part1(const vector<string>& lines)
{
	auto read_ints = [](const string& line) {
		stringstream iss{ line };
		return vector<long>{ istream_iterator<long>{ iss }, istream_iterator<long>{} };
	};

	auto seeds = read_ints(lines[0].substr(6));

	map<string, string>        category_mapping;
	map<string, vector<Entry>> range_mapping;

	// read blocks
	for ( size_t idx = 1; idx < lines.size(); ) {
		++idx; // skip empty line

		auto description = lines[idx++];
		description.erase(description.find(' '));
		auto parts = split(description, '-');

		category_mapping[parts[0]] = parts[2];

		vector<Entry> mapping;

		for ( ; idx < lines.size() && !lines[idx].empty(); ++idx ) {
			auto values = read_ints(lines[idx]);

			auto destination = values[0];
			auto source      = values[1];
			auto amount      = values[2];

			mapping.emplace_back(destination, source, amount);
		}

		range_mapping[parts[0]] = mapping;
	}

	auto min_value = numeric_limits<long>::max();
	for ( auto seed: seeds ) {
		for ( string category{ "seed" }; !category.empty(); category = category_mapping[category] ) {
			const auto& ranges = range_mapping[category];

			for (const auto& range: ranges) {
				if (range.in_range(seed)) {
					seed = range.get_destination(seed);
					break;
				}
			}
		}
		min_value = min(min_value, seed);
	}
	cout << min_value << endl;
}

void
part2(const vector<string>& lines)
{
	auto read_ints = [](const string& line) {
		stringstream iss{ line };
		return vector<long>{ istream_iterator<long>{ iss }, istream_iterator<long>{} };
	};

	auto seeds = read_ints(lines[0].substr(6));

	map<string, string>        category_mapping;
	map<string, vector<Entry>> range_mapping;

	// read blocks
	for ( size_t idx = 1; idx < lines.size(); ) {
		++idx; // skip empty line

		auto description = lines[idx++];
		description.erase(description.find(' '));
		auto parts = split(description, '-');

		category_mapping[parts[0]] = parts[2];

		vector<Entry> mapping;

		for ( ; idx < lines.size() && !lines[idx].empty(); ++idx ) {
			auto values = read_ints(lines[idx]);

			auto destination = values[0];
			auto source      = values[1];
			auto amount      = values[2];

			mapping.emplace_back(destination, source, amount);
		}

		range_mapping[parts[0]] = mapping;
	}

	// brute force -- slow, but works
	auto min_value = numeric_limits<long>::max();
	for ( size_t idx = 0; idx != seeds.size(); idx += 2 ) {
		for ( auto start = seeds[idx]; start != seeds[idx] + seeds[idx+1]; ++start ) {
			auto seed = start;

			for ( string category{ "seed" }; !category.empty(); category = category_mapping[category] ) {
				const auto& ranges = range_mapping[category];

				for (const auto& range: ranges) {
					if (range.in_range(seed)) {
						seed = range.get_destination(seed);
						break;
					}
				}
			}
			min_value = min(min_value, seed);
		}
	}
	cout << min_value << endl;
}

int
main()
{
	auto lines = read_file("data/day05.txt");
	part1(lines);
	part2(lines);
}
