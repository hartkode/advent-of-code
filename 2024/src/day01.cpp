#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>
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

	sort(lefts.begin(), lefts.end());
	sort(rights.begin(), rights.end());

	long total_distance = 0;
	for ( size_t i = 0; i != lefts.size(); ++i ) {
		long distance = abs(lefts[i] - rights[i]);
		total_distance += distance;
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

	long score = 0;
	for ( const auto& lhs: lefts ) {
		score += lhs * rights[lhs];
	}
	cout << score << endl;
}

int
main()
{
	auto data = read_file("data/day01.txt");
	part1(data);
	part2(data);
}
