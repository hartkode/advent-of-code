#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

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
	fstream                          input{ filename };
	set<string>                      names;
	map<tuple<string, string>, long> table;

	for ( string line; getline(input, line); ) {
		if ( line.ends_with(".") ) {
			line.pop_back();
		}

		auto parts = split(line);

		auto start = parts[0];
		auto units = parts[2] == "gain" ? stol(parts[3]) : -stol(parts[3]);
		auto end   = parts[10]; // NOLINT

		table[{ start, end }] = units;

		names.insert(start);
	}
	return table;
}

void
part1(map<tuple<string, string>, long>& table)
{
	set<string> unique_names;
	for ( const auto& item: table ) {
		unique_names.insert(get<0>(item.first));
	}
	vector<string> names{ begin(unique_names), end(unique_names) };

	long happiness_sofar = numeric_limits<long>::min();
	do {
		long happiness = 0;
		for ( size_t idx = 0; idx != names.size(); ++idx ) {
			auto next_idx = (idx + 1) % names.size();

			happiness += table[{ names[idx], names[next_idx] }];
			happiness += table[{ names[next_idx], names[idx] }];
		}

		happiness_sofar = max(happiness_sofar, happiness);
	} while ( next_permutation(begin(names), end(names)) );
	cout << happiness_sofar << endl;
}

void
part2(map<tuple<string, string>, long>& table)
{
	set<string> unique_names;
	for ( const auto& item: table ) {
		unique_names.insert(get<0>(item.first));
	}
	vector<string> names{ begin(unique_names), end(unique_names) };
	names.emplace_back("Hartkode");

	long happiness_sofar = numeric_limits<long>::min();
	do {
		long happiness = 0;
		for ( size_t idx = 0; idx != names.size(); ++idx ) {
			auto next_idx = (idx + 1) % names.size();

			happiness += table[{ names[idx], names[next_idx] }];
			happiness += table[{ names[next_idx], names[idx] }];
		}

		happiness_sofar = max(happiness_sofar, happiness);
	} while ( next_permutation(begin(names), end(names)) );
	cout << happiness_sofar << endl;
}

int
main()
{
	auto table = read_file("data/day13.txt");
	part1(table);
	part2(table);
}
