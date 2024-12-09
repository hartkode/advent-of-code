#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
using namespace std;

vector<int>
read_file(string_view filename)
{
	fstream input{ filename };
	string  line;
	int     fid = 0;

	getline(input, line);

	vector<int> data;
	for ( size_t i = 0; i != line.size(); ++i ) {
		vector<int> values(size_t(line.at(i) - '0'), (i % 2 == 0) ? fid++ : -1);
		data.insert(data.end(), values.begin(), values.end());
	}
	return data;
}

void
part1(const vector<int>& data)
{
	vector<int> result;

	auto lhs = data.begin();
	auto rhs = data.end();

	while ( lhs != rhs ) {
		if ( *lhs != -1 ) {
			result.push_back(*lhs);
		}
		else {
			--rhs;
			while ( *rhs == -1 ) {
				--rhs;
			}
			result.push_back(*rhs);
		}
		++lhs;
	}

	long sum = 0;
	for ( size_t i = 0; i != result.size(); ++i ) {
		sum += long(i) * result.at(i);
	}
	cout << sum << endl;
}

int
main()
{
	auto data = read_file("data/day09.txt");
	part1(data);
}
