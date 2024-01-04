#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
using namespace std;

auto
read_file(string_view filename)
{
	fstream                                 input{ filename };
	vector<tuple<long, long, char, string>> data;

	for ( string line; getline(input, line); ) {
		long min = 0;
		long max = 0;
		char chr = 0;
		char password[100];
		sscanf(line.data(), "%ld-%ld %c: %99s", &min, &max, &chr, password); // NOLINT
		data.emplace_back(min, max, chr, password);
	}

	return data;
}

void
part1(vector<tuple<long, long, char, string>>& data)
{
	long sum = 0;
	for ( const auto& [min, max, chr, password]: data ) {
		const auto amount = count(password.begin(), password.end(), chr);
		sum += static_cast<long>(min <= amount && amount <= max);
	}
	cout << sum << endl;
}

void
part2(vector<tuple<long, long, char, string>>& data)
{
	long sum = 0;
	for ( const auto& [min, max, chr, password]: data ) {
		sum += ((password[size_t(min - 1)] == chr) ^ (password[size_t(max - 1)] == chr));
	}
	cout << sum << endl;
}

int
main()
{
	auto input = read_file("data/day02.txt");
	part1(input);
	part2(input);
}
