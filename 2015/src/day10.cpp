#include <iostream>
#include <string>

using namespace std;

string
process(string_view str)
{
	string result{};
	size_t pos = 0;

	while ( pos != str.size() ) {
		char   cur   = str[pos];
		size_t count = 0;
		while ( pos != str.size() && cur == str[pos] ) {
			++count;
			++pos;
		}
		result += to_string(count);
		result += cur;
	}
	return result;
}

void
solve(string str, int n)
{
	for (int i = 0; i != n; ++i) {
		str = process(str);
	}
	cout << str.size() << endl;
}

int
main()
{
	solve("1113222113", 40);
	solve("1113222113", 50);
}
