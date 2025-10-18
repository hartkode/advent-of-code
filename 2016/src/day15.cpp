#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

using namespace std;

namespace {

struct Disc {
	[[nodiscard]]
	bool zeroAtTime(int time) const
	{
		return (time + num + phase) % positions == 0;
	}

	int num;
	int phase;
	int positions;
};

vector<int>
extractNumbers(const string& text)
{
	static const regex number_regex("\\d+");

	vector<int> numbers;
	smatch      match;

	auto searchStart = text.cbegin();
	while ( regex_search(searchStart, text.cend(), match, number_regex) ) {
		numbers.emplace_back(stoi(match.str()));
		searchStart = match.suffix().first;
	}

	return numbers;
}

vector<Disc>
readFile(const filesystem::path& filename)
{
	ifstream     file{ filename };
	vector<Disc> discs;

	for ( string line; getline(file, line); ) {
		auto numbers = extractNumbers(line);
		discs.emplace_back(numbers.at(0), numbers.at(3), numbers.at(1));
	}

	return discs;
}

void
solve(const vector<Disc>& discs)
{
	for ( int time = 0;; ++time ) {
		auto found = ranges::all_of(discs, [&time](const auto& disc) { return disc.zeroAtTime(time); });
		if ( found ) {
			cout << time << '\n';
			return;
		}
	}
}

void
part1(const vector<Disc>& discs)
{
	solve(discs);
}

void
part2(vector<Disc> discs)
{
	discs.emplace_back(discs.size() + 1, 0, 11);
	solve(discs);
}

} // namespace

int
main()
{
	auto discs = readFile("data/day15.txt");
	part1(discs);
	part2(discs);
}
