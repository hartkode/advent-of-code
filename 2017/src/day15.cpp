#include <iostream>

using namespace std;

namespace {

struct Generator {
	Generator(unsigned long factor, unsigned long prev)
	    : factor{ factor }
	    , prev{ prev }
	{
	}

	unsigned long next()
	{
		prev *= factor;
		prev %= 2147483647;
		return prev;
	}

	unsigned long factor;
	unsigned long prev;
};

void
part1(unsigned long start_a, unsigned long start_b)
{
	Generator GenA(16807, start_a);
	Generator GenB(48271, start_b);

	int count = 0;
	for ( int i = 0; i != 40000000; ++i ) {
		if ( (GenA.next() & 0xFFFF) == (GenB.next() & 0xFFFF) ) {
			++count;
		}
	}
	cout << "Part1: " << count << '\n';
}

void
part2(unsigned long start_a, unsigned long start_b)
{
	Generator GenA(16807, start_a);
	Generator GenB(48271, start_b);

	int count = 0;
	for ( int i = 0; i != 5000000; ++i ) {
		auto value_a = GenA.next();
		while ( value_a % 4 != 0 ) {
			value_a = GenA.next();
		}
		auto value_b = GenB.next();
		while ( value_b % 8 != 0 ) {
			value_b = GenB.next();
		}
		if ( (value_a & 0xFFFF) == (value_b & 0xFFFF) ) {
			++count;
		}
	}
	cout << "Part2: " << count << '\n';
}

} // namespace

int
main()
{
	static const unsigned long start_a = 618;
	static const unsigned long start_b = 814;

	part1(start_a, start_b);
	part2(start_a, start_b);
}
