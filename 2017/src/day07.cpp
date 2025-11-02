#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <vector>

using namespace std;

namespace {

struct Node {
	int              weight;
	optional<string> parent;
	vector<string>   children;
};

vector<string>
split(const string& line, const string& delimiters)
{
	vector<string> result;

	size_t start = 0;
	size_t end   = 0;

	while ( (end = line.find_first_of(delimiters, start)) != string::npos ) {
		if ( end != start ) {
			result.emplace_back(line.substr(start, end - start));
		}

		start = end + 1;
	}

	if ( start != line.size() ) {
		result.emplace_back(line.substr(start));
	}

	return result;
}

map<string, Node>
read_file(const filesystem::path& filename)
{
	ifstream file{ filename };

	map<string, Node> data; // name -> (weight, parent)

	for ( string line; getline(file, line); ) {
		const auto  parts  = split(line, " ()->,");
		const auto& parent = parts.at(0);

		data[parent].weight = stoi(parts.at(1));

		for ( size_t idx = 2; idx != parts.size(); ++idx ) {
			const auto& child  = parts.at(idx);
			data[child].parent = parent;
			data[parent].children.emplace_back(child);
		}
	}

	return data;
}

string
root_node(const map<string, Node>& data)
{
	for ( const auto& [name, node]: data ) {
		if ( !node.parent ) {
			return name;
		}
	}
	throw runtime_error("no root found");
}

void
part1(const map<string, Node>& data)
{
	cout << "Part1: " << root_node(data) << '\n';
}

int
calculate_weight(const map<string, Node>& data, const string& name) // NOLINT
{
	const auto& node = data.at(name);

	if ( node.children.empty() ) {
		return node.weight;
	}

	int total_weight = node.weight;

	map<string, int> child_weights;
	for ( const auto& child: node.children ) {
		auto child_weight = calculate_weight(data, child);
		child_weights.emplace(child, child_weight);
		total_weight += child_weight;
	}

	map<int, vector<string>> grouped;
	for ( const auto& [child, weight]: child_weights ) {
		grouped[weight].emplace_back(child);
	}

	if ( grouped.size() != 1 ) {
		int    wrong_weight{};
		int    correct_weight{};
		string wrong_name;

		for ( const auto& [weight, names]: grouped ) {
			if ( names.size() == 1 ) {
				wrong_weight = weight;
				wrong_name   = names.front();
			}
			else {
				correct_weight = weight;
			}
		}

		const auto& wrong_node = data.at(wrong_name);
		auto        diff       = wrong_weight - correct_weight;

		cout << "Part2: " << wrong_node.weight - diff << '\n';

		exit(0);
	}

	return total_weight;
}

void
part2(const map<string, Node>& data)
{
	auto node = root_node(data);
	calculate_weight(data, node);
}

} // namespace

int
main()
{
	auto data = read_file("data/day07.txt");
	part1(data);
	part2(data);
}
