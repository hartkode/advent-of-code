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

int
main()
{
	auto data = read_file("data/day07.txt");

	part1(data);
}
