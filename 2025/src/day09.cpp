#include <filesystem>
#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>

using namespace std;

namespace {

using Pos = tuple<long, long>;

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

} // namespace

int
main()
{
	auto data = read_file("data/day09.txt");
	part1(data);
}
