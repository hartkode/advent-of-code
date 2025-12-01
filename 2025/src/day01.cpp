#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace {

vector<string>
read_file(const filesystem::path& filename)
{
	ifstream       file{ filename };
	vector<string> lines;

	for ( string line; getline(file, line); ) {
		lines.emplace_back(line);
	}

	return lines;
}

void
part1(const vector<string>& lines)
{
	auto zeros = 0;
	auto pos   = 50;

	for ( const auto& line: lines ) {
		const auto value = stoi(line.substr(1)) % 100;
		pos += (line[0] == 'L') ? (100 - value) : value;
		if ( pos % 100 == 0 ) {
			++zeros;
		}
	}
	cout << "Part 1: " << zeros << '\n';
}

void
part2(const vector<string>& lines)
{
	auto zeros = 0;
	auto pos   = 50;

	for ( const auto& line: lines ) {
		const auto delta = (line[0] == 'L') ? 99 : 1;
		for ( auto value = stoi(line.substr(1)); value > 0; --value ) {
			pos += delta;
			if ( pos % 100 == 0 ) {
				++zeros;
			}
		}
	}
	cout << "Part 2: " << zeros << '\n';
}

void
part2_opti(const vector<string>& lines)
{
	auto zeros = 0;
	auto pos   = 50;

	auto is_zero = [&pos] {
		return pos % 100 == 0;
	};

	for ( const auto& line: lines ) {
		const auto delta = (line[0] == 'L') ? 99 : 1;
		auto       value = stoi(line.substr(1));

		if ( !is_zero() ) {
			for ( ; value > 0 && !is_zero(); --value ) {
				pos += delta;
			}

			if ( is_zero() ) {
				++zeros;
			}
		}

		zeros += value / 100;
		value %= 100;

		for ( ; value > 0; --value ) {
			pos += delta;
			if ( is_zero() ) {
				++zeros;
			}
		}
	}
	cout << "Part2 (Opti): " << zeros << '\n';
}

} // namespace

int
main()
{
	auto data = read_file("data/day01.txt");
	part1(data);
	part2(data);
	part2_opti(data);
}
