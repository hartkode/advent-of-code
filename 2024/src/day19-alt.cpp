#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <string>
#include <tuple>
#include <vector>
using namespace std;

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

tuple<set<string>, vector<string>>
read_file(string_view filename)
{
	fstream input{ filename };

	vector<string> lines;
	for ( string line; getline(input, line); ) {
		lines.emplace_back(line);
	}

	const auto parts = split(lines[0], ", ");

	return { { parts.begin(), parts.end() }, { lines.begin() + 2, lines.end() } };
}

void
part1(const tuple<set<string>, vector<string>>& data)
{
	const auto& parts = get<0>(data);
	const auto& lines = get<1>(data);

	map<string, bool> cache;

	function<bool(const string&)> match = [&](const string& line) {
		if ( line.empty() ) {
			return true;
		}

		if ( cache.contains(line) ) {
			return cache.at(line);
		}

		for ( const auto& part: parts ) {
			if ( line.starts_with(part) ) {
				if ( match(line.substr(part.size())) ) {
					return cache[line] = true;
				}
			}
		}

		return cache[line] = false;
	};

	cout << ranges::count_if(lines, match) << endl;
}

void
part2(const tuple<set<string>, vector<string>>& data)
{
	const auto& parts = get<0>(data);
	const auto& lines = get<1>(data);

	map<string, long> cache;

	function<long(const string&)> match = [&](const string& line) -> long {
		if ( line.empty() ) {
			return 1;
		}

		if ( cache.contains(line) ) {
			return cache.at(line);
		}

		long counter = 0;
		for ( const auto& part: parts ) {
			if ( line.starts_with(part) ) {
				counter += match(line.substr(part.size()));
			}
		}

		return cache[line] = counter;
	};

	cout << accumulate(lines.begin(), lines.end(), 0L, [&](auto init, const auto& line) { return init + match(line); }) << endl;
}

int
main()
{
	const auto data = read_file("data/day19.txt");
	part1(data);
	part2(data);
}
