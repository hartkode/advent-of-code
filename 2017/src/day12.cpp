#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

using namespace std;

namespace {

using graph_type = map<int, set<int>>;

vector<int>
split_to_int(const string& line, const string& delimiters)
{
	vector<int> result;

	size_t start = 0;
	size_t end   = 0;

	while ( (end = line.find_first_of(delimiters, start)) != string::npos ) {
		if ( end != start ) {
			result.emplace_back(stoi(line.substr(start, end - start)));
		}

		start = end + 1;
	}

	if ( start != line.size() ) {
		result.emplace_back(stoi(line.substr(start)));
	}

	return result;
}

graph_type
read_lines(const filesystem::path& filename)
{
	ifstream   file{ filename };
	graph_type result;

	for ( string line; getline(file, line); ) {
		auto parts = split_to_int(line, "<-> ,");

		for ( size_t i = 1; i < parts.size(); ++i ) {
			result[parts[0]].insert(parts[i]);
			result[parts[i]].insert(parts[0]);
		}
	}

	return result;
}

void
bfs_mark(const graph_type& graph, int start, set<int>& seen)
{
	queue<int> queue;

	queue.emplace(start);
	seen.emplace(start);

	while ( !queue.empty() ) {
		int prg = queue.front();
		queue.pop();

		for ( auto link: graph.at(prg) ) {
			if ( !seen.contains(link) ) {
				queue.emplace(link);
				seen.emplace(link);
			}
		}
	}
}

void
part1(const graph_type& graph)
{
	set<int> seen;

	bfs_mark(graph, 0, seen);

	cout << "Part1: " << seen.size() << '\n';
}

void
part2(const graph_type& graph)
{
	set<int> seen;

	int count = 0;

	for ( const auto& [i, _]: graph ) {
		if ( seen.contains(i) ) {
			continue;
		}

		++count;

		bfs_mark(graph, i, seen);
	}
	cout << "Part2: " << count << '\n';
}

} // namespace

int
main()
{
	auto data = read_lines("data/day12.txt");
	part1(data);
	part2(data);
}
