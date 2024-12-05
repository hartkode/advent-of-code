#include <fstream>
#include <iostream>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

vector<long>
split(const string& line, char sep = ',')
{
	vector<long> parts;
	stringstream input{ line };

	for ( string part; getline(input, part, sep); ) {
		parts.emplace_back(stol(part));
	}

	return parts;
}

tuple<vector<tuple<long, long>>, vector<vector<long>>>
read_file(string_view filename)
{
	fstream input{ filename };

	vector<tuple<long, long>> rules;
	for ( string line; getline(input, line); ) {
		if ( line.empty() ) {
			break;
		}
		long lhs = 0;
		long rhs = 0;
		sscanf(line.c_str(), "%ld|%ld", &lhs, &rhs); // NOLINT
		rules.emplace_back(lhs, rhs);
	}

	vector<vector<long>> pages;
	for ( string line; getline(input, line); ) {
		pages.emplace_back(split(line));
	}

	return { rules, pages };
}

bool
is_valid(const vector<tuple<long, long>>& rules, const vector<long>& page)
{
	set<long> processed;

	bool invalid = false;
	for ( const auto& num: page ) {
		processed.emplace(num);

		auto pred = [num](const tuple<long, long>& rule) {
			return get<0>(rule) == num;
		};

		for ( auto it = find_if(rules.begin(), rules.end(), pred);
		      it != rules.end();
		      it = find_if(it + 1, rules.end(), pred) ) {
			if ( processed.contains(get<1>(*it)) ) {
				invalid = true;
			}
		}
	}
	return !invalid;
}

void
part1(const vector<tuple<long, long>>& rules, const vector<vector<long>>& pages)
{
	long sum = 0;
	for ( const auto& page: pages ) {
		if ( is_valid(rules, page) ) {
			sum += page.at(page.size() / 2);
		}
	}
	cout << sum << endl;
}

void
part2(const vector<tuple<long, long>>& rules, const vector<vector<long>>& pages)
{
	long sum = 0;
	for ( auto page: pages ) {
		if ( is_valid(rules, page) ) {
			continue;
		}

		ranges::sort(page, [&rules](auto lhs, auto rhs) {
			return ranges::find(rules, make_tuple(lhs, rhs)) != rules.end();
		});

		sum += page.at(page.size() / 2);
	}
	cout << sum << endl;
}

int
main()
{
	auto [rules, pages] = read_file("data/day05.txt");
	part1(rules, pages);
	part2(rules, pages);
}
