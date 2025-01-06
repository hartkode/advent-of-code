#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

tuple<long, long>
read_file(string_view filename)
{
	fstream input{ filename };

	vector<long> values;
	for ( string word; input >> word; ) {
		long value{};
		if ( stringstream(word) >> value ) {
			values.push_back(value);
		}
	}
	return { values.at(0), values.at(1) };
}

long
triangle_num(long row, long col)
{
	const auto side = row + col - 1;
	return (side * (side + 1)) / 2 - row;
}

long
powmod(long base, long pow, long mod)
{
	long result = 0;
	for ( result = 1; pow != 0; pow /= 2 ) {
		if ( pow % 2 == 1 ) {
			result = (result * base) % mod;
		}
		base = (base * base) % mod;
	}
	return result;
}

void
part1(long row, long col)
{
	cout << (powmod(252533, triangle_num(row, col), 33554393) * 20151125) % 33554393 << endl;
}

int
main()
{
	const auto [row, col] = read_file("data/day25.txt");
	part1(row, col);
}
