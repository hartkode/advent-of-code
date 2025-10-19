#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

namespace {

string
readFile(const filesystem::path& filename)
{
	ifstream file{ filename };
	string   line;
	getline(file, line);
	return line;
}

void
solve(string line, size_t lines)
{
	long count = 0;
	while ( lines-- > 0 ) {
		string current_line = "." + line + ".";

		string new_line;

		for ( size_t i = 1; i < current_line.size() - 1; ++i ) {
			auto next_chr = '.';

			const auto left   = current_line[i - 1];
			const auto middle = current_line[i];
			const auto right  = current_line[i + 1];

			if ( left == '^' && middle == '^' && right == '.' ) {
				next_chr = '^';
			}
			else if ( left == '.' && middle == '^' && right == '^' ) {
				next_chr = '^';
			}
			else if ( left == '^' && middle == '.' && right == '.' ) {
				next_chr = '^';
			}
			else if ( left == '.' && middle == '.' && right == '^' ) {
				next_chr = '^';
			}

			new_line += next_chr;
		}

		count += ranges::count(line, '.');

		line = std::move(new_line);
	}

	cout << count << '\n';
}

void
part1(const string& line)
{
	solve(line, 40);
}

void
part2(const string& line)
{
	solve(line, 400000);
}

} // namespace

int
main()
{
	auto line = readFile("data/day18.txt");
	part1(line);
	part2(line);
}
