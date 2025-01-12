// Standard C++
#include <array>
#include <fstream>
#include <iostream>
#include <string>

// Standard C
#include <cstdlib> // memcmp

// System
#include <md5.h>

using namespace std;

void
part1(string_view puzzle)
{
	array<char, MD5_DIGEST_STRING_LENGTH> digest{};

	string result;

	string input;
	for ( unsigned long counter = 0;; ++counter ) {
		input = puzzle;
		input += to_string(counter);
		MD5Data(input.data(), input.size(), digest.data());

		if ( memcmp(digest.data(), "00000", 5) == 0 ) {
			result += digest.at(5);
			if ( result.length() == 8 ) {
				cout << result << endl;
				return;
			}
		}
	}
}

void
part2(string_view puzzle)
{
	array<char, MD5_DIGEST_STRING_LENGTH> digest{};

	string result = "________";

	string input;
	for ( unsigned long counter = 0;; ++counter ) {
		input = puzzle;
		input += to_string(counter);
		MD5Data(input.data(), input.size(), digest.data());

		if ( memcmp(digest.data(), "00000", 5) == 0 ) {
			auto position = digest.at(5) - '0';
			if ( position < 0 || position > 7 ) {
				continue;
			}
			if ( result.at(size_t(position)) != '_' ) {
				continue;
			}
			result.at(size_t(position)) = digest.at(6);
			if ( result.find('_') == string::npos ) {
				cout << result << endl;
				return;
			}
		}
	}
}

int
main()
{
	auto puzzle = "reyedfim"s;
	part1(puzzle);
	part2(puzzle);
}
