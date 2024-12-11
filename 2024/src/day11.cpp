#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <vector>
using namespace std;

vector<long>
read_file(string_view filename)
{
	fstream input{ filename };
	return { istream_iterator<long>{ input }, {} };
}

void
part1(vector<long> data)
{
	for ( size_t i = 0; i != 25; ++i ) {
		vector<long> temp;
		for ( const auto value: data ) {
			if ( value == 0 ) {
				temp.push_back(1);
				continue;
			}

			auto str = to_string(value);
			if ( str.size() % 2 == 0 ) {
				auto len = str.size() / 2;
				temp.push_back(stol(str.substr(0, len)));
				temp.push_back(stol(str.substr(len)));
				continue;
			}

			temp.push_back(value * 2024);
		}
		data.swap(temp);
	}
	cout << data.size() << endl;
}

void
part2(const vector<long>& data)
{
	static const int MAX_DEPTH = 75;

	map<tuple<long, int>, long> cache;

	function<long(long, int)> count = [&](long value, int depth) -> long {
		if ( cache.contains({ value, depth }) ) {
			return cache[{ value, depth }];
		}

		if ( depth == MAX_DEPTH ) {
			return 1;
		}

		if ( value == 0 ) {
			return cache[{ value, depth }] = count(1, depth + 1);
		}

		auto str = to_string(value);
		if ( str.size() % 2 == 0 ) {
			auto len = str.size() / 2;
			auto lhs = stol(str.substr(0, len));
			auto rhs = stol(str.substr(len));

			return cache[{ value, depth }] = count(lhs, depth + 1) + count(rhs, depth + 1);
		}

		return cache[{ value, depth }] = count(value * 2024, depth + 1);
	};

	cout << accumulate(data.begin(), data.end(), 0L, [&](long init, long value) { return init + count(value, 0); }) << endl;
}

int
main()
{
	auto data = read_file("data/day11.txt");
	part1(data);
	part2(data);
}
