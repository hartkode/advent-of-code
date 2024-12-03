#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <string>
using namespace std;

string
read_file(string_view filename)
{
	fstream input{ filename };
	return { istreambuf_iterator<char>{ input }, {} };
}

void
part1(const string& data)
{
	static const regex pattern{ R"(mul\((\d{1,3}),(\d{1,3})\))" };

	long sum = 0;
	for ( auto it = sregex_iterator{ data.begin(), data.end(), pattern }; it != sregex_iterator{}; ++it ) {
		sum += stol((*it)[1]) * stol((*it)[2]);
	}
	cout << sum << endl;
}

void
part2(const string& data)
{
	static const regex pattern{ R"(do\(\)|don't\(\)|mul\((\d{1,3}),(\d{1,3})\))" };

	long sum     = 0;
	bool enabled = true;
	for ( auto it = sregex_iterator{ data.begin(), data.end(), pattern }; it != sregex_iterator{}; ++it ) {
		if ( it->str() == "do()" ) {
			enabled = true;
		}
		else if ( it->str() == "don't()" ) {
			enabled = false;
		}
		else if ( enabled ) {
			sum += stol((*it)[1]) * stol((*it)[2]);
		}
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
