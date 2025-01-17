#include <fstream>
#include <iostream>

using namespace std;

string
read_file(string_view filename)
{
	fstream input{ filename };
	string  line;
	getline(input, line);
	return line;
}

void
part1(string_view line)
{
	size_t len = 0;
	for ( size_t i = 0; i != line.length(); ) {
		if ( size_t lhs = 0, rhs = 0; sscanf(&line[i], "(%zux%zu)", &lhs, &rhs) == 2 ) {
			i = line.find(')', i) + 1 + lhs;
			len += lhs * rhs;
		}
		else {
			++i;
			++len;
		}
	}
	cout << len << endl;
}

void
part2(string_view line)
{
	function<size_t(string_view)> part2_rec = [&](string_view line) {
		size_t len = 0;
		for ( size_t i = 0; i != line.length(); ) {
			if ( size_t lhs = 0, rhs = 0; sscanf(&line[i], "(%zux%zu)", &lhs, &rhs) == 2 ) {
				i = line.find(')', i) + 1;
				len += part2_rec({ &line[i], lhs }) * rhs;
				i += lhs;
			}
			else {
				++i;
				++len;
			}
		}
		return len;
	};

	cout << part2_rec(line) << endl;
}

int
main()
{
	auto line = read_file("data/day09.txt");
	// part1("ADVENT");
	// part1("A(1x5)BC");
	// part1("(3x3)XYZ");
	// part1("A(2x2)BCD(2x2)EFG");
	// part1("(6x1)(1x3)A");
	// part1("X(8x2)(3x3)ABCY");
	part1(line);
	part2(line);
}
