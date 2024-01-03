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
	vector<bool> v(values.size());
	fill(v.end() - long(r), v.end(), true);
	vector<long> set(r);
	do {
		set.clear();
		for ( size_t i = 0; i != values.size(); ++i ) {
			if ( v[i] ) {
				set.emplace_back(values[i]);
			}
		}
		process(set);
	} while ( std::next_permutation(v.begin(), v.end()) );
}

int
main()
{
	auto values = read_file("data/day01-sample1.txt");
	combination(values, 2);
	combination(values, 3);
}
