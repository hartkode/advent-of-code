#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<int>
read_file(string_view filename)
{
	fstream     input{ filename };
	vector<int> cycles{};
	int         reg_x = 1;

	static const string add_instr = "addx ";

	for ( string line; getline(input, line); ) {
		cycles.emplace_back(reg_x);

		if ( line.starts_with(add_instr) ) {
			cycles.emplace_back(reg_x);
			reg_x += stoi(line.substr(add_instr.size()));
		}
	}
	cycles.emplace_back(reg_x);

	return cycles;
}

void
part1(const vector<int>& cycles)
{
	int sum = 0;
	for ( size_t cycle = 19; cycle < cycles.size(); cycle += 40 ) {
		sum += static_cast<int>(cycle + 1) * cycles[cycle];
	}
	cout << sum << endl;
}

int
main()
{
	const auto cycles = read_file("data/day10.txt");
	part1(cycles);
}
