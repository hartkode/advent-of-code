#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

vector<vector<long>>
read_file(string_view filename)
{
	fstream              input{ filename };
	vector<vector<long>> lines;

	auto split = [](const string& line) {
		stringstream line_stream{ line };
		vector<long> values;

		for ( long value = 0; line_stream >> value; ) {
			values.emplace_back(value);
		}
		return values;
	};

	for ( string line; getline(input, line); ) {
		lines.emplace_back(split(line));
	}
	return lines;
}

bool
is_safe(const vector<long>& values)
{
	vector<long> diffs;
	for ( size_t i = 1; i < values.size(); ++i ) {
		diffs.emplace_back(values.at(i) - values.at(i - 1));
	}

	auto all_negative = ranges::all_of(diffs, [](auto diff) { return diff >= -3 && diff < 0; });
	auto all_positive = ranges::all_of(diffs, [](auto diff) { return diff <= 3 && diff > 0; });

	return all_positive || all_negative;
}

bool
is_real_safe(const vector<long>& values)
{
	for ( size_t i = 0; i != values.size(); ++i ) {
		auto values_copy = values;
		values_copy.erase(values_copy.begin() + ptrdiff_t(i));
		if ( is_safe(values_copy) ) {
			return true;
		}
	}
	return false;
}

void
part1(const vector<vector<long>>& lines)
{
	cout << ranges::count_if(lines, is_safe) << endl;
}

void
part2(const vector<vector<long>>& lines)
{
	cout << ranges::count_if(lines, is_real_safe) << endl;
}

int
main()
{
	auto lines = read_file("data/day02.txt");
	part1(lines);
	part2(lines);
}
