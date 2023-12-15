#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

string
read_file(string_view filename)
{
	fstream input{ filename };
	return { istreambuf_iterator<char>{ input }, istreambuf_iterator<char>{} };
}

void
rtrim(string& str)
{
	str.erase(find_if(str.rbegin(), str.rend(), [](auto chr) { return !isspace(chr); }).base(), str.end());
}

vector<string>
split(const string& line, char sep)
{
	vector<string> parts{};
	stringstream   input{ line };

	for ( string part; getline(input, part, sep); ) {
		rtrim(part);
		parts.emplace_back(part);
	}

	return parts;
}

unsigned int
calculate_hash(string_view str)
{
	unsigned int value = 0;
	for ( auto chr: str ) {
		value += static_cast<unsigned char>(chr);
		value *= 17;
	}
	return value % 256;
}

void
part1()
{
	const auto parts = split(read_file("data/day15.txt"), ',');
	cout << accumulate(parts.begin(), parts.end(), 0UL, [](auto init, const auto& str) { return init + calculate_hash(str); }) << endl;
}

void
part2()
{
	vector<list<tuple<string, long>>> boxes(256);

	const auto line  = read_file("data/day15.txt");
	const auto parts = split(line, ',');
	for ( const auto& part: parts ) {
		const auto pos  = part.find_first_of("=-");
		const auto lens = part.substr(0, pos);
		auto&      box  = boxes[calculate_hash(lens)];

		if ( part[pos] == '=' ) {
			const auto value = stol(part.substr(pos + 1));

			auto iter = find_if(box.begin(), box.end(), [&lens](const auto& element) { return get<0>(element) == lens; });
			if ( iter == box.end() ) {
				box.emplace_back(lens, value);
			}
			else {
				*iter = { lens, value };
			}
		}
		else {
			box.remove_if([&lens](const auto& element) { return get<0>(element) == lens; });
		}
	}

	long value = 0;
	for ( size_t idx = 0; idx != boxes.size(); ++idx ) {
		const auto& box = boxes[idx];

		long lens_number = 1;
		for ( const auto& lens: box ) {
			value += static_cast<long>(idx + 1) * lens_number * get<1>(lens);
			++lens_number;
		}
	}
	cout << value << endl;
}

int
main()
{
	part1();
	part2();
}
