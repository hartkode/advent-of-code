#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>
using namespace std;

template<typename T = long>
vector<T>
read_ints(const string& line)
{
	stringstream iss{ line };
	return vector<T>{ istream_iterator<T>{ iss }, istream_iterator<T>{} };
}

long
solve(long time, long winningDistance)
{
	long counter = 0;
	for ( long pushTime = 0; pushTime < time; ++pushTime ) {
		auto distance = (time * pushTime - pushTime * pushTime);
		counter += long(distance > winningDistance);
	}
	return counter;
}

void
part1()
{
	fstream input{ "data/day06.txt" };
	string  line;

	getline(input, line);
	auto times = read_ints(line.substr(line.find(':') + 1));

	getline(input, line);
	auto distances = read_ints(line.substr(line.find(':') + 1));

	long result = 1;
	for ( size_t idx = 0; idx != times.size(); ++idx ) {
		result *= solve(times[idx], distances[idx]);
	}
	cout << result << endl;
}

string
join(const string& line)
{
	stringstream iss{ line };
	return accumulate(istream_iterator<string>{ iss }, istream_iterator<string>{}, string{});
}

void
part2()
{
	fstream input{ "data/day06.txt" };
	string  line;

	getline(input, line);
	auto time = stol(join(line.substr(line.find(':') + 1)));

	getline(input, line);
	auto distance = stol(join(line.substr(line.find(':') + 1)));

	cout << solve(time, distance) << endl;
}

int
main()
{
	part1();
	part2();
}
