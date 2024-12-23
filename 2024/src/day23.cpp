#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

vector<string>
split(const string& line, char sep = ' ')
{
	vector<string> parts;
	stringstream   input{ line };

	for ( string part; getline(input, part, sep); ) {
		parts.emplace_back(part);
	}

	return parts;
}

template<typename Container>
string
join(const Container& container, const string& delimiter)
{
	ostringstream oss;
	auto          it = container.begin();
	if ( it != container.end() ) {
		oss << *it;
		++it;
	}
	while ( it != container.end() ) {
		oss << delimiter << *it;
		++it;
	}
	return oss.str();
}

map<string, set<string>>
read_file(string_view filename)
{
	fstream                  input{ filename };
	map<string, set<string>> data;

	for ( string line; getline(input, line); ) {
		const auto  parts = split(line, '-');
		const auto& lhs   = parts[0];
		const auto& rhs   = parts[1];
		data[lhs].insert(rhs);
		data[rhs].insert(lhs);
	}

	return data;
}

void
part1(const map<string, set<string>>& nodes)
{
	set<set<string>> connected;

	for ( const auto& [node0, links]: nodes ) {
		for ( const auto& node1: links ) {
			for ( const auto& node2: nodes.at(node1) ) {
				if ( node0 != node2 and nodes.at(node2).contains(node0) ) {
					if ( node0.starts_with('t') || node1.starts_with('t') || node2.starts_with('t') ) {
						connected.insert({ node0, node1, node2 });
					}
				}
			}
		}
	}

	cout << connected.size() << endl;
}

void
bronKerbosch(set<string> R, set<string> P, set<string> X, const map<string, set<string>>& graph, set<set<string>>& maxCliques)
{
	if ( P.empty() && X.empty() ) {
		// Found a maximal clique
		maxCliques.insert(R);
		return;
	}

	// Choose a pivot (heuristic: choose a vertex with maximum connections in P union X)
	string      pivot;
	set<string> unionPX;
	set_union(P.begin(), P.end(), X.begin(), X.end(), inserter(unionPX, unionPX.begin()));

	if ( !unionPX.empty() ) {
		pivot = *unionPX.begin(); // Choose the first vertex as pivot (simple heuristic)
	}

	set<string> candidates;
	set_difference(P.begin(), P.end(), graph.at(pivot).begin(), graph.at(pivot).end(), inserter(candidates, candidates.begin()));

	for ( const string& candidate: candidates ) {
		set<string> newR = R;
		newR.insert(candidate);

		set<string> newP;
		set<string> newX;
		for ( const string& neighbor: graph.at(candidate) ) {
			if ( P.find(neighbor) != P.end() ) {
				newP.insert(neighbor);
			}
			if ( X.find(neighbor) != X.end() ) {
				newX.insert(neighbor);
			}
		}

		bronKerbosch(newR, newP, newX, graph, maxCliques);

		P.erase(candidate);
		X.insert(candidate);
	}
}

set<string>
findLargestSet(const set<set<string>>& sets)
{
	set<string> largest;
	for ( const auto& sub: sets ) {
		if ( sub.size() > largest.size() ) {
			largest = sub;
		}
	}
	return largest;
}

void
part2(const map<string, set<string>>& nodes)
{
	set<string> R; // Initially empty
	set<string> P; // All vertices are potential candidates
	set<string> X; // Initially empty

	for ( const auto& [vertex, _]: nodes ) {
		P.insert(vertex);
	}

	set<set<string>> max_cliques;
	bronKerbosch(R, P, X, nodes, max_cliques);

	cout << join(findLargestSet(max_cliques), ",") << endl;
}

int
main()
{
	auto data = read_file("data/day23.txt");
	part1(data);
	part2(data);
}
