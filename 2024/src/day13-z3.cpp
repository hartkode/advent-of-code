#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <regex>
#include <vector>
#include <z3++.h>
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
test_z3(const array<long, 6>& machine, long offset)
{
	auto config  = z3::config();
	auto context = z3::context(config);
	auto solver  = z3::solver(context);

	const auto i = context.int_const("i");
	const auto j = context.int_const("j");

	solver.add(i > 0);
	solver.add(j > 0);

	const auto button_a_x = context.int_val(machine[0]);
	const auto button_a_y = context.int_val(machine[1]);
	const auto button_b_x = context.int_val(machine[2]);
	const auto button_b_y = context.int_val(machine[3]);
	const auto price_x    = context.int_val(machine[4] + offset);
	const auto price_y    = context.int_val(machine[5] + offset);

	solver.add(i * button_a_x + j * button_b_x == price_x);
	solver.add(i * button_a_y + j * button_b_y == price_y);

	if ( solver.check() == z3::sat ) {
		auto model = solver.get_model();
		return model.eval(i).as_int64() * 3 + model.eval(j).as_int64();
	}

	return 0;
}

void
part1(const vector<array<long, 6>>& machines)
{
	cout << accumulate(machines.begin(), machines.end(), 0L, [](auto init, const auto& machine) {
		return init + test_z3(machine, 0);
	}) << endl;
}

void
part2(const vector<array<long, 6>>& machines)
{
	cout << accumulate(machines.begin(), machines.end(), 0L, [](auto init, const auto& machine) {
		return init + test_z3(machine, 10000000000000);
	}) << endl;
}

int
main()
{
	auto data = read_file("data/day13.txt");
	part1(data);
	part2(data);
}
