#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
using namespace std;

using pattern_type = vector<string>;

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

tuple<vector<pattern_type>, vector<pattern_type>>
read_file(string_view filename)
{
	fstream    input{ filename };
	const auto patterns{ split(string{ istreambuf_iterator<char>{ input }, {} }, "\n\n") };

	vector<pattern_type> keys;
	vector<pattern_type> locks;

	for ( const auto& pattern: patterns ) {
		const pattern_type& lines = split(pattern, "\n");

		if ( lines[0] == "#####" && lines[6] == "....." ) {
			locks.push_back(lines);
		}
		else if ( lines[0] == "....." && lines[6] == "#####" ) {
			keys.push_back(lines);
		}
	}

	return { keys, locks };
}

bool
fits(const pattern_type& key, const pattern_type& lock)
{
	for ( size_t row = 0; row != key.size(); ++row ) {
		for ( size_t col = 0; col != key[row].size(); ++col ) {
			if ( key[row][col] == '#' && lock[row][col] == '#' ) {
				return false;
			}
		}
	}
	return true;
}

void
part1(const tuple<vector<pattern_type>, vector<pattern_type>>& data)
{
	const auto [keys, locks] = data;

	long count = 0;
	for ( const auto& key: keys ) {
		for ( const auto& lock: locks ) {
			if ( fits(key, lock) ) {
				++count;
			}
		}
	}
	cout << count << endl;
}

int
main()
{
	const auto data = read_file("data/day25.txt");
	part1(data);
}
