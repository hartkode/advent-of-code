#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>
using namespace std;

auto
read_file(string_view filename)
{
	fstream      input{ filename };
	vector<long> records;

	for ( long value{}; input >> value; ) {
		records.emplace_back(value);
	}
	return records;
}

// https://stackoverflow.com/a/1617797
template<typename Iterator>
bool
next_combination(const Iterator first, Iterator k, const Iterator last)
{
	if ( (first == last) || (first == k) || (last == k) )
		return false;
	Iterator i1 = first;
	Iterator i2 = last;
	++i1;
	if ( last == i1 )
		return false;
	i1 = last;
	--i1;
	i1 = k;
	--i2;
	while ( first != i1 ) {
		if ( *--i1 < *i2 ) {
			Iterator j = k;
			while ( !(*i1 < *j) )
				++j;
			std::iter_swap(i1, j);
			++i1;
			++j;
			i2 = k;
			std::rotate(i1, j, last);
			while ( last != j ) {
				++j;
				++i2;
			}
			std::rotate(k, i2, last);
			return true;
		}
	}
	std::rotate(first, k, last);
	return false;
}

void
part1(const vector<long>& values)
{
	vector<size_t> indicies;
	for ( size_t idx = 0; idx != values.size(); ++idx ) {
		indicies.emplace_back(idx);
	}

	long count = 0;
	for ( size_t comp_size = 1; comp_size != values.size(); ++comp_size ) {
		sort(indicies.begin(), indicies.end());

		do {
			long sum = 0;
			for ( size_t idx = 0; idx != comp_size; ++idx ) {
				sum += values[indicies[idx]];
			}
			if ( sum == 150 ) {
				count++;
			}
		} while ( next_combination(indicies.begin(), indicies.begin() + (long) comp_size, indicies.end()) );
	}
	cout << count << endl;
}

void
part2(const vector<long>& values)
{
	vector<size_t> indicies;
	for ( size_t idx = 0; idx != values.size(); ++idx ) {
		indicies.emplace_back(idx);
	}

	long count = 0;
	for ( size_t comp_size = 1; comp_size != values.size(); ++comp_size ) {
		sort(indicies.begin(), indicies.end());

		do {
			long sum = 0;
			for ( size_t idx = 0; idx != comp_size; ++idx ) {
				sum += values[indicies[idx]];
			}
			if ( sum == 150 ) {
				count++;
			}
		} while ( next_combination(indicies.begin(), indicies.begin() + (long) comp_size, indicies.end()) );

		if ( count != 0 ) {
			cout << count << endl;
			return;
		}
	}
}

int
main()
{
	auto records = read_file("data/day17.txt");
	part1(records);
	part2(records);
}
