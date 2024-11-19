#include <iostream>
#include <unordered_map>
using namespace std;

void
part1(int limit)
{
	unordered_map<int, int> house;

	for ( int i = 1; i < limit / 10; ++i ) {
		for ( int j = i; j < limit / 10; j += i ) {
			house[j] += i * 10;
		}
	}

	for ( int i = 0;; ++i ) {
		if ( house[i] >= limit ) {
			cout << i << endl;
			return;
		}
	}
}

void
part2(int limit)
{
	unordered_map<int, int> house;

	for ( int i = 1; i < limit / 10; ++i ) {
		int visits = 0;
		for ( int j = i; j < limit / 10; j += i ) {
			if ( visits++ < 50 ) {
				house[j] += i * 11;
			}
		}
	}

	for ( int i = 0;; ++i ) {
		if ( house[i] >= limit ) {
			cout << i << endl;
			return;
		}
	}
}

int
main()
{
	static const auto limit = 29000000;
	part1(limit);
	part2(limit);
}
