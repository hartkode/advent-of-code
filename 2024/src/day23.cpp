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

int
main()
{
	auto data = read_file("data/day23.txt");
	part1(data);
}
