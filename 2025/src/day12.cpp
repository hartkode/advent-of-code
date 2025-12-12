#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <vector>

using namespace std;

namespace {

using Region = tuple<long, long, array<long, 6>>;

tuple<map<long, long>, vector<Region>>
read_file(const filesystem::path& filename)
{
	ifstream       file{ filename };
	vector<string> lines;

	for ( string line; getline(file, line); ) {
		lines.emplace_back(line);
	}

	map<long, long> areas;

	size_t idx = 0;
	for ( ;; idx += 5 ) {
		if ( lines.at(idx).at(1) != ':' ) {
			break;
		}

		const long num = lines.at(idx).at(0) - '0';

		long count = 0;
		for ( size_t j = idx + 1; j != idx + 4; ++j ) {
			count += ranges::count(lines.at(j), '#');
		}

		areas[num] = count;
	}

	vector<Region> regions;
	for ( ; idx != lines.size(); ++idx ) {
		static const regex rgx(R"(^(\d+)x(\d+):\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)$)");

		smatch match;
		if ( !regex_match(lines.at(idx), match, rgx) ) {
			continue;
		}

		regions.emplace_back(
		    stol(match[1]), // width
		    stol(match[2]), // height
		    array<long, 6>{
		        stol(match[3]),
		        stol(match[4]),
		        stol(match[5]),
		        stol(match[6]),
		        stol(match[7]),
		        stol(match[8]) } // counts
		);
	}

	return { areas, regions };
}

void
part1(const map<long, long>& areas, const vector<Region>& regions)
{
	const auto result = ranges::count_if(regions, [&](const auto& region) {
		const auto& [width, height, counts] = region;

		long required = 0;
		for ( size_t i = 0; i != counts.size(); ++i ) {
			required += counts.at(i) * areas.at(static_cast<long>(i));
		}
		return required < width * height;
	});

	cout << "Part 1: " << result << '\n';
}

} // namespace

int
main()
{
	const auto [areas, regions] = read_file("data/day12.txt");
	part1(areas, regions);
}
