#include <filesystem>
#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>

using namespace std;

namespace {

vector<tuple<unsigned long, unsigned long>>
read_file(const filesystem::path& filename)
{
	ifstream      file{ filename };
	unsigned long depth = 0;
	unsigned long range = 0;
	char          colon = 0;

	vector<tuple<unsigned long, unsigned long>> data;

	while ( file >> depth >> colon >> range ) {
		data.emplace_back(depth, range);
	}

	return data;
}

template<typename T>
T
get_pos(T depth, T range, T time)
{
	const auto period    = 2 * (range - 1);
	const auto remainder = (depth + time) % period;
	return remainder < range ? remainder : period - remainder;
}

void
part1(const vector<tuple<unsigned long, unsigned long>>& data)
{
	unsigned long sum = 0;
	for ( const auto [depth, range]: data ) {
		if ( get_pos(depth, range, 0UL) == 0 ) {
			sum += depth * range;
		}
	}
	cout << "Part1: " << sum << '\n';
}

void
part2(const vector<tuple<unsigned long, unsigned long>>& data)
{
	for ( unsigned long delay = 0;; ++delay ) {
		bool pass = true;

		for ( const auto [depth, range]: data ) {
			if ( get_pos(depth, range, delay) == 0 ) {
				pass = false;
				break;
			}
		}

		if ( pass ) {
			cout << "Part2: " << delay << '\n';
			return;
		}
	}
}

} // namespace

int
main()
{
	auto data = read_file("data/day13.txt");
	part1(data);
	part2(data);
}
