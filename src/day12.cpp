#include <fstream>
#include <iostream>
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
brute_force(string_view puzzle, const vector<long>& nums)
{
	long counts = count_if(puzzle.begin(), puzzle.end(), [](char chr) { return chr == '?'; });

	string test_pattern;

	auto arrangements = 0L;
	for ( size_t counter = 0; counter != (1U << size_t(counts)); ++counter ) {
		auto bit_pattern = counter;

		for ( char chr: puzzle ) {
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
		if ( nums == count_groups(test_pattern) ) {
			arrangements++;
		}

		test_pattern.clear();
	}

	return arrangements;
}

void
part1()
{
	const auto input = read_file("data/day12.txt");

	auto sum = 0L;
	for ( const auto& line: input ) {
		auto parts  = split(line, ' ');
		auto puzzle = parts[0];
		auto nums   = read_ints(parts[1]);

		sum += brute_force(puzzle, nums);
	}
	cout << sum << endl;
}

int
main()
{
	part1();
}
