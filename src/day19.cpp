#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <vector>
using namespace std;

string
read_file(string_view filename)
{
	fstream input{ filename };
	return { istreambuf_iterator<char>{ input }, istreambuf_iterator<char>{} };
}

vector<string>
split(string_view line, string_view delimiter)
{
	size_t pos_start = 0;
	size_t pos_end   = 0;

	vector<string> res;

	while ( (pos_end = line.find(delimiter, pos_start)) != string::npos ) {
		auto token = line.substr(pos_start, pos_end - pos_start);
		pos_start  = pos_end + delimiter.length();

		res.emplace_back(token);
	}

	if ( pos_start != line.size() ) {
		res.emplace_back(line.substr(pos_start));
	}
	return res;
}

void
part1(map<string, tuple<vector<tuple<string, string, long, string>>, string>> rules, const vector<string>& parts)
{
	long sum = 0;
	for ( const auto& part: parts ) {
		static const regex parts_pattern{ R"(\{x=(\d*),m=(\d*),a=(\d*),s=(\d*)\})" };

		smatch smatch;
		if ( !regex_search(part, smatch, parts_pattern) ) {
			continue;
		}

		map<string, long> values = {
			{ "x", stol(smatch[1]) },
			{ "m", stol(smatch[2]) },
			{ "a", stol(smatch[3]) },
			{ "s", stol(smatch[4]) },
		};

		string rule = "in";
		while ( rule != "A" && rule != "R" ) {
			auto [sub_rules, next_rule] = rules[rule];

			for ( const auto& sub_rule: sub_rules ) {
				const auto [field, cmp, value, dest] = sub_rule;

				if ( values.contains(field) && ((cmp == ">" && values[field] > value) || (cmp == "<" && values[field] < value)) ) {
					next_rule = dest;
					break;
				}
			}

			rule = next_rule;
		}

		if ( rule == "A" ) {
			sum += values["x"];
			sum += values["m"];
			sum += values["a"];
			sum += values["s"];
		}
	}
	cout << sum << endl;
}

void
part2(map<string, tuple<vector<tuple<string, string, long, string>>, string>> rules)
{
	function<long(map<string, tuple<long, long>>, string)> count = [&](map<string, tuple<long, long>> ranges, const string& name) -> long {
		if ( name == "R" ) {
			return 0;
		}

		if ( name == "A" ) {
			long result = 1;
			for ( const auto& range: ranges ) {
				const auto [lo, hi] = range.second;
				result *= hi - lo + 1;
			}
			return result;
		}

		const auto [sub_rules, fallback] = rules[name];

		long result = 0;

		bool run_trough = true;
		for ( const auto& [key, cmp, value, target]: sub_rules ) {
			const auto [lo, hi] = ranges[key];
			pair<long, long> T; // NOLINT
			pair<long, long> F; // NOLINT
			if ( cmp == "<" ) {
				T = { lo, min(value - 1, hi) };
				F = { max(value, lo), hi };
			}
			else {
				T = { max(value + 1, lo), hi };
				F = { lo, min(value, hi) };
			}
			if ( T.first <= T.second ) {
				auto copy = ranges;
				copy[key] = T;
				result += count(copy, target);
			}
			if ( F.first <= F.second ) {
				ranges[key] = F;
			}
			else {
				run_trough = false;
				break;
			}
		}
		if ( run_trough ) {
			result += count(ranges, fallback);
		}

		return result;
	};

	cout << count({
	                  { "x", { 1, 4000 } },
	                  { "m", { 1, 4000 } },
	                  { "a", { 1, 4000 } },
	                  { "s", { 1, 4000 } },
	              },
	              "in")
	     << endl;
}

int
main()
{
	const auto input        = split(read_file("data/day19.txt"), "\n\n");
	const auto rules_string = split(input[0], "\n");
	const auto parts        = split(input[1], "\n");

	map<string, tuple<vector<tuple<string, string, long, string>>, string>> rules;

	for ( const auto& rule: rules_string ) {
		static const regex rules_pattern{ R"((.*)\{(.*),(.*)\})" };

		smatch smatch;
		if ( !regex_search(rule, smatch, rules_pattern) ) {
			continue;
		}

		string name         = smatch[1];
		string default_rule = smatch[3];

		vector<tuple<string, string, long, string>> sub_rules;

		for ( const auto& sub_rule: split(smatch[2].str(), ",") ) {
			static const regex sub_rules_pattern{ R"((.)(.)(\d*):(.*))" };

			std::smatch smatch2;
			if ( regex_search(sub_rule, smatch2, sub_rules_pattern) ) {
				sub_rules.emplace_back(smatch2[1], smatch2[2], stol(smatch2[3]), smatch2[4]);
			}
		}

		rules[name] = make_tuple(sub_rules, default_rule);
	}

	part1(rules, parts);
	part2(rules);
}
