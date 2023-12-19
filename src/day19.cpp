#include <fstream>
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

	while ( (pos_end = line.find(delimiter, pos_start)) != std::string::npos ) {
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
part1()
{
	const auto input        = split(read_file("data/day19.txt"), "\n\n");
	const auto rules_string = split(input[0], "\n");
	const auto parts        = split(input[1], "\n");

	const regex rules_pattern{ R"((.*)\{(.*),(.*)\})" };
	const regex sub_rules_pattern{ R"((.)(.)(\d*):(.*))" };

	map<string, tuple<vector<tuple<string, string, long, string>>, string>> rules;

	for ( const auto& rule: rules_string ) {
		smatch smatch;
		if ( regex_search(rule, smatch, rules_pattern) ) {
			string rule_name    = smatch[1];
			string default_dest = smatch[3];

			vector<tuple<string, string, long, string>> foo;

			for ( const auto& sub_rule: split(smatch[2].str(), ",") ) {
				std::smatch smatch2;
				if ( regex_search(sub_rule, smatch2, sub_rules_pattern) ) {
					foo.emplace_back(smatch2[1], smatch2[2], stol(smatch2[3]), smatch2[4]);
				}
			}

			rules[rule_name] = make_tuple(foo, default_dest);
		}
	}

	long sum = 0;
	const regex parts_pattern{ R"(\{x=(\d*),m=(\d*),a=(\d*),s=(\d*)\})" };
	for ( const auto& part: parts ) {
		smatch smatch;
		if ( regex_search(part, smatch, parts_pattern) ) {
			const auto xval = stol(smatch[1]);
			const auto mval = stol(smatch[2]);
			const auto aval = stol(smatch[3]);
			const auto sval = stol(smatch[4]);

			string rule = "in";
			while ( rule != "A" && rule != "R" ) {
				auto sub_rules = get<0>(rules[rule]);
				auto next_rule = get<1>(rules[rule]);

				for ( auto sub_rule: sub_rules ) {
					const auto field = get<0>(sub_rule);
					const auto cmp   = get<1>(sub_rule);
					const auto value = get<2>(sub_rule);

					if ( field == "x" && ((cmp == ">" && xval > value) || (cmp == "<" && xval < value)) ) {
						next_rule = get<3>(sub_rule);
						break;
					}
					if ( field == "m" && ((cmp == ">" && mval > value) || (cmp == "<" && mval < value)) ) {
						next_rule = get<3>(sub_rule);
						break;
					}
					if ( field == "a" && ((cmp == ">" && aval > value) || (cmp == "<" && aval < value)) ) {
						next_rule = get<3>(sub_rule);
						break;
					}
					if ( field == "s" && ((cmp == ">" && sval > value) || (cmp == "<" && sval < value)) ) {
						next_rule = get<3>(sub_rule);
						break;
					}
				}

				rule = next_rule;
			}
			if (rule == "A") {
				sum += xval;
				sum += mval;
				sum += aval;
				sum += sval;
			}
		}
	}
	cout << sum << endl;
}

int
main()
{
	part1();
}
