#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <tuple>
#include <vector>
#include <ranges>
using namespace std;

vector<tuple<long, long>>
read_file(string_view filename)
{
	fstream                   input{ filename };
	vector<tuple<long, long>> data;

	long lhs = 0;
	long rhs = 0;

	while ( input >> lhs >> rhs ) {
		data.emplace_back(lhs, rhs);
	}

	return data;
}

void
part1(const vector<tuple<long, long>>& data)
{
	vector<long> lefts;
	vector<long> rights;

	for ( const auto& [lhs, rhs]: data ) {
		lefts.emplace_back(lhs);
		rights.emplace_back(rhs);
	}

	ranges::sort(lefts);
	ranges::sort(rights);

	long total_distance = 0;
	for ( size_t i = 0; i != lefts.size(); ++i ) {
		total_distance += abs(lefts[i] - rights[i]);
	}
	cout << total_distance << endl;
}

void
part2(const vector<tuple<long, long>>& data)
{
	vector<long>    lefts;
	map<long, long> rights;

	for ( const auto& [lhs, rhs]: data ) {
		lefts.emplace_back(lhs);
		rights[rhs]++;
	}

	auto score = accumulate(lefts.begin(), lefts.end(), 0, [&](auto init, auto lhs) {
		return init + lhs * rights[lhs];
	});
	cout << score << endl;
}

int
main()
{
	auto data = read_file("data/day01.txt");
	part1(data);
	part2(data);
}
