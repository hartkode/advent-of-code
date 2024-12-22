#include <cstdlib>
#include <fstream>
#include <iostream>
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

int
main()
{
	auto data = read_file("data/day22.txt");
	part1(data);
}
