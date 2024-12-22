#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>
using namespace std;

vector<unsigned long>
read_file(string_view filename)
{
	fstream               input{ filename };
	vector<unsigned long> lines;

	for ( unsigned long value{}; input >> value; ) {
		lines.emplace_back(value);
	}

	return lines;
}

unsigned long
step(unsigned long value)
{
	// compiler will optimize these calculations
	value = (value ^ (value * 64)) % 16777216;
	value = (value ^ (value / 32)) % 16777216;
	value = (value ^ (value * 2048)) % 16777216;
	return value;
}

void
part1(const vector<unsigned long>& values)
{
	unsigned long sum = 0;
	for ( auto value: values ) {
		for ( int i = 0; i != 2000; ++i ) {
			value = step(value);
		}
		sum += value;
	}
	cout << sum << endl;
}

void
part2(const vector<unsigned long>& values)
{
	using seq_type = tuple<unsigned long, unsigned long, unsigned long, unsigned long>;

	map<seq_type, unsigned long> total;

	for ( auto value: values ) {
		vector<unsigned long> list{ value % 10 };

		for ( int i = 0; i != 2000; ++i ) {
			value = step(value);
			list.emplace_back(value % 10);
		}

		set<seq_type> seen;

		for ( size_t idx = 4; idx < list.size(); ++idx ) {
			const auto a = list[idx - 4];
			const auto b = list[idx - 3];
			const auto c = list[idx - 2];
			const auto d = list[idx - 1];
			const auto e = list[idx];

			seq_type seq{ b - a, c - b, d - c, e - d };

			if ( seen.contains(seq) ) {
				continue;
			}
			seen.insert(seq);

			total[seq] += e;
		}
	}

	cout << ranges::max_element(total, [](const auto& lhs, const auto& rhs) { return lhs.second < rhs.second; })->second << endl;
}

int
main()
{
	auto data = read_file("data/day22.txt");
	part1(data);
	part2(data);
}
