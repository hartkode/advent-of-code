#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
using namespace std;

vector<tuple<long, vector<long>>>
read_file(string_view filename)
{
	fstream input{ filename };

	vector<tuple<long, vector<long>>> data;
	for ( string line; getline(input, line); ) {
		auto pos = line.find(':');

		stringstream str{ line.substr(pos + 1) };
		data.emplace_back(stol(line.substr(0, pos)), vector<long>{ istream_iterator<long>{ str }, {} });
	}
	return data;
}

bool
can_evaluated(long first, const vector<long>& values)
{
	for ( unsigned long pattern = 0; pattern != (1U << (values.size() - 1)); ++pattern ) {
		long result = values.at(0);
		for ( size_t idx = 1; idx < values.size(); ++idx ) {
			if ( (pattern & (1U << (idx - 1))) == 0 ) {
				result += values.at(idx);
			}
			else {
				result *= values.at(idx);
			}
		}
		if ( first == result ) {
			return true;
		}
	}
	return false;
}

void
part1(const vector<tuple<long, vector<long>>>& data)
{
	long sum = 0;
	for ( const auto& [first, values]: data ) {
		if ( can_evaluated(first, values) ) {
			sum += first;
		}
	}
	cout << sum << endl;
}

size_t
ipow(size_t base, size_t exp) // NOLINT
{
	size_t result = 1;
	while ( exp != 0 ) {
		if ( (exp & 1U) == 1 ) {
			result *= base;
		}
		exp >>= 1U;
		base *= base;
	}

	return result;
}

bool
can_evaluated2(long first, const vector<long>& values)
{
	const auto N = ipow(3, values.size() - 1); // NOLINT

	for ( unsigned long pattern = 0; pattern != N; ++pattern ) {
		auto test_pattern = pattern;

		long result = values.at(0);
		for ( size_t idx = 1; idx < values.size(); ++idx ) {
			auto rem = test_pattern % 3;

			if ( rem == 0 ) {
				result += values.at(idx);
			}
			else if ( rem == 1 ) {
				result *= values.at(idx);
			}
			else {
				result = stol(to_string(result) + to_string(values.at(idx)));
			}

			test_pattern /= 3;
		}
		if ( first == result ) {
			return true;
		}
	}
	return false;
}

void
part2(const vector<tuple<long, vector<long>>>& data)
{
	long sum = 0;
	for ( const auto& [first, values]: data ) {
		if ( can_evaluated2(first, values) ) {
			sum += first;
		}
	}
	cout << sum << endl;
}

int
main()
{
	auto data = read_file("data/day07.txt");

	part1(data);
	part2(data);
}
