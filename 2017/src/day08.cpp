#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace {

vector<string>
split(const string& line, char sep = ' ')
{
	stringstream   strm{ line };
	vector<string> parts;

	for ( string part; getline(strm, part, sep); ) {
		parts.emplace_back(part);
	}

	return parts;
}

vector<vector<string>>
read_file(const filesystem::path& filename)
{
	ifstream               file{ filename };
	vector<vector<string>> data;

	for ( string line; getline(file, line); ) {
		auto parts = split(line);
		data.emplace_back(parts);
	}

	return data;
}

void
compute(const vector<vector<string>>& data, map<string, int>& memory, int& max_value)
{
	auto match = [](int lhs, string_view rel, int rhs) {
		if ( rel == "==" ) {
			return lhs == rhs;
		}
		else if ( rel == "!=" ) {
			return lhs != rhs;
		}
		else if ( rel == "<" ) {
			return lhs < rhs;
		}
		else if ( rel == "<=" ) {
			return lhs <= rhs;
		}
		else if ( rel == ">" ) {
			return lhs > rhs;
		}
		else if ( rel == ">=" ) {
			return lhs >= rhs;
		}

		throw runtime_error("unknown relation operator: "s + string(rel));
	};

	auto execute = [&](const string& reg, string_view opcode, int value) {
		if ( opcode == "inc" ) {
			memory[reg] += value;
		}
		else if ( opcode == "dec" ) {
			memory[reg] -= value;
		}
		else {
			throw runtime_error("unknown opcode: "s + string(opcode));
		}
	};

	for ( const auto& instr: data ) {
		const auto& reg    = instr.at(0);
		const auto& opcode = instr.at(1);
		const auto  value  = stoi(instr.at(2));
		const auto  lhs    = memory[instr.at(4)];
		const auto& rel    = instr.at(5);
		const auto  rhs    = stoi(instr.at(6));

		if ( match(lhs, rel, rhs) ) {
			execute(reg, opcode, value);
			max_value = max(max_value, memory[reg]);
		}
	}
}

void
solve(const vector<vector<string>>& data)
{
	map<string, int> memory;
	auto             max_value = numeric_limits<int>::min();

	compute(data, memory, max_value);

	auto itr = ranges::max_element(memory, [](const auto& lhs, const auto& rhs) { return lhs.second < rhs.second; });
	cout << "Part1: " << itr->second << '\n';
	cout << "Part2: " << max_value << '\n';
}

} // namespace

int
main()
{
	auto data = read_file("data/day08.txt");
	try {
		solve(data);
	}
	catch ( exception& e ) {
		cerr << "Fehler: " << e.what() << '\n';
	}
}
