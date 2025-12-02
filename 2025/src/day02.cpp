#include <filesystem>
#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace {

vector<string>
split(const string& line, char sep)
{
	istringstream  strm{ line };
	vector<string> parts;

	for ( string part; getline(strm, part, sep); ) {
		parts.emplace_back(part);
	}
	return parts;
}

vector<tuple<long, long>>
read_file(const filesystem::path& filename)
{
	ifstream file{ filename };
	string   line;
	getline(file, line);

	vector<tuple<long, long>> ranges;
	for ( const auto& part: split(line, ',') ) {
		const auto range = split(part, '-');
		ranges.emplace_back(stol(range.at(0)), stol(range.at(1)));
	}
	return ranges;
}

long
accumulate_invalids(const vector<tuple<long, long>>& ranges, const function<bool(long)>& is_invalid)
{
	long sum = 0;
	for ( const auto [from, to]: ranges ) {
		for ( auto i = from; i <= to; ++i ) {
			if ( is_invalid(i) ) {
				sum += i;
			}
		}
	}
	return sum;
}

void
part1(const vector<tuple<long, long>>& ranges)
{
	auto is_invalid = [](long num) {
		const auto str = to_string(num);
		const auto len = str.length();

		if ( len % 2 == 1 ) {
			return false;
		}

		const auto half = len / 2;

		for ( size_t i = 0; i != half; ++i ) {
			if ( str[i] != str[i + half] ) {
				return false;
			}
		}

		return true;
	};

	cout << "Part 1: " << accumulate_invalids(ranges, is_invalid) << '\n';
}

void
part2(const vector<tuple<long, long>>& ranges)
{
	auto is_invalid = [](long num) {
		const auto str  = to_string(num);
		const auto len  = str.length();
		const auto half = len / 2;

		for ( size_t i = 1; i <= half; ++i ) {
			if ( len % i != 0 ) {
				continue;
			}

			string pattern;
			while ( pattern.length() != len ) {
				pattern += str.substr(0, i);
			}
			if ( pattern == str ) {
				return true;
			}
		}

		return false;
	};

	cout << "Part 2: " << accumulate_invalids(ranges, is_invalid) << '\n';
}

} // namespace

int
main()
{
	auto ranges = read_file("data/day02.txt");
	part1(ranges);
	part2(ranges);
}
