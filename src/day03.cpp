#include <cctype>
#include <fstream>
#include <iostream>
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

bool
is_digit(char chr)
{
	return std::isdigit(static_cast<unsigned char>(chr)) != 0;
}

vector<tuple<size_t, size_t>>
positions(size_t row, size_t col)
{
	return vector<tuple<size_t, size_t>> {
		{ row - 1, col - 1 },
		{ row - 0, col - 1 },
		{ row + 1, col - 1 },

		{ row - 1, col },
		{ row + 1, col },

		{ row - 1, col + 1 },
		{ row - 0, col + 1 },
		{ row + 1, col + 1 },
	};
}

void
part1() // NOLINT
{
	auto lines = read_file("data/day03.txt");

	auto is_symbol = [&lines](size_t row, size_t col) -> bool {
		static const string symbols{ "*#+$%=-@&/" };

		if ( row >= lines.size() || col >= lines[row].size() ) {
			return false;
		}

		auto symbol = lines[row][col];

		return symbols.find(symbol) != string::npos;
	};

	auto bordering = [&is_symbol](size_t row, size_t col) -> bool {
		bool result = false;

		for (const auto& position: positions(row, col)) {
			result |= is_symbol(get<0>(position), get<1>(position));
		}

		return result;
	};

	auto sum = 0;
	for ( size_t row = 0; row != lines.size(); ++row ) {
		const auto& line = lines[row];

		string::size_type start = 0;
		while ( start != line.size() ) {
			while ( start != line.size() && !is_digit(line[start]) ) {
				++start;
			}

			auto end = start;
			while ( end != line.size() && is_digit(line[end]) ) {
				++end;
			}

			if ( start != end ) {
				auto result = false;

				for ( auto col = start; col != end; ++col ) {
					result |= bordering(row, col);
				}

				if ( result ) {
					sum += stoi(line.substr(start, end - start));
				}

				start = end;
			}
		}
	}
	cout << sum << endl;
}

void
part2() // NOLINT
{
	auto lines = read_file("data/day03.txt");

	auto find_integer = [](vector<string>& lines, size_t row, size_t col, int& value) -> bool { // NOLINT
		if ( row >= lines.size() || col >= lines[row].size() ) {
			return false;
		}

		auto& line = lines[row];

		auto digit = line[col];

		if ( !is_digit(digit) ) {
			return false;
		}

		// Anfang der Zahl suchen
		auto start = col;
		for ( ; start != 0 && is_digit(line[start - 1]); --start ) // NOLINT
			;

		auto end = col;
		for ( ; end != line.size() && is_digit(line[end]); ++end ) // NOLINT
			;

		value = stoi(line.substr(start, end - start));

		for ( ; start != end; ++start ) {
			line[start] = '.';
		}

		return true;
	};

	auto sum = 0;
	for ( size_t row = 0; row != lines.size(); ++row ) {
		const auto& line = lines[row];

		auto col = line.find('*');
		while ( col != string::npos ) {
			vector<int> values;

			auto lines_copy{ lines };

			for ( const auto& position: positions(row, col) ) {
				auto value = 0;

				if ( find_integer(lines_copy, get<0>(position), get<1>(position), value) ) {
					values.emplace_back(value);
				}
			}

			if ( values.size() == 2 ) {
				auto ratio = values[0] * values[1];
				sum += ratio;
			}

			col = line.find('*', col + 1);
		}
	}
	cout << sum << endl;
}

int
main()
{
	part1();
	part2();
}
