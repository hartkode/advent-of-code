#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <numeric>
using namespace std;

auto
read_file(string_view filename)
{
	fstream      input{ filename };
	vector<long> data;

	for ( long value = 0; input >> value; ) {
		data.emplace_back(value);
	}

	return data;
}

void
process(const vector<long>& values)
{
	const long year = 2020;

	if (accumulate(values.begin(), values.end(), 0L) == year) {
		cout << accumulate(values.begin(), values.end(), 1, multiplies<>()) << endl;
	}
}

void
combination(const vector<long>& values, size_t r)
{
	vector<bool> marker(values.size());

	fill(marker.end() - long(r), marker.end(), true);

	vector<long> subset(r);

	do {
		subset.clear();
		for ( size_t i = 0; i != values.size(); ++i ) {
			if ( marker[i] ) {
				subset.emplace_back(values[i]);
			}
		}
		process(subset);
	} while ( std::next_permutation(marker.begin(), marker.end()) );
}

int
main()
{
	auto values = read_file("data/day01.txt");
	combination(values, 2);
	combination(values, 3);
}
