#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

string
read_file(string_view filename)
{
	fstream input{ filename };
	return { istreambuf_iterator<char>{ input }, istreambuf_iterator<char>{} };
}

vector<string>
split(string_view line, string_view delimiter)
{
	size_t pos_start = 0;
	size_t pos_end   = 0;

	vector<string> res;

	while ( (pos_end = line.find(delimiter, pos_start)) != std::string::npos ) {
		auto token = line.substr(pos_start, pos_end - pos_start);
		pos_start  = pos_end + delimiter.length();

		res.emplace_back(token);
	}

	res.emplace_back(line.substr(pos_start));
	return res;
}

vector<string>
transpose(const vector<string>& input)
{
	vector<string> result(input[0].size());

	for ( const auto& i: input ) {
		for ( size_t j = 0; j < i.size(); j++ ) {
			result[j] += i[j];
		}
	}
	return result;
}

long
find_mirror(const vector<string>& input)
{
	for ( size_t idx = 1; idx < input.size(); ++idx ) {
		bool equal = true;
		for ( size_t cnt = 0; cnt != min(idx, input.size() - idx); ++cnt ) {
			auto cmp_result = (input.at(idx + cnt) == input.at(idx - 1 - cnt));
			if ( !cmp_result ) {
				equal = false;
				break;
			}
		}
		if ( equal ) {
			return long(idx);
		}
	}
	return 0;
}

void
part1()
{
	auto contents = read_file("data/day13.txt");
	auto parts    = split(contents, "\n\n");

	long sum = 0;
	for ( const auto& part: parts ) {
		auto lines = split(part, "\n");

		sum += find_mirror(transpose(lines));
		sum += find_mirror(lines) * 100;
	}
	cout << sum << endl;
}

long
count_differences(string_view a, string_view b) // NOLINT
{
	long errs = 0;
	for (size_t idx = 0; idx != a.size(); ++idx) {
		if (a[idx] != b[idx]) {
			++errs;
		}
	}
	return errs;
}

long
find_mirror2(const vector<string>& input)
{
	for ( size_t idx = 1; idx < input.size(); ++idx ) {
		long errs = 0;
		for ( size_t cnt = 0; cnt != min(idx, input.size() - idx); ++cnt ) {
			errs += count_differences(input.at(idx + cnt), input.at(idx - 1 - cnt));
		}
		if ( errs == 1 ) {
			return long(idx);
		}
	}
	return 0;
}

void
part2()
{
	auto contents = read_file("data/day13.txt");
	auto parts    = split(contents, "\n\n");

	long sum = 0;
	for ( const auto& part: parts ) {
		auto lines = split(part, "\n");

		sum += find_mirror2(transpose(lines));
		sum += find_mirror2(lines) * 100;
	}
	cout << sum << endl;
}

int
main()
{
	// part1();
	part2();
}
