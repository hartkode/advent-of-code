#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

namespace {

vector<tuple<long, long>>
read_file(const filesystem::path& filename)
{
	ifstream                  file{ filename };
	vector<tuple<long, long>> data;

	long lhs = 0;
	long rhs = 0;
	char sep = 0;

	while ( file >> lhs >> sep >> rhs ) {
		data.emplace_back(lhs, rhs);
	}
	return data;
}

long
strongest(const vector<tuple<long, long>>& data, long current = 0, const set<size_t>& used = {})
{
	long best_strength = 0;

	for ( size_t i = 0; i != data.size(); ++i ) {
		if ( used.contains(i) ) {
			continue;
		}

		auto [lhs, rhs] = data.at(i);
		if ( lhs == current || rhs == current ) {
			auto next_port = (lhs == current) ? rhs : lhs;
			auto new_used{ used };
			new_used.emplace(i);
			auto strength = lhs + rhs + strongest(data, next_port, new_used);
			best_strength = max(best_strength, strength);
		}
	}

	return best_strength;
}

tuple<long, size_t>
strongest_and_longest(const vector<tuple<long, long>>& data, long current = 0, const set<size_t>& used = {})
{
	long   best_strength = 0;
	size_t best_length   = 0;

	for ( size_t i = 0; i != data.size(); ++i ) {
		if ( used.contains(i) ) {
			continue;
		}

		auto [lhs, rhs] = data.at(i);
		if ( lhs == current || rhs == current ) {
			auto next_port = (lhs == current) ? rhs : lhs;
			auto new_used{ used };
			new_used.emplace(i);
			auto [sub_strength, sub_length] = strongest_and_longest(data, next_port, new_used);

			auto total_strength = sub_strength + lhs + rhs;
			auto total_length   = sub_length + 1;

			if ( total_length > best_length || (total_length == best_length && total_strength > best_strength) ) {
				best_strength = total_strength;
				best_length   = total_length;
			}
		}
	}

	return make_tuple(best_strength, best_length);
}

void
part1(const vector<tuple<long, long>>& data)
{
	cout << "Part1: " << strongest(data) << '\n';
}

void
part2(const vector<tuple<long, long>>& data)
{
	auto [strength, length] = strongest_and_longest(data);
	cout << "Part2: " << strength << '\n';
}

} // namespace

int
main()
{
	auto data = read_file("data/day24.txt");
	part1(data);
	part2(data);
}
