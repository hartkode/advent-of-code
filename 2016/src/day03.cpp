#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

vector<array<long, 3>>
read_file(const string& filename)
{
	fstream                input{ filename };
	vector<array<long, 3>> result;

	for ( string line; getline(input, line); ) {
		stringstream sstr{ line };
		vector<long> nums{ istream_iterator<long>{ sstr }, {} };

		result.push_back({ nums.at(0), nums.at(1), nums.at(2) });
	}

	return result;
}

bool
is_triangle(array<long, 3> set)
{
	ranges::sort(set);
	return set[0] + set[1] > set[2];
}

void
part1(const vector<array<long, 3>>& data)
{
	cout << ranges::count_if(data, is_triangle) << endl;
}

void
part2(const vector<array<long, 3>>& data)
{
	long sum = 0;
	for ( size_t row = 0; row < data.size(); row += 3 ) {
		sum += is_triangle({ data[row][0], data[row + 1][0], data[row + 2][0] }) ? 1 : 0;
		sum += is_triangle({ data[row][1], data[row + 1][1], data[row + 2][1] }) ? 1 : 0;
		sum += is_triangle({ data[row][2], data[row + 1][2], data[row + 2][2] }) ? 1 : 0;
	}
	cout << sum << endl;
}

int
main()
{
	auto data = read_file("data/day03.txt");
	part1(data);
	part2(data);
}
