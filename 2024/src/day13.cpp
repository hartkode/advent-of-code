#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <regex>
#include <vector>
using namespace std;

vector<string>
split(string_view line, string_view delimiter)
{
	string::size_type pos_start = 0;
	string::size_type pos_end   = 0;

	vector<string> res;
	while ( (pos_end = line.find(delimiter, pos_start)) != string::npos ) {
		res.emplace_back(line.substr(pos_start, pos_end - pos_start));

		pos_start = pos_end + delimiter.length();
	}
	if ( pos_start != line.size() ) {
		res.emplace_back(line.substr(pos_start));
	}
	return res;
}

vector<array<long, 6>>
read_file(string_view filename)
{
	static const regex pattern{ R"((\d+).*\+(\d+).*\+(\d+).*\+(\d+).*=(\d+).*=(\d+))" };

	fstream input{ filename };
	string  content{ istreambuf_iterator<char>{ input }, {} };

	auto parts = split(content, "\n\n");

	vector<array<long, 6>> data;
	for ( auto part: parts ) {
		part.erase(std::remove(part.begin(), part.end(), '\n'), part.cend());

		smatch matches;
		if ( regex_search(part, matches, pattern) ) {
			data.push_back({ stoi(matches[1]),
			                 stoi(matches[2]),
			                 stoi(matches[3]),
			                 stoi(matches[4]),
			                 stoi(matches[5]),
			                 stoi(matches[6]) });
		}
	}

	return data;
}

long
test_brute_force(const array<long, 6>& machine)
{
	const auto button_a_x = machine[0];
	const auto button_a_y = machine[1];
	const auto button_b_x = machine[2];
	const auto button_b_y = machine[3];
	const auto price_x    = machine[4];
	const auto price_y    = machine[5];

	auto min_value = numeric_limits<long>::max();
	for ( long i = 0; i != 100; ++i ) {
		for ( long j = 0; j != 100; ++j ) {
			if ( i * button_a_x + j * button_b_x == price_x &&
			     i * button_a_y + j * button_b_y == price_y ) {
				min_value = min(min_value, i * 3 + j);
			}
		}
	}
	return min_value != numeric_limits<long>::max() ? min_value : 0;
}

long
test_algorithmic(const array<long, 6>& machine)
{
	const auto button_a_x = machine[0];
	const auto button_a_y = machine[1];
	const auto button_b_x = machine[2];
	const auto button_b_y = machine[3];
	const auto price_x    = machine[4] + 10000000000000;
	const auto price_y    = machine[5] + 10000000000000;

	const auto i = (price_x * button_b_y - price_y * button_b_x) / (button_a_x * button_b_y - button_a_y * button_b_x);
	const auto j = (price_x - button_a_x * i) / button_b_x;

	if ( i * button_a_x + j * button_b_x == price_x &&
	     i * button_a_y + j * button_b_y == price_y ) {
		return i * 3 + j;
	}
	return 0;
}

void
part1(const vector<array<long, 6>>& machines)
{
	cout << accumulate(machines.begin(), machines.end(), 0L, [](auto init, const auto& machine) {
		return init + test_brute_force(machine);
	}) << endl;
}

void
part2(const vector<array<long, 6>>& machines)
{
	cout << accumulate(machines.begin(), machines.end(), 0L, [](auto init, const auto& machine) {
		return init + test_algorithmic(machine);
	}) << endl;
}

int
main()
{
	auto data = read_file("data/day13.txt");
	part1(data);
	part2(data);
}
