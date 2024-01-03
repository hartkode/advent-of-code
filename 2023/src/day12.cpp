#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

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
	vector<string> parts{};
	stringstream   input{ line };

	for ( string part; getline(input, part, sep); ) {
		parts.emplace_back(part);
	}

	return parts;
}

template<typename T = long>
vector<T>
read_ints(const string& line)
{
	vector<T>    parts{};
	stringstream input{ line };

	for ( string part; getline(input, part, ','); ) {
		parts.emplace_back(stol(part));
	}

	return parts;
}

vector<long>
count_groups(string_view str)
{
	vector<long> groups;

	size_t idx = 0U;
	while ( idx != str.length() ) {
		while ( idx != str.length() && str[idx] == '.' ) {
			++idx;
		}
		if ( idx != str.length() && str[idx] == '#' ) {
			auto num = 0L;
			while ( idx != str.length() && str[idx] == '#' ) {
				++num;
				++idx;
			}
			groups.emplace_back(num);
		}
	}

	return groups;
}

long
brute_force(string_view springs, const vector<long>& groups)
{
	long counts = count_if(springs.begin(), springs.end(), [](char chr) { return chr == '?'; });

	string test_pattern;

	auto arrangements = 0L;
	for ( size_t counter = 0; counter != (1U << size_t(counts)); ++counter ) {
		auto bit_pattern = counter;

		for ( char chr: springs ) {
			if ( chr == '?' ) {
				if ( (bit_pattern & 1U) != 0U ) {
					test_pattern += '.';
				}
				else {
					test_pattern += '#';
				}
				bit_pattern >>= 1U;
			}
			else {
				test_pattern += chr;
			}
		}
		if ( groups == count_groups(test_pattern) ) {
			arrangements++;
		}

		test_pattern.clear();
	}

	return arrangements;
}

long
count(const string& springs, const vector<long>& groups)
{
	map<tuple<string, vector<long>>, long> cache;

	function<long(string, const vector<long>&)> count_rec = [&](string springs, const vector<long>& groups) -> long {
		auto cached_value = cache.find(make_tuple(springs, groups));
		if ( cached_value != cache.end() ) {
			return cached_value->second;
		}

		springs.erase(0, springs.find_first_not_of('.'));

		if ( springs.empty() ) {
			return groups.empty() ? 1 : 0;
		}

		if ( groups.empty() ) {
			return springs.find('#') == string_view::npos ? 1 : 0;
		}

		if ( springs[0] == '#' ) {
			auto gidx = size_t(groups[0]);
			if ( springs.length() < gidx || springs.substr(0, gidx).find('.') != string_view::npos || springs[gidx] == '#' ) {
				return 0;
			}

			return count_rec(springs.substr(gidx + 1), { groups.begin() + 1, groups.end() });
		}

		auto value = count_rec(string("#") + springs.substr(1), groups) + count_rec(springs.substr(1), groups);

		cache[make_tuple(springs, groups)] = value;

		return value;
	};

	return count_rec(springs + ".", groups);
}

void
part1(const vector<string>& input)
{
	auto sum = 0L;
	for ( const auto& line: input ) {
		auto parts   = split(line, ' ');
		auto springs = parts[0];
		auto groups  = read_ints(parts[1]);

		sum += brute_force(springs, groups);
		// sum += count(springs, groups);
	}
	cout << sum << endl;
}

void
part2(const vector<string>& input)
{
	auto sum = 0L;
	for ( const auto& line: input ) {
		auto parts   = split(line, ' ');
		auto springs = parts[0] + "?" + parts[0] + "?" + parts[0] + "?" + parts[0] + "?" + parts[0];
		auto groups  = read_ints(parts[1] + "," + parts[1] + "," + parts[1] + "," + parts[1] + "," + parts[1]);

		sum += count(springs, groups);
	}
	cout << sum << endl;
}

int
main()
{
	const auto input = read_file("data/day12.txt");
	part1(input);
	part2(input);
}
