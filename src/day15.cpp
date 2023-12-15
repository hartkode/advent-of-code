#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

string
read_file(string_view filename)
{
	fstream input{ filename };
	return { istreambuf_iterator<char>{ input }, istreambuf_iterator<char>{} };
}

inline string&
rtrim(string& s, const char* t = " \t\n\r\f\v")
{
	s.erase(s.find_last_not_of(t) + 1);
	return s;
}

vector<string>
split(const string& line, char sep)
{
	vector<string> parts{};
	stringstream   input{ line };

	for ( string part; getline(input, part, sep); ) {
		parts.emplace_back(rtrim(part));
	}

	return parts;
}

unsigned int
calculate_hash(string_view str)
{
	unsigned int value = 0;
	for ( auto chr: str ) {
		value += static_cast<unsigned char>(chr);
		value *= 17;
	}
	return value % 256;
}

void
part1()
{
	const auto line  = read_file("data/day15.txt");
	const auto parts = split(line, ',');
	unsigned long value = 0;
	for ( const auto& part: parts ) {
		value += calculate_hash(part);
	}
	cout << value << endl;
}

int
main()
{
	part1();
}
