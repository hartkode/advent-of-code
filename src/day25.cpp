#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <random>
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
bfs(graph_type graph, const string& source, const string& dest, function<void(const string&, const string&)> visit) // NOLINT
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

auto
random_node(const graph_type& graph)
{
	static random_device dev;
	static mt19937       generator(dev());

	uniform_int_distribution<graph_type::size_type> distribute(0, graph.size());

	auto iter = graph.begin();
	advance(iter, distribute(generator));
	return iter->first;
}

void
part1(graph_type graph)
{
	map<tuple<string, string>, int> frequencies;
	for ( size_t idx = 0; idx != 2; ++idx ) {
		for ( const auto& second: graph ) {
			bfs(graph, random_node(graph), second.first, [&](const string& candidate, const string& neighbour) {
				if ( candidate < neighbour ) {
					frequencies[make_tuple(candidate, neighbour)]++;
				}
				else {
					frequencies[make_tuple(neighbour, candidate)]++;
				}
			});
		}
	}

	vector<tuple<tuple<string, string>, int>> sorted_frequencies{ frequencies.begin(), frequencies.end() };

	sort(sorted_frequencies.begin(), sorted_frequencies.end(), [](const auto& lhs, const auto& rhs) {
		return get<1>(lhs) > get<1>(rhs);
	});

	sorted_frequencies.erase(sorted_frequencies.begin() + 3, sorted_frequencies.end());

	for ( const auto& link: sorted_frequencies ) {
		const auto [left, right] = get<0>(link);

		graph[left].erase(right);
		graph[right].erase(left);
	}

	auto reachable = [&graph](const string& start_node) {
		set<string> nodes;
		bfs(graph, start_node, "", [&nodes](const string& candidate, const string& neighbour) {
			nodes.insert(candidate);
			nodes.insert(neighbour);
		});
		return nodes.size();
	};

	const auto [left, right] = get<0>(sorted_frequencies[0]);

	cout << reachable(left) * reachable(right) << endl;
}

int
main()
{
	auto graph = read_file("data/day25.txt");
	part1(graph);
}
