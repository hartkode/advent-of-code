#include <cctype>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

vector<string>
read_file(string_view filename)
{
	fstream        input{ filename };
	vector<string> data;

	for ( string line; getline(input, line); ) {
		data.emplace_back(line);
	}

	return data;
}

vector<string>
split(const string& line, char sep)
{
	vector<string> parts{};
	stringstream   input{ line };

	for ( string part; getline(input, part, sep); ) {
		parts.emplace_back(part);
	}

	return parts;
}

template<typename Container>
Container
read_ints(const string& line)
{
	Container    container{};
	stringstream input{ line };

	for ( typename Container::value_type value; input >> value; ) {
		container.emplace(value);
	}

	return container;
}

vector<unsigned long>
build_data_set()
{
	const auto            lines = read_file("data/day04.txt");
	vector<unsigned long> values{};

	for ( const auto& line: lines ) {
		const auto cards   = split(line, ':');
		const auto numbers = split(cards[1], '|');

		const auto winning_numbers = read_ints<set<int>>(numbers[0]);
		const auto card_numbers    = read_ints<set<int>>(numbers[1]);

		auto count = 0U;
		for ( auto number: winning_numbers ) {
			if ( card_numbers.contains(number) ) {
				++count;
			}
		}
		values.emplace_back(count);
	}

	return values;
}

void
pass1()
{
	auto sum = 0U;
	for ( auto value: build_data_set() ) {
		if ( value != 0 ) {
			sum += (1U << (value - 1));
		}
	}

	cout << sum << endl;
}

void
pass2()
{
	const auto values = build_data_set();
	auto       count  = 0U;

	function<void(size_t, size_t)> process = [&](size_t start, size_t end) -> void {
		for ( ; start != end; ++start ) {
			auto value = values[start];

			++count;

			process(start + 1, start + value + 1);
		}
	};

	process(0, values.size());

	cout << count << endl;
}

int
main()
{
	pass1();
	pass2();
}
