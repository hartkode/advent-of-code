#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
using namespace std;

vector<tuple<int, int, int, int>>
read_file(string_view filename)
{
	fstream                           input{ filename };
	vector<tuple<int, int, int, int>> data;

	for ( string line; getline(input, line); ) {
		int a, b, c, d;
		(void) sscanf(line.c_str(), "%d-%d,%d-%d", &a, &b, &c, &d);
		data.emplace_back(a, b, c, d);
	}

	return data;
}

void
part1(const vector<tuple<int, int, int, int>>& data)
{
	auto sum = count_if(begin(data), end(data), [](auto& tuple) {
		const auto [a, b, c, d] = tuple;
		return (a <= c && b >= d) || (c <= a && d >= b);
	});
	cout << sum << endl;
}

void
part2(const vector<tuple<int, int, int, int>>& data)
{
	auto sum = count_if(begin(data), end(data), [](auto& tuple) {
		const auto [a, b, c, d] = tuple;
		return max(a, c) <= min(b, d);
	});
	cout << sum << endl;
}

int
main()
{
	const auto data = read_file("data/day04.txt");
	part1(data);
	part2(data);
}
