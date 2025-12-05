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
		if ( regex_search(line, match, range) ) {
			ranges.emplace_back(stol(match[1]), stol(match[2]));
		}
		else if ( regex_search(line, match, single) ) {
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
			if ( number >= start && number <= end ) {
				++count;
				break;
			}
		}
	}
	cout << "Part 1: " << count << '\n';
}

} // namespace

int
main()
{
	const auto [ranges, numbers] = read_file("data/day05.txt");
	part1(ranges, numbers);
}
