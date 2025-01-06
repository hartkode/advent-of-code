#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

vector<long>
read_file(string_view filename)
{
	fstream input{ filename };
	return { istream_iterator<long>{ input }, {} };
}

template<typename Process>
void
combination(const vector<long>& values, size_t r, Process process)
{
	vector<bool> marker(values.size());

	fill(marker.end() - ptrdiff_t(r), marker.end(), true);

	vector<long> subset(r);

	do {
		subset.clear();
		for ( size_t i = 0; i != values.size(); ++i ) {
			if ( marker[i] ) {
				subset.emplace_back(values[i]);
			}
		}
		process(subset);
	} while ( std::next_permutation(marker.begin(), marker.end()) );
}

void
solve(const vector<long>& weights, long num)
{
	auto totalWeight = accumulate(weights.begin(), weights.end(), 0L);
	auto groupWeight = totalWeight / num;

	auto min_so_far = numeric_limits<long>::max();

	for ( size_t i = 1; i < weights.size(); ++i ) {
		combination(weights, i, [&](const vector<long>& subset) {
			auto subWeight = accumulate(subset.begin(), subset.end(), 0L);
			if ( subWeight == groupWeight ) {
				auto product = accumulate(subset.begin(), subset.end(), 1L, multiplies<>());
				min_so_far   = min(min_so_far, product);
			}
		});
		if ( min_so_far != numeric_limits<long>::max() ) {
			cout << min_so_far << endl;
			return;
		}
	}
}

void
part1(const vector<long>& weights)
{
	solve(weights, 3);
}

void
part2(const vector<long>& weights)
{
	solve(weights, 4);
}

int
main()
{
	auto weights = read_file("data/day24.txt");
	part1(weights);
	part2(weights);
}
