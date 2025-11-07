#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <vector>

using namespace std;

namespace {

vector<string>
split(const string& line, const string& delimiter)
{
	vector<string> result;

	size_t start = 0;
	size_t end   = 0;

	while ( (end = line.find(delimiter, start)) != string::npos ) {
		if ( end != start ) {
			result.emplace_back(line.substr(start, end - start));
		}

		start = end + delimiter.length();
	}

	if ( start != line.size() ) {
		result.emplace_back(line.substr(start));
	}

	return result;
}

vector<vector<vector<string>>>
read_file(const filesystem::path& filename)
{
	ifstream file{ filename };
	string   content{ istreambuf_iterator<char>{ file }, {} };

	vector<vector<vector<string>>> result;

	auto blocks = split(content, "\n\n");
	for ( const auto& block: blocks ) {
		vector<vector<string>> data;

		auto lines = split(block, "\n");
		for ( auto line: lines ) {
			line.pop_back();
			auto words = split(line, " ");
			data.emplace_back(words);
		}

		result.emplace_back(data);
	}

	return result;
}

using action_type    = tuple<int, int, string>;       // value (0, 1), direction (-1, 1), next_state)
using condition_type = array<action_type, 2>;         // [0] -> action, [1] -> action
using rule_type      = map<string, condition_type>;   // "A" -> condition
using puzzle_type    = tuple<string, int, rule_type>; // (start_state, steps, rules)

puzzle_type
parse_input(const vector<vector<vector<string>>>& input)
{
	rule_type rules{};

	for ( size_t idx = 1; idx < input.size(); ++idx ) {
		const auto& block = input.at(idx);

		condition_type condition{};

		for ( size_t i = 0; i != 2; ++i ) {
			const auto base = i * 4;

			auto read_val   = stoul(block.at(base + 1).back());
			auto write_val  = stoi(block.at(base + 2).back());
			auto dir        = block.at(base + 3).back() == "left" ? -1 : 1;
			auto next_state = block.at(base + 4).back();

			condition.at(read_val) = { write_val, dir, next_state };
		}

		rules[block.at(0).back()] = condition;
	}

	auto start_state = input[0][0][3];
	auto steps       = stoi(input[0][1][5]);

	return puzzle_type{ start_state, steps, rules };
}

void
part1(const puzzle_type& puzzle)
{
	auto [state, steps, rules] = puzzle;

	map<int, int> tape;

	int pos = 0;

	while ( steps-- > 0 ) {
		auto value                            = tape[pos];
		const auto& [write, move, next_state] = rules.at(state).at(size_t(value));
		state                                 = next_state;
		tape[pos]                             = write;
		pos += move;
	}

	int sum = 0;
	for ( const auto& [key, value]: tape ) {
		sum += value;
	}
	cout << "Part1: " << sum << '\n';
}

} // namespace

int
main()
{
	auto input = read_file("data/day25.txt");
	part1(parse_input(input));
}
