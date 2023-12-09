#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <numeric>
using namespace std;

struct Generator {
	explicit Generator(string_view init)
	    : values_(init)
	    , iter_(values_.begin())

	{
	}

	size_t next()
	{
		size_t gen = (*iter_ == 'L') ? 0 : 1;

		++iter_;
		if ( iter_ == values_.end() ) {
			iter_ = values_.begin();
		}

		return gen;
	}

	string           values_;
	string::iterator iter_;
};

void
part1()
{
	fstream input{ "data/day08.txt" };

	string line;
	getline(input, line);

	Generator generator{ line };

	map<string, array<string, 2>> puzzle;

	const regex pattern{ R"((.*) = \((.*), (.*)\))" };
	while ( getline(input, line) ) {
		if ( line.empty() ) {
			continue;
		}

		smatch sub_match;
		if ( regex_search(line, sub_match, pattern) ) {
			puzzle[sub_match[1]] = { sub_match[2], sub_match[3] };
		}
	}

	auto counter = 0;
	for ( string str{ "AAA" }; str != "ZZZ"; str = puzzle[str][generator.next()] ) {
		++counter;
	}
	cout << counter << endl;
}

void
part2()
{
	fstream input{ "data/day08.txt" };

	string firstline;
	getline(input, firstline);

	map<string, array<string, 2>> puzzle;

	const regex pattern{ R"((.*) = \((.*), (.*)\))" };
	for ( string line; getline(input, line); ) {
		if ( line.empty() ) {
			continue;
		}

		smatch sub_match;
		if ( regex_search(line, sub_match, pattern) ) {
			puzzle[sub_match[1]] = { sub_match[2], sub_match[3] };
		}
	}

	auto value = 1L;
	for ( const auto& entry: puzzle ) {
		if ( entry.first[2] != 'A' ) {
			continue;
		}

		Generator generator{ firstline };

		auto counter = 0L;

		for ( string str{ entry.first }; str[2] != 'Z'; str = puzzle[str][generator.next()] ) {
			++counter;
		}

		value = lcm(value, counter);
	}
	cout << value << endl;
}

int
main()
{
	part1();
	part2();
}
