#include <fstream>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <tuple>
#include <vector>
using namespace std;

using instructions_t = vector<tuple<size_t, size_t, size_t>>;
using stacks_t       = map<size_t, stack<char>>;

void
read_file(string_view filename, instructions_t& instructions, stacks_t& stacks)
{
	fstream        input{ filename };
	vector<string> lines;

	instructions.clear();

	for ( string line; getline(input, line); ) {
		if ( line.length() == 0 ) {
			continue;
		}
		else if ( line.starts_with("move") ) {
			size_t a, b, c;
			sscanf(line.c_str(), "move %zu from %zu to %zu", &a, &b, &c);
			instructions.emplace_back(a, b, c);
		}
		else if ( line.find('[') != string::npos ) {
			lines.emplace_back(line);
		}
	}

	reverse(begin(lines), end(lines));
	stacks.clear();

	for ( const auto& line: lines ) {
		for ( size_t col = 0; col * 4 < line.length(); ++col ) {
			auto chr = line[col * 4 + 1];
			if ( isalpha(chr) != 0 ) {
				stacks[col + 1].push(chr);
			}
		}
	}
}

void
part1(const instructions_t& instructions, stacks_t stacks)
{
	for ( const auto& [amount, from, to]: instructions ) {
		for ( size_t i = 0; i != amount; ++i ) {
			const auto value = stacks[from].top();
			stacks[from].pop();
			stacks[to].emplace(value);
		}
	}
	for ( size_t i = 0; i != stacks.size(); ++i ) {
		cout << stacks[i + 1].top();
	}
	cout << endl;
}

void
part2(const instructions_t& instructions, stacks_t stacks)
{
	for ( const auto& [amount, from, to]: instructions ) {
		vector<char> values;

		for ( size_t i = 0; i != amount; ++i ) {
			const auto value = stacks[from].top();
			stacks[from].pop();
			values.emplace_back(value);
		}

		reverse(begin(values), end(values));

		for ( const auto& value: values ) {
			stacks[to].emplace(value);
		}
	}
	for ( size_t i = 0; i != stacks.size(); ++i ) {
		cout << stacks[i + 1].top();
	}
	cout << endl;
}

int
main()
{
	instructions_t instructions;
	stacks_t       stacks;

	read_file("data/day05.txt", instructions, stacks);
	part1(instructions, stacks);
	part2(instructions, stacks);
}
