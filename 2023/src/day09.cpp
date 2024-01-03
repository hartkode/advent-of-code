#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

vector<string>
read_file(string_view filename)
{
	fstream        input{ filename };
	vector<string> data;

	for ( string line; getline(input, line); ) {
		data.emplace_back(line);
	}

	return data;
}

template<typename T = long>
vector<T>
read_ints(const string& line)
{
	stringstream iss{ line };
	return vector<T>{ istream_iterator<T>{ iss }, istream_iterator<T>{} };
}

template<typename T>
T
solve_rec(const vector<T>& values)
{
	vector<T> differences;

	for ( size_t idx = 1; idx < values.size(); ++idx ) {
		differences.emplace_back(values[idx] - values[idx - 1]);
	}
	auto all_zeros = all_of(differences.begin(), differences.end(), [](T diff) { return diff == 0; });
	if ( !all_zeros ) {
		return solve_rec(differences) + values.back();
	}
	else {
		return values.back();
	}
}

void
part1(const vector<string>& lines)
{
	long sum = 0;
	for ( const auto& line: lines ) {
		sum += solve_rec(read_ints(line));
	}
	cout << sum << endl;
}

void
part2(const vector<string>& lines)
{
	long sum = 0;
	for ( const auto& line: lines ) {
		auto values = read_ints(line);
		reverse(values.begin(), values.end());
		sum += solve_rec(values);
	}
	cout << sum << endl;
}

int
main()
{
	auto lines = read_file("data/day09.txt");
	part1(lines);
	part2(lines);
}
