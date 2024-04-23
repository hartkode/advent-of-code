#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

string
read_file(string_view filename)
{
	fstream input{ filename };
	return { istreambuf_iterator<char>{ input }, {} };
}

vector<string>
split(string_view line, string_view delimiter)
{
	string::size_type pos_start = 0;
	string::size_type pos_end   = 0;

	vector<string> result;

	while ( (pos_end = line.find(delimiter, pos_start)) != string::npos ) {
		result.emplace_back(line.substr(pos_start, pos_end - pos_start));
		pos_start = pos_end + delimiter.length();
	}

	if ( pos_start != line.size() ) {
		result.emplace_back(line.substr(pos_start));
	}

	return result;
}

vector<long>
split(const string& line)
{
	stringstream input{ line };
	return { istream_iterator<long>(input), {} };
}

void
part1(const vector<string>& parts)
{
	long max_so_far = 0;
	for ( const auto& part: parts ) {
		const auto nums = split(part);
		max_so_far      = max(accumulate(begin(nums), end(nums), 0L), max_so_far);
	}
	cout << max_so_far << endl;
}

void
part2(const vector<string>& parts)
{
	vector<long> all;
	for ( const auto& part: parts ) {
		const auto nums = split(part);
		all.emplace_back(accumulate(begin(nums), end(nums), 0L));
	}
	sort(begin(all), end(all), greater<>());
	cout << all[0] + all[1] + all[2] << endl;
}

int
main()
{
	const auto line  = read_file("data/day01.txt");
	const auto parts = split(line, "\n\n");
	part1(parts);
	part2(parts);
}
