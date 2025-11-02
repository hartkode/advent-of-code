#include <iostream>
#include <vector>

using namespace std;

namespace {

void
part1(size_t steps, size_t iterations)
{
	vector<size_t> buffer{ 0 };
	size_t         pos = 0;

	for ( size_t i = 1; i <= iterations; ++i ) {
		pos = (pos + steps) % i + 1;
		buffer.insert(buffer.begin() + static_cast<vector<size_t>::difference_type>(pos), i);
	}

	cout << "Part1: " << buffer[(pos + 1) % buffer.size()] << '\n';
}

void
part2(size_t steps, size_t iterations)
{
	size_t pos   = 0;
	size_t value = 0;

	for ( size_t i = 1; i <= iterations; ++i ) {
		pos = (pos + steps) % i;
		if ( pos == 0 ) {
			value = i;
		}
		pos = (pos + 1) % (i + 1);
	}

	cout << "Part2: " << value << '\n';
}

} // namespace

int
main()
{
	const auto input = 371;
	part1(input, 2017);
	part2(input, 50'000'000);
}
