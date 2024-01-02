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
	stringstream   input{ line };
	vector<string> parts;

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

auto
build_data_set(string_view filename)
{
	vector<unsigned long> values;

	for ( const auto& line: read_file(filename) ) {
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
pass1(const vector<unsigned long>& values)
{
	auto sum = 0U;
	for ( auto value: values ) {
		if ( value != 0 ) {
			sum += (1U << (value - 1));
		}
	}

	cout << sum << endl;
}

void
pass2(const vector<unsigned long>& values)
{
	auto count = 0U;

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
	const auto values = build_data_set("data/day04.txt");
	pass1(values);
	pass2(values);
}
