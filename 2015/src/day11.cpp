#include <iostream>
#include <regex>
#include <string>

using namespace std;

void
increment(string& str)
{
	for ( auto it = rbegin(str); it != rend(str); ++it ) {
		*it += 1;
		if ( *it > 'z' ) {
			*it = 'a';
			continue;
		}
		return;
	}
}

bool
has_increasing_chars(string_view str)
{
	for ( size_t idx = 2; idx != str.length(); ++idx ) {
		if ( str[idx - 2] + 1 == str[idx - 1] && str[idx - 1] + 1 == str[idx] ) {
			return true;
		}
	}
	return false;
}

bool
contains_no_invalid_char(const string& str)
{
	return str.find_first_of("iol") == string::npos;
}

bool
contains_two_pairs(const string& str)
{
	static const regex regex("(.)\\1.*(.)\\2");

	return regex_search(str, regex);
}

void
part1(string& str)
{
	while ( true ) {
		if ( has_increasing_chars(str) && contains_no_invalid_char(str) && contains_two_pairs(str) ) {
			cout << str << endl;
			break;
		}
		increment(str);
	}
}

int
main()
{
	string str = "hepxcrrq";

	part1(str);

	increment(str);
	part1(str);
}
