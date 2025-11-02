#include <filesystem>
#include <fstream>
#include <iostream>

using namespace std;

namespace {

vector<int>
read_lines(const filesystem::path& filename)
{
	ifstream    file{ filename };
	vector<int> lines;

	for ( int value{}; file >> value; ) {
		lines.emplace_back(value);
	}

	return lines;
}

void
part1(vector<int> values)
{
	int steps = 0;
	for ( size_t ip = 0; ip < values.size(); ) {
		auto offset = values[ip]++;
		ip += static_cast<size_t>(offset);
		++steps;
	}

	cout << "Part1: " << steps << '\n';
}

void
part2(vector<int> values)
{
	int steps = 0;
	for ( size_t ip = 0; ip < values.size(); ) {
		auto offset = values[ip];
		values[ip] += (offset >= 3) ? -1 : 1;
		ip += static_cast<size_t>(offset);
		++steps;
	}

	cout << "Part2: " << steps << '\n';
}

} // namespace

int
main()
{
	auto values = read_lines("data/day05.txt");
	part1(values);
	part2(values);
}
