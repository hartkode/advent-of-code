#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
using namespace std;

vector<tuple<long, vector<long>>>
read_file(string_view filename)
{
	fstream input{ filename };

	vector<tuple<long, vector<long>>> data;
	for ( string line; getline(input, line); ) {
		auto pos = line.find(':');

		stringstream str{ line.substr(pos + 1) };
		data.emplace_back(stol(line.substr(0, pos)), vector<long>{ istream_iterator<long>{ str }, {} });
	}
	return data;
}

void
part1(const vector<tuple<long, vector<long>>>& data)
{
	auto can_evaluated = [](long first, vector<long> values) -> bool {
		set<long> candidates = { first };

		while ( values.size() > 1 ) {
			auto value = values.back();
			values.pop_back();

			set<long> new_candidates;
			for ( const auto candidate: candidates ) {
				if ( candidate % value == 0 ) {
					new_candidates.insert(candidate / value);
				}
				if ( candidate >= value ) {
					new_candidates.insert(candidate - value);
				}
			}

			candidates = new_candidates;
		}

		return values.size() == 1 && candidates.contains(values.back());
	};

	long sum = 0;
	for ( const auto& [first, values]: data ) {
		if ( can_evaluated(first, values) ) {
			sum += first;
		}
	}
	cout << sum << endl;
}

void
part2(const vector<tuple<long, vector<long>>>& data)
{
	auto can_evaluated = [](long first, vector<long> values) -> bool {
		set<long> candidates = { first };

		while ( values.size() > 1 ) {
			auto value = values.back();
			values.pop_back();

			set<long> new_candidates;
			for ( const auto candidate: candidates ) {
				if ( candidate % value == 0 ) {
					new_candidates.insert(candidate / value);
				}
				if ( candidate >= value ) {
					new_candidates.insert(candidate - value);
				}

				if ( candidate > value ) {
					auto value_str     = to_string(value);
					auto candidate_str = to_string(candidate);

					if ( candidate_str.ends_with(value_str) ) {
						candidate_str.erase(candidate_str.length() - value_str.length());
						new_candidates.insert(stol(candidate_str));
					}
				}
			}

			candidates = new_candidates;
		}

		return values.size() == 1 && candidates.contains(values.back());
	};

	long sum = 0;
	for ( const auto& [first, values]: data ) {
		if ( can_evaluated(first, values) ) {
			sum += first;
		}
	}
	cout << sum << endl;
}

int
main()
{
	auto data = read_file("data/day07.txt");

	part1(data);
	part2(data);
}
