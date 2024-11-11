#include <fstream>
#include <iostream>
#include <set>
#include <string>

using namespace std;

auto
read_file(string_view filename)
{
	fstream input{ filename };
	return string{ istreambuf_iterator<char>{ input }, {} };
}

void
part1(string_view directions)
{
	set<tuple<int, int>> positions{};
	int                  xpos = 0;
	int                  ypos = 0;

	positions.emplace(xpos, ypos);
	for ( const auto direction: directions ) {
		switch ( direction ) {
		case '^':
			--ypos;
			break;
		case 'v':
			++ypos;
			break;
		case '<':
			--xpos;
			break;
		case '>':
			++xpos;
			break;
		default:
			break;
		}
		positions.emplace(xpos, ypos);
	}
	cout << positions.size() << endl;
}

void
part2(string_view directions)
{
	set<tuple<int, int>> positions{};

	for ( string_view::size_type start = 0; start != 2; ++start ) {
		int xpos = 0;
		int ypos = 0;

		positions.emplace(xpos, ypos);

		for ( string_view::size_type idx = start; idx < directions.length(); idx += 2 ) {
			const auto direction = directions[idx];

			switch ( direction ) {
			case '^':
				--ypos;
				break;
			case 'v':
				++ypos;
				break;
			case '<':
				--xpos;
				break;
			case '>':
				++xpos;
				break;
			default:
				break;
			}
			positions.emplace(xpos, ypos);
		}
	}
	cout << positions.size() << endl;
}

int
main()
{
	auto directions = read_file("data/day03.txt");
	// part1(">");
	// part1("^>v<");
	// part1("^v^v^v^v^v");
	part1(directions);
	// part2("^v");
	// part2("^>v<");
	// part2("^v^v^v^v^v");
	part2(directions);
}
