#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <vector>
using namespace std;

set<char>
make_unique(string_view str)
{
	return { begin(str), end(str) };
}

map<char, int>
make_lookup()
{
	map<char, int> lookup;
	for ( int i = 0; i != 26; ++i ) {
		lookup[char('a' + i)] = i + 1;
		lookup[char('A' + i)] = i + 27;
	}
	return lookup;
}

vector<tuple<set<char>, set<char>>>
read_file(string_view filename)
{
	fstream                             input{ filename };
	vector<tuple<set<char>, set<char>>> data;

	for ( string line; getline(input, line); ) {
		const auto lhs = line.substr(0, line.length() / 2);
		const auto rhs = line.substr(line.length() / 2);
		data.emplace_back(make_unique(lhs), make_unique(rhs));
	}

	return data;
}

vector<string>
read_file2(string_view filename)
{
	fstream        input{ filename };
	vector<string> data;

	for ( string line; getline(input, line); ) {
		data.emplace_back(line);
	}

	return data;
}

void
part1(const vector<tuple<set<char>, set<char>>>& data)
{
	auto lookup = make_lookup();

	int sum = 0;
	for ( const auto& [lhs, rhs]: data ) {
		string intersection;
		set_intersection(begin(lhs), end(lhs), begin(rhs), end(rhs), back_inserter(intersection));

		sum += lookup[intersection[0]];
	}
	cout << sum << endl;
}

void
part2(const vector<string>& data)
{
	auto lookup = make_lookup();

	int sum = 0;
	for ( size_t i = 0; i < data.size(); i += 3 ) {
		map<char, int> count;

		const array<set<char>, 3> sets = { make_unique(data[i]),
			                               make_unique(data[i + 1]),
			                               make_unique(data[i + 2]) };

		for ( const auto& set: sets ) {
			for ( const auto chr: set ) {
				count[chr]++;
			}
		}

		for ( const auto& [key, value]: count ) {
			if ( value == 3 ) {
				sum += lookup[key];
			}
		}
	}
	cout << sum << endl;
}

int
main()
{
	const auto* const filename = "data/day03.txt";
	part1(read_file(filename));
	part2(read_file2(filename));
}
