#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

enum Operation {
	On,
	Off,
	Toggle
};

using instruction = tuple<Operation, int, int, int, int>;

auto
read_file(string_view filename)
{
	fstream             input{ filename };
	vector<instruction> instructions;

	for ( string line; getline(input, line); ) {
		int x0, y0, x1, y1; // NOLINT

		if ( sscanf(line.c_str(), "turn on %d,%d through %d,%d", &x0, &y0, &x1, &y1) == 4 ) { // NOLINT
			instructions.emplace_back(Operation::On, x0, y0, x1, y1);
		}
		else if ( sscanf(line.c_str(), "toggle %d,%d through %d,%d", &x0, &y0, &x1, &y1) == 4 ) { // NOLINT
			instructions.emplace_back(Operation::Toggle, x0, y0, x1, y1);
		}
		else if ( sscanf(line.c_str(), "turn off %d,%d through %d,%d", &x0, &y0, &x1, &y1) == 4 ) { // NOLINT
			instructions.emplace_back(Operation::Off, x0, y0, x1, y1);
		}
	}

	return instructions;
}

// clang-format off
static void turn_on(int& value) { value = 1; }
static void turn_off(int& value) { value = 0; }
static void toggle(int& value) { value ^= 1; }
// clang-format on

void
part1(const vector<instruction>& instructions)
{
	map<Operation, function<void(int&)>> ops = {
		{ Operation::On, turn_on },
		{ Operation::Toggle, toggle },
		{ Operation::Off, turn_off }
	};

	map<tuple<int, int>, int> lights;

	for ( const auto& [operation, x0, y0, x1, y1]: instructions ) {
		for ( int x = x0; x <= x1; ++x ) {
			for ( int y = y0; y <= y1; ++y ) {
				ops[operation](lights[{ x, y }]);
			}
		}
	}

	auto sum = count_if(begin(lights), end(lights), [](const auto& light) {
		return light.second == 1;
	});

	cout << sum << endl;
}

// clang-format off
static void increase(int& value) { ++value; }
static void decrease(int& value) { if (value > 0) { --value; } }
static void increase_by_2(int& value) { value += 2; }
// clang-format on

void
part2(const vector<instruction>& instructions)
{
	map<Operation, function<void(int&)>> ops = {
		{ Operation::On, increase },
		{ Operation::Toggle, increase_by_2 },
		{ Operation::Off, decrease }
	};

	map<tuple<int, int>, int> lights;

	for ( const auto& [operation, x0, y0, x1, y1]: instructions ) {
		for ( int x = x0; x <= x1; ++x ) {
			for ( int y = y0; y <= y1; ++y ) {
				ops[operation](lights[{ x, y }]);
			}
		}
	}

	auto sum = accumulate(begin(lights), end(lights), 0, [](int value, const auto& light) {
		return value + light.second;
	});

	cout << sum << endl;
}

int
main()
{
	auto instructions = read_file("data/day06.txt");
	part1(instructions);
	part2(instructions);
}
