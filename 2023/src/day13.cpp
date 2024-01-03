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
transpose(const vector<string>& lines)
{
	vector<string> result(lines[0].size());

	for ( const auto& line: lines ) {
		for ( size_t i = 0; i < line.size(); ++i ) {
			result[i] += line[i];
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
			if ( !(input[idx + cnt] == input[idx - 1 - cnt]) ) {
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

long
count_differences(string_view str1, string_view str2)
{
	long diffs = 0;
	for ( size_t idx = 0; idx != str1.size(); ++idx ) {
		diffs += long(str1[idx] != str2[idx]);
	}
	return diffs;
}

long
find_mirror_part2(const vector<string>& input)
{
	for ( size_t idx = 1; idx < input.size(); ++idx ) {
		long errs = 0;
		for ( size_t i = 0; i != min(idx, input.size() - idx); ++i ) {
			errs += count_differences(input[idx + i], input[idx - 1 - i]);
		}
		if ( errs == 1 ) {
			return long(idx);
		}
	}
	return 0;
}

void
solve(const function<long(const vector<string>&)>& find_mirror)
{
	static const long multiplier = 100;

	auto contents = read_file("data/day13.txt");
	auto parts    = split(contents, "\n\n");

	long sum = 0;
	for ( const auto& part: parts ) {
		auto lines = split(part, "\n");

		sum += find_mirror(transpose(lines));
		sum += find_mirror(lines) * multiplier;
	}
	cout << sum << endl;
}

int
main()
{
	solve(find_mirror);
	solve(find_mirror_part2);
}
