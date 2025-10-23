#include <cmath>
#include <iostream>

using namespace std;

namespace {

template<typename T>
T
fn(T N)
    requires std::is_integral_v<T>
{
	return T(1) << static_cast<T>(floor(log2(N)));
}

void
part1(unsigned N)
{
	cout << (2 * (N - fn(N))) + 1 << '\n';
}

void
part2(unsigned N)
{
	auto log_3 = [](double x) { return log(x) / log(3); };

	auto threes = pow(3, floor(log_3(N - 1)));
	if ( N <= 2 * threes ) {
		cout << static_cast<unsigned>(N - threes);
	}
	else {
		cout << static_cast<unsigned>(N - threes + N - (2 * threes));
	}
	cout << '\n';
}

} // namespace

int
main()
{
	const unsigned puzzle = 3017957;
	part1(puzzle);
	part2(puzzle);
}
