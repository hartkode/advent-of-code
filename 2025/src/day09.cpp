#include <filesystem>
#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>

using namespace std;

namespace {

using Pos     = tuple<long, long>; // (x, y)
using Segment = tuple<Pos, Pos>;   // (p1, p2)

vector<Pos>
read_file(const filesystem::path& filename)
{
	ifstream file{ filename };

	vector<Pos> data;

	long x{};
	long y{};
	char skip{};

	while ( file >> x >> skip >> y ) {
		data.emplace_back(x, y);
	}
	return data;
}

void
part1(const vector<Pos>& data)
{
	auto area = numeric_limits<long>::min();

	for ( size_t i = 0; i != data.size(); ++i ) {
		for ( size_t j = i + 1; j != data.size(); ++j ) {
			const auto [x1, y1] = data[i];
			const auto [x2, y2] = data[j];

			const auto width  = abs(x1 - x2) + 1;
			const auto height = abs(y1 - y2) + 1;

			area = max(area, width * height);
		}
	}
	cout << "Part 1: " << area << '\n';
}

vector<Segment>
build_edges(const vector<Pos>& data)
{
	vector<Segment> edges;
	for ( size_t i = 0; i != data.size(); ++i ) {
		edges.emplace_back(data[i], data[(i + 1) % data.size()]);
	}
	return edges;
}

bool
intersects(const Segment& line, const tuple<long, long, long, long>& rect)
{
	const auto [start, end] = line;
	const auto [x1, y1]     = start;
	const auto [x2, y2]     = end;

	const auto min_x = min(x1, x2);
	const auto max_x = max(x1, x2);
	const auto min_y = min(y1, y2);
	const auto max_y = max(y1, y2);

	const auto [left, top, right, bottom] = rect;

	return max_x > left && min_x < right && max_y > top && min_y < bottom;
}

bool
contains(const vector<Segment>& poly, const tuple<long, long, long, long>& rect)
{
	return ranges::all_of(poly, [&](const auto& line) { return !intersects(line, rect); });
}

void
part2(const vector<Pos>& data)
{
	const auto poly = build_edges(data);

	auto max_area = numeric_limits<long>::min();

	for ( size_t i = 0; i != data.size(); ++i ) {
		for ( size_t j = i + 1; j != data.size(); ++j ) {
			const auto [x1, y1] = data[i];
			const auto [x2, y2] = data[j];

			const auto width  = abs(x1 - x2) + 1;
			const auto height = abs(y1 - y2) + 1;

			const auto area = width * height;

			if ( area <= max_area ) {
				continue;
			}

			const auto left   = min(x1, x2);
			const auto right  = max(x1, x2);
			const auto top    = min(y1, y2);
			const auto bottom = max(y1, y2);

			if ( contains(poly, { left, top, right, bottom }) ) {
				max_area = area;
			}
		}
	}
	cout << "Part 2: " << max_area << '\n';
}

} // namespace

int
main()
{
	auto data = read_file("data/day09.txt");
	part1(data);
	part2(data);
}
