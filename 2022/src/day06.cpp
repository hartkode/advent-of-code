#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>
#include <set>
using namespace std;

string
read_file(string_view filename)
{
	fstream input{ filename };
	return { istreambuf_iterator<char>{ input }, {} };
}

void
solve(const string& data, size_t length)
{
	for ( size_t i = 0; i != data.length() - length; ++i ) {
		const auto substr = data.substr(i, length);
		set<char> set(begin(substr), end(substr));
		if (set.size() == length) {
			cout << i + length << endl;
			break;
		}
	}
}

int
main()
{
	const auto data = read_file("data/day06.txt");
	solve(data, 4);
	solve(data, 14);
}
