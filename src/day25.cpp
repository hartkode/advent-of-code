#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
using namespace std;

using graph_type = map<string, set<string>>;

vector<string>
split(const string& line, char sep)
{
	vector<string> parts;
	stringstream   input{ line };

	for ( string part; getline(input, part, sep); ) {
		parts.emplace_back(part);
	}

	return parts;
}

graph_type
read_file(string_view filename)
{
	fstream    input{ filename };
	graph_type graph;

	for ( string line; getline(input, line); ) {
		const auto components = split(line.erase(line.find(':'), 1), ' ');

		for ( size_t i = 1; i < components.size(); ++i ) {
			graph[components[0]].insert(components[i]);
			graph[components[i]].insert(components[0]);
		}
	}

	return graph;
}

bool
bfs(const graph_type& graph, const string& source, const string& dest, map<string, string>& parents) // NOLINT
{
	set<string> visited;

	parents.clear();

	queue<string> queue;
	queue.emplace(source);

	while ( !queue.empty() ) {
		auto candidate = queue.front();
		queue.pop();

		if ( candidate == dest ) {
			return true;
		}

		for ( const auto& neighbour: graph.at(candidate) ) {
			if ( !visited.contains(neighbour) ) {
				queue.emplace(neighbour);
				parents[neighbour] = candidate;
				visited.emplace(neighbour);
			}
		}
	}

	return false;
}

void
dfs(const graph_type& graph, const string& source, set<string>& visited) // NOLINT
{
	visited.emplace(source);
	for ( const auto& node: graph.at(source) ) {
		if ( !visited.contains(node) ) {
			dfs(graph, node, visited);
		}
	}
}

bool
bfs1(graph_type graph, const string& source, const string& dest, function<void(const string&, const string&)> visit) // NOLINT
{
	set<string> visited;

	queue<string> queue;
	queue.emplace(source);

	while ( !queue.empty() ) {
		auto candidate = queue.front();
		queue.pop();

		if ( candidate == dest ) {
			return true;
		}

		for ( const auto& neighbour: graph[candidate] ) {
			if ( !visited.contains(neighbour) ) {
				queue.emplace(neighbour);

				visit(candidate, neighbour);
				visited.emplace(neighbour);
			}
		}
	}
	return false;
}

void
print_graph_stats(const graph_type& graph)
{
	size_t nodes = 0;
	size_t edges = 0;
	for ( const auto& link: graph ) {
		nodes++;
		edges += link.second.size();
	}
	cout << nodes << ", " << edges / 2 << endl;
}

void
part1(graph_type graph)
{
	srand(unsigned(time(nullptr)));

	vector<string> nodes(graph.size());
	for ( const auto& foo: graph ) {
		nodes.emplace_back(foo.first);
	}

	map<tuple<string, string>, int> counter;
	for ( size_t idx = 0; idx != 2; ++idx ) {
		for ( const auto& second: graph ) {
			bfs1(graph, nodes[size_t(rand()) % nodes.size()], second.first, [&](const string& candidate, const string& neighbour) {
				if ( candidate < neighbour ) {
					counter[make_tuple(candidate, neighbour)]++;
				}
				else {
					counter[make_tuple(neighbour, candidate)]++;
				}
			});
		}
	}

	vector<tuple<tuple<string, string>, int>> all_links{ counter.begin(), counter.end() };

	sort(all_links.begin(), all_links.end(), [](const auto& lhs, const auto& rhs) {
		return get<1>(lhs) > get<1>(rhs);
	});

	for ( size_t i = 0; i != 3; ++i ) {
		const auto [links, num] = all_links[i];
		const auto [lhs, rhs]   = links;

		graph[lhs].erase(rhs);
		graph[rhs].erase(lhs);
	}

	set<string> reachable;
	const auto start_from_a = get<0>(get<0>(all_links[0]));
	bfs1(graph, start_from_a, "--not there--", [&](const string& candidate, const string& neighbour) {
		reachable.insert(candidate);
		reachable.insert(neighbour);
	});

	set<string> reachable2;
	const auto start_from_b = get<1>(get<0>(all_links[0]));
	bfs1(graph, start_from_b, "--not there--", [&](const string& candidate, const string& neighbour) {
		reachable2.insert(candidate);
		reachable2.insert(neighbour);
	});

	cout << reachable.size() * reachable2.size() << endl;
}

void
print_graph(const graph_type& graph)
{
	for ( const auto& node: graph ) {
		cout << node.first << ": ";
		copy(node.second.begin(), node.second.end(), ostream_iterator<string>(cout, ", "));
		cout << endl;
	}
}

graph_type
get_rgraph(const graph_type& graph)
{
	graph_type rgraph;
	for ( const auto& node: graph ) {
		for ( const auto& link: node.second ) {
			rgraph[link].insert(node.first);
		}
	}
	return rgraph;
}

int
main()
{
	auto graph = read_file("data/day25.txt");

	// print_graph(graph);

	part1(graph);

	// print_graph(graph);
	// cout << " ------------ " << endl;
	// print_graph(get_rgraph(graph));
	// cout << " ------------ " << endl;
	// print_graph(get_rgraph(get_rgraph(graph)));

	// map<string, string> parents;

	// if ( bfs(graph, "jqt", "bvb", parents) ) {
	// 	for ( const auto& [lhs, rhs]: parents ) {
	// 		cout << lhs << ": " << rhs << endl;
	// 	}
	// }
}
