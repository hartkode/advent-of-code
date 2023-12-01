#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int
main()
{
	string  digits{ "0123456789" };
	fstream input{ "data/01a.txt" };

	int sum = 0;
	for ( string line; input >> line; ) {
		auto first = find_first_of(begin(line), end(line), begin(digits), end(digits));
		auto last  = find_first_of(rbegin(line), rend(line), begin(digits), end(digits));

		auto value = (*first - '0') * 10 + (*last - '0');

		sum += value;
	}
	cout << sum << endl;
}
