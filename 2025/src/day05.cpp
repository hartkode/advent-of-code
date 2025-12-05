#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

namespace {

using Range  = tuple<long, long>;
using Ranges = vector<Range>;

tuple<Ranges, vector<long>>
read_file(const filesystem::path& filename)
{
	static const regex range{ R"(^(\d+)-(\d+)$)" };
	static const regex single{ R"(^(\d+)$)" };

	ifstream file{ filename };

	Ranges       ranges;
	vector<long> numbers;

	for ( string line; getline(file, line); ) {
		smatch match;
		if ( regex_match(line, match, range) ) {
			ranges.emplace_back(stol(match[1]), stol(match[2]));
		}
		else if ( regex_match(line, match, single) ) {
			numbers.emplace_back(stol(match[1]));
		}
	}

	return { ranges, numbers };
}

void
part1(const Ranges& ranges, const vector<long>& numbers)
{
	long count = 0;
	for ( const auto number: numbers ) {
		for ( const auto [start, end]: ranges ) {
			if ( start <= number && number <= end ) {
				++count;
				break;
			}
		}
	}
	cout << "Part 1: " << count << '\n';
}

void
part2(Ranges ranges)
{
	ranges::sort(ranges);

	Ranges merged;
	merged.emplace_back(ranges[0]);

	for ( size_t i = 1; i < ranges.size(); ++i ) {
		auto& [last_start, last_end] = merged.back();
		auto& [curr_start, curr_end] = ranges[i];

		if ( last_end < curr_start ) {
			merged.emplace_back(ranges[i]);
		}
		else if ( last_end < curr_end ) {
			last_end = curr_end;
		}
	}

	long count = 0;
	for ( const auto [start, end]: merged ) {
		count += end - start + 1;
	}
	cout << "Part 2: " << count << '\n';
}

} // namespace

int
main()
{
	const auto [ranges, numbers] = read_file("data/day05.txt");
	part1(ranges, numbers);
	part2(ranges);
}
