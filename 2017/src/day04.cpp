#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>

using namespace std;

namespace {

vector<string>
read_lines(const filesystem::path& filename)
{
	ifstream       file{ filename };
	vector<string> lines;

	for ( string line; getline(file, line); ) {
		lines.emplace_back(line);
	}

	return lines;
}

vector<string>
split(const string& line)
{
	stringstream   strm{ line };
	vector<string> words;

	for ( string word; strm >> word; ) {
		words.emplace_back(word);
	}

	return words;
}

void
part1(const vector<string>& lines)
{
	auto num = ranges::count_if(lines, [](const string& line) {
		auto        words = split(line);
		set<string> set{ words.begin(), words.end() };

		return set.size() == words.size();
	});

	cout << "Part1: " << num << '\n';
}

void
part2(const vector<string>& lines)
{
	auto num = ranges::count_if(lines, [](const string& line) {
		auto        words = split(line);
		set<string> set;

		for ( auto word: words ) {
			ranges::sort(word);
			set.insert(word);
		}

		return set.size() == words.size();
	});

	cout << "Part2: " << num << '\n';
}

} // namespace

int
main()
{
	auto lines = read_lines("data/day04.txt");
	part1(lines);
	part2(lines);
}
