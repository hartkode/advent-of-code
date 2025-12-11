#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

namespace {

using Graph = map<string, set<string>>;

vector<string>
split(const string& str, const string& delims)
{
	vector<string> result;

	size_t start = str.find_first_not_of(delims);
	while ( start != string::npos ) {
		size_t end = str.find_first_of(delims, start);

		if ( end == string::npos ) {
			// letzter Teil
			result.push_back(str.substr(start));
			break;
		}

		result.push_back(str.substr(start, end - start));

		start = str.find_first_not_of(delims, end);
	}

	return result;
}

Graph
read_file(const filesystem::path& filename)
{
	ifstream file{ filename };

	Graph graph;
	for ( string line; getline(file, line); ) {
		auto parts = split(line, ": ");
		graph[parts[0]].insert(parts.begin() + 1, parts.end());
	}
	return graph;
}

void
part1(const Graph& graph)
{
	map<string, long> cache;

	function<long(const string&)> dfs = [&](const string& src) {
		if ( src == "out" ) {
			return 1L;
		}

		if ( cache.contains(src) ) {
			return cache.at(src);
		}

		long count = 0;

		for ( const auto& node: graph.at(src) ) {
			count += dfs(node);
		}

		return cache[src] = count;
	};

	cout << "Part 1: " << dfs("you") << '\n';
}

} // namespace

int
main()
{
	auto graph = read_file("data/day11.txt");
	part1(graph);
}
