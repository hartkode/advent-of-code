#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

vector<string>
split(const string& line, const regex& sep)
{
	return { sregex_token_iterator(line.begin(), line.end(), sep, -1), {} };
}

vector<tuple<vector<string>, long, string>>
read_file(string_view filename)
{
	fstream input{ filename };

	vector<tuple<vector<string>, long, string>> result;

	for ( string line; getline(input, line); ) {
		static const regex sep{ "[\\[\\]-]" };

		auto parts = split(line, sep);

		auto checksum = parts.at(parts.size() - 1);
		parts.pop_back();

		auto selector = stol(parts.at(parts.size() - 1));
		parts.pop_back();

		result.emplace_back(parts, selector, checksum);
	}

	return result;
}

void
part1(const vector<tuple<vector<string>, long, string>>& data)
{
	long sum = 0;
	for ( const auto& [ids, selector, checksum]: data ) {
		map<char, long> counts;
		for ( const auto& id: ids ) {
			for ( const auto chr: id ) {
				++counts[chr];
			}
		}

		set<tuple<long, char>> tops;
		for ( const auto& [chr, count]: counts ) {
			tops.emplace(-count, chr);
		}

		string result;
		for ( const auto& [count, chr]: tops ) {
			result += chr;
		}

		if ( result.starts_with(checksum) ) {
			sum += selector;
		}
	}
	cout << sum << endl;
}

void
part2(const vector<tuple<vector<string>, long, string>>& lines)
{
	auto rot = [](string str, int n) {
		for ( auto& chr: str ) {
			chr = ((chr - 'a' + n) % 26) + 'a';
		}
		return str;
	};

	auto is_northpole = [&](const string& str, int n) {
		return rot(str, n) == "northpole";
	};

	for ( const auto& line: lines ) {
		const auto& ids      = get<0>(line);
		const auto  selector = get<1>(line);
		if ( ranges::any_of(ids, [&](auto& str) { return is_northpole(str, (int) selector); }) ) {
			cout << selector << endl;
			return;
		}
	}
}

int
main()
{
	auto data = read_file("data/day04.txt");
	part1(data);
	part2(data);
}
