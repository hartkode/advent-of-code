#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace {

vector<string>
read_line(const filesystem::path& filename)
{
	ifstream       file{ filename };
	vector<string> result;

	for ( string line; getline(file, line, ','); ) {
		result.emplace_back(line);
	}
	return result;
}

vector<string>
split(const string& line, char sep = '/')
{
	stringstream   strm{ line };
	vector<string> result;

	for ( string line; getline(strm, line, sep); ) {
		result.emplace_back(line);
	}
	return result;
}

void
spin(string& line, size_t n)
{
	ranges::reverse(line);
	reverse(line.begin(), line.begin() + static_cast<string::difference_type>(n));
	reverse(line.begin() + static_cast<string::difference_type>(n), line.end());
}

void
exchange(string& line, size_t a, size_t b)
{
	swap(line.at(a), line.at(b));
}

void
partner(string& line, char a, char b)
{
	auto iter_a = ranges::find(line, a);
	auto iter_b = ranges::find(line, b);
	iter_swap(iter_a, iter_b);
}

void
dance(const vector<string>& instructions, string& line)
{
	for ( const auto& instr: instructions ) {
		auto values = split(instr.substr(1));

		switch ( instr.at(0) ) {
		case 's':
			spin(line, stoul(values.at(0)));
			break;
		case 'x':
			exchange(line, stoul(values.at(0)), stoul(values.at(1)));
			break;
		case 'p':
			partner(line, values.at(0).at(0), values.at(1).at(0));
			break;
		default:
			throw runtime_error("inknown command");
		}
	}
}

void
part1(const vector<string>& instructions, string line)
{
	dance(instructions, line);
	cout << "Part1: " << line << '\n';
}

#if defined(WASTE_TIME)
void
part2_bf(const vector<string>& instructions, string line)
{
	for ( long round = 0; round != 1'000'000'000; ++round ) {
		dance(instructions, line);
	}

	cout << "Part2-BF: " << line << '\n';
}
#endif

void
part2(const vector<string>& instructions, string line)
{
	map<string, size_t> seen;
	vector<string>      history;

	size_t cycle_start  = 0;
	size_t cycle_length = 0;

	for ( size_t round = 0;; ++round ) {
		if ( seen.contains(line) ) {
			cycle_start  = seen.at(line);
			cycle_length = round - cycle_start;
			break;
		}

		seen[line] = round;
		history.emplace_back(line);

		dance(instructions, line);
	}

	const size_t total_rounds = 1'000'000'000L;
	const size_t remaining    = (total_rounds - cycle_start) % cycle_length;

	cout << "Part2: " << history.at(cycle_start + remaining) << '\n';
}

void
part2_alt(const vector<string>& instructions, const string& line)
{
	auto slow = line;
	dance(instructions, slow);

	auto fast = line;
	dance(instructions, fast);
	dance(instructions, fast);

	// find cycle
	while ( slow != fast ) {
		dance(instructions, slow);
		dance(instructions, fast);
		dance(instructions, fast);
	}

	// find cycle length
	size_t cycle_length = 1;
	dance(instructions, slow);
	while ( slow != fast ) {
		dance(instructions, slow);
		++cycle_length;
	}

	// process remaining rounds
	const size_t total_rounds = 1'000'000'000L;
	for ( auto remaining = total_rounds % cycle_length; remaining-- > 0; ) {
		dance(instructions, slow);
	}

	cout << "Part2-Alt: " << slow << '\n';
}

} // namespace

int
main()
{
	auto instructions = read_line("data/day16.txt");
	auto line         = "abcdefghijklmnop"s;

	part1(instructions, line);
	part2(instructions, line);
	part2_alt(instructions, line);
}
