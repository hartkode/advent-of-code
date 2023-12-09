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
void
solve_rec_part1(vector<T>& values)
{
	vector<T> differences;

	for ( size_t idx = 1; idx < values.size(); ++idx ) {
		auto difference = values[idx] - values[idx - 1];
		differences.emplace_back(difference);
	}
	auto all_zeros = all_of(differences.begin(), differences.end(), [](long diff) { return diff == 0; });
	if ( !all_zeros) {
		solve_rec_part1(differences);
	}
	else {
		differences.emplace_back(0);
	}
	auto last_value = values.back();
	auto last_difference = differences.back();
	values.emplace_back(last_value + last_difference);
}

void
part1()
{
	auto lines = read_file("data/day09.txt");

	long sum = 0;
	for ( const auto& line: lines ) {
		auto values = read_ints(line);
		solve_rec_part1(values);
		sum += values.back();
	}
	cout << sum << endl;
}

template<typename T>
void
solve_rec_part2(vector<T>& values)
{
	vector<T> differences;

	for ( size_t idx = 1; idx < values.size(); ++idx ) {
		auto difference = values[idx] - values[idx - 1];
		differences.emplace_back(difference);
	}
	auto all_zeros = all_of(differences.begin(), differences.end(), [](long diff) { return diff == 0; });
	if ( !all_zeros) {
		solve_rec_part2(differences);
	}
	else {
		differences.insert(differences.begin(), 0);
	}

	auto first_value = values.front();
	auto first_difference = differences.front();
	values.insert(values.begin(), first_value - first_difference);
}

void
part2()
{
	auto lines = read_file("data/day09.txt");

	long sum = 0;
	for ( const auto& line: lines ) {
		auto values = read_ints(line);
		solve_rec_part2(values);
		sum += values.front();
	}
	cout << sum << endl;
}

int
main()
{
	part1();
	part2();
}
