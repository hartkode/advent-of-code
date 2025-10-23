#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace {

struct Range {
	long long start;
	long long end;
};

vector<Range>
readFile(const filesystem::path& filename)
{
	ifstream      file{ filename };
	vector<Range> ranges;

	for ( string line; getline(file, line); ) {
		stringstream stream{ line };
		char         dash{};
		long long    start{};
		long long    end{};

		stream >> start >> dash >> end;

		ranges.emplace_back(start, end);
	}

	return ranges;
}

void
sortRanges(vector<Range>& ranges)
{
	ranges::sort(ranges, [](const auto& lhs, const auto& rhs) {
		if ( lhs.start != rhs.start ) {
			return lhs.start < rhs.start;
		}
		return lhs.end < rhs.end;
	});
}

void
mergeRanges(vector<Range>& ranges)
{
	if ( ranges.empty() ) {
		return;
	}

	vector<Range> mergedRanges;

	auto current = ranges[0];
	for ( size_t i = 1; i != ranges.size(); ++i ) {
		if ( ranges[i].start <= current.end + 1 ) {
			current.end = max(current.end, ranges[i].end);
		}
		else {
			mergedRanges.emplace_back(current);
			current = ranges[i];
		}
	}
	mergedRanges.emplace_back(current);

	ranges.swap(mergedRanges);
}

long long
findLowestAllowedIP(const vector<Range>& ranges)
{
	if ( ranges.empty() || ranges[0].start > 0 ) {
		return 0;
	}
	return ranges[0].end + 1;
}

long long
countAllowedIPs(const vector<Range>& ranges, long long max_ip)
{
	long long allowed_count = 0;

	for ( size_t i = 0; i < ranges.size() - 1; ++i ) {
		allowed_count += ranges[i + 1].start - ranges[i].end - 1;
	}

	if ( !ranges.empty() && ranges.back().end < max_ip ) {
		allowed_count += max_ip - ranges.back().end;
	}

	if ( ranges.empty() ) {
		allowed_count = max_ip + 1;
	}

	return allowed_count;
}

void
part1(vector<Range> ranges)
{
	sortRanges(ranges);
	mergeRanges(ranges);

	cout << findLowestAllowedIP(ranges) << '\n';
}

void
part2(vector<Range> ranges, long long max_ip = 4294967295)
{
	sortRanges(ranges);
	mergeRanges(ranges);

	cout << countAllowedIPs(ranges, max_ip) << '\n';
}

} // namespace

int
main()
{
	auto ranges = readFile("data/day20.txt");
	part1(ranges);
	part2(ranges);
}
