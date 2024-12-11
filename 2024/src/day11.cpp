#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
using namespace std;

vector<long>
read_file(string_view filename)
{
	fstream input{ filename };
	return { istream_iterator<long>{ input }, {} };
}

void
part1(vector<long> data)
{
	for ( size_t i = 0; i != 25; ++i ) {
		vector<long> temp;
		for ( const auto value: data ) {
			if ( value == 0 ) {
				temp.push_back(1);
				continue;
			}

			auto str = to_string(value);
			if ( str.size() % 2 == 0 ) {
				auto len = str.size() / 2;
				temp.push_back(stol(str.substr(0, len)));
				temp.push_back(stol(str.substr(len)));
				continue;
			}

			temp.push_back(value * 2024);
		}
		data.swap(temp);
	}
	cout << data.size() << endl;
}

int
main()
{
	auto data = read_file("data/day11.txt");
	part1(data);
}
