#include <filesystem>
#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>

// Geos
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Polygon.h>

using namespace std;
using namespace geos;
using namespace geos::geom;

namespace {

using Pos = tuple<long, long>; // (x, y)

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

std::unique_ptr<Polygon>
makePolygon(const std::vector<Pos>& pts)
{
	CoordinateSequence coords;
	for ( const auto [x, y]: pts ) {
		coords.add(static_cast<double>(x), static_cast<double>(y));
	}

	// polygon schließen
	coords.add(static_cast<double>(get<0>(pts[0])), static_cast<double>(get<1>(pts[0])));

	const auto* factory = GeometryFactory::getDefaultInstance();

	auto shell = factory->createLinearRing(coords);

	return factory->createPolygon(std::move(shell), {});
}

void
part2_geos(const vector<Pos>& data)
{
	auto poly = makePolygon(data);

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

			auto rect = makePolygon(vector<Pos>{
			    make_tuple(left, top),
			    make_tuple(right, top),
			    make_tuple(right, bottom),
			    make_tuple(left, bottom),
			    make_tuple(left, top),
			});

			if ( poly->covers(rect.get()) ) {
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
	part2_geos(data);
}
