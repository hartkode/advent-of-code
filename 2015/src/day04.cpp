// Standard C++
#include <array>
#include <fstream>
#include <iostream>
#include <string>

// Standard C
#include <cstring>

// System
#include <md5.h>

using namespace std;

void
brute_force(string_view puzzle, size_t length)
{
	array<char, MD5_DIGEST_STRING_LENGTH> digest{};

	string input;
	for ( unsigned long counter = 0;; ++counter ) {
		input = puzzle;
		input += to_string(counter);
		MD5Data(input.data(), input.size(), digest.data());

		if ( memcmp(digest.data(), "00000000000", length) == 0 ) {
			cout << digest.data() << ": " << counter << endl;
			return;
		}
	}
}

void
part1(string_view puzzle)
{
	brute_force(puzzle, 5);
}

void
part2(string_view puzzle)
{
	brute_force(puzzle, 6);
}

int
main()
{
	string puzzle = "yzbqklnj";

	part1(puzzle);
	part2(puzzle);
}
