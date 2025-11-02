#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

using namespace std;

namespace {

string
remove_garbage(string_view line, int& removed)
{
	string result;

	removed = 0;
	for ( size_t pos = 0; pos < line.size(); ) {
		if ( line.at(pos) == '<' ) {
			++pos;
			while ( pos < line.size() && line.at(pos) != '>' ) {
				if ( line.at(pos) == '!' ) {
					pos += 2;
				}
				else {
					++pos;
					++removed;
				}
			}
			++pos;
		}
		else {
			result += line.at(pos);
			++pos;
		}
	}

	return result;
}

string
read_file(const filesystem::path& filename)
{
	ifstream file{ filename };
	return { istreambuf_iterator<char>{ file }, {} };
}

void
solve(string_view data)
{
	int score   = 0;
	int depth   = 0;
	int removed = 0;

	for ( const auto chr: remove_garbage(data, removed) ) {
		if ( chr == '{' ) {
			++depth;
		}
		else if ( chr == '}' ) {
			score += depth;
			--depth;
		}
	}

	cout << "Part1: " << score << '\n';
	cout << "Part2: " << removed << '\n';
}

} // namespace

int
main()
{
	auto data = read_file("data/day09.txt");
	solve(data);
}
