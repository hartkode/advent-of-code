#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <numeric>
using namespace std;

auto
read_file(string_view filename)
{
	fstream input{ filename };
	return vector<string>{ istream_iterator<string>{ input }, istream_iterator<string>{} };
}

auto
to_int(string_view str)
{
	auto result = 0U;
	for ( auto chr: str ) {
		result <<= 1U;
		if ( chr == 'B' || chr == 'R' ) {
			result |= 1U;
		}
	}
	return result;
}

void
part1(const vector<string>& lines)
{
	auto value = 0U;
	for ( const auto& line: lines ) {
		value = max(value, to_int(line));
	}
	cout << value << endl;
}

void
part2(const vector<string>& lines)
{
	vector<unsigned int> values;
	transform(lines.begin(), lines.end(), back_inserter(values), to_int);

	sort(values.begin(), values.end());

	for ( size_t idx = 1; idx != values.size(); ++idx ) {
		if ( values[idx - 1] != values[idx] - 1 ) {
			cout << values[idx] - 1 << endl;
			break;
		}
	}
}

int
main()
{
	auto lines = read_file("data/day05.txt");
	part1(lines);
	part2(lines);
}
