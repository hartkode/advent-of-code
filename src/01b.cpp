#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

int
main()
{
	map<string, int> map{
		{ "0", 0 },
		{ "1", 1 },
		{ "2", 2 },
		{ "3", 3 },
		{ "4", 4 },
		{ "5", 5 },
		{ "6", 6 },
		{ "7", 7 },
		{ "8", 8 },
		{ "9", 9 },
		{ "one", 1 },
		{ "two", 2 },
		{ "three", 3 },
		{ "four", 4 },
		{ "five", 5 },
		{ "six", 6 },
		{ "seven", 7 },
		{ "eight", 8 },
		{ "nine", 9 },
	};

	fstream input{ "data/01a.txt" };

	auto sum = 0;
	for ( string line; input >> line; ) {
		string sub;
		vector<int> digits;

		for ( auto ch: line ) {
			sub += ch;

			for ( const auto& word: map ) {
				if ( sub.ends_with(word.first) ) {
					digits.push_back(word.second);
				}
			}
		}

		auto first = digits.front();
		auto last = digits.back();

		sum += (first * 10) + last;
	}
	cout << sum << endl;
}
