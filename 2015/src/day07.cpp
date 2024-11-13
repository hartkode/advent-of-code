#include <cinttypes>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

auto
read_file(string_view filename)
{
	fstream             input{ filename };
	map<string, string> deps;

	for ( string line; getline(input, line); ) {
		const auto pos = line.find(" -> ");
		if ( pos == string::npos ) {
			continue;
		}

		deps[line.substr(pos + 4)] = line.substr(0, pos);
	}

	return deps;
}

// NOLINTBEGIN
uint16_t
solve(map<string, string>& deps, const string& dep)
{
	map<string, uint16_t> cache;

	function<uint16_t(const string&)> solve = [&](const string& dep) -> uint16_t {
		if (cache.contains(dep)) {
			return cache[dep];
		}

		uint16_t value = 0;
		if (sscanf(dep.c_str(), "%" SCNu16, &value) == 1) {
			return cache[dep] = value;
		}

		const auto& line = deps[dep];

		static const size_t MAX = 11;
		char lhs[MAX], rhs[MAX];

		if ( sscanf(line.c_str(), "%10s AND %10s", lhs, rhs) == 2 ) {
			value = solve(lhs) & solve(rhs);
		}
		else if ( sscanf(line.c_str(), "%10s OR %10s", lhs, rhs) == 2 ) {
			value = solve(lhs) | solve(rhs);
		}
		else if ( sscanf(line.c_str(), "%10s LSHIFT %10s", lhs, rhs) == 2 ) {
			value = static_cast<uint16_t>(solve(lhs) << solve(rhs));
		}
		else if ( sscanf(line.c_str(), "%10s RSHIFT %10s", lhs, rhs) == 2 ) {
			value = static_cast<uint16_t>(solve(lhs) >> solve(rhs));
		}
		else if ( sscanf(line.c_str(), "NOT %10s", lhs) == 1 ) {
			value = ~solve(lhs);
		}
		else {
			value = solve(line);
		}

		return cache[dep] = value;
	};

	return solve(dep);
}
// NOLINTEND

auto
part1(map<string, string>& deps)
{
	auto result = solve(deps, "a");
	cout << result << endl;
	return result;
}

void
part2(map<string, string>& deps, uint16_t result)
{
	deps["b"] = to_string(result);
	cout << solve(deps, "a") << endl;
}

int
main()
{
	auto deps = read_file("data/day07.txt");
	auto result = part1(deps);
	part2(deps, result);
}
