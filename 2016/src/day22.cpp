#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <vector>

using namespace std;

namespace {

struct Node {
	int x, y;
	int size, used, avail, percent;

	[[nodiscard]]
	tuple<int, int> pos() const
	{
		return { x, y };
	}
};

vector<Node>
readNodes(const filesystem::path& filename)
{
	static const regex line_regex(R"(^/dev/grid/node-x(\d+)-y(\d+)\s+(\d+)T\s+(\d+)T\s+(\d+)T\s+(\d+)%$)");

	ifstream     file{ filename };
	vector<Node> nodes;

	for ( string line; getline(file, line); ) {
		smatch match;

		if ( !regex_match(line, match, line_regex) ) {
			continue;
		}

		Node node{
			.x       = stoi(match[1].str()), // x
			.y       = stoi(match[2].str()), // y
			.size    = stoi(match[3].str()), // size
			.used    = stoi(match[4].str()), // used
			.avail   = stoi(match[5].str()), // avail
			.percent = stoi(match[6].str())  // percent
		};

		nodes.emplace_back(node);
	}

	return nodes;
}

void
part1(const vector<Node>& nodes)
{
	long count = 0;

	for ( size_t i = 0; i != nodes.size(); ++i ) {
		if ( nodes[i].used == 0 ) {
			continue;
		}

		for ( size_t j = 0; j != nodes.size(); ++j ) {
			if ( i == j ) {
				continue;
			}

			if ( nodes[i].used < nodes[j].avail ) {
				++count;
			}
		}
	}

	cout << count << '\n';
}

void
part2(const vector<Node>& nodes)
{
	using pos_type = tuple<int, int>;

	map<pos_type, char> grid;

#ifdef PRINT
	const auto max_y = ranges::max_element(nodes, {}, &Node::y)->y;
#endif
	const auto max_x   = ranges::max_element(nodes, {}, &Node::x)->x;
	const auto largest = ranges::max_element(nodes, {}, &Node::avail)->avail;

	const pos_type g_pos = { max_x, 0 };
	const pos_type s_pos = { 0, 0 };

	pos_type start{};

	for ( const auto& node: nodes ) {
		auto pos = node.pos();
		if ( pos == s_pos ) {
			grid[pos] = 'S';
		}
		else if ( pos == g_pos ) {
			grid[pos] = 'G';
		}
		else if ( node.used == 0 ) {
			grid[pos] = '_';
			start     = pos;
		}
		else if ( node.used >= largest ) {
			grid[pos] = '#';
		}
		else {
			grid[pos] = '.';
		}
	}

#ifdef PRINT
	string result{};
	for ( int pos_y = 0; pos_y <= max_y; ++pos_y ) {
		for ( int pos_x = 0; pos_x <= max_x; ++pos_x ) {
			result += grid[{ pos_x, pos_y }];
		}
		result += '\n';
	}

	cout << result << '\n';
#endif

	set<pos_type>               seen;
	queue<tuple<pos_type, int>> queue; // position, #Schritte

	queue.emplace(start, 0);
	while ( !queue.empty() ) {
		const auto [pos, steps] = queue.front();
		queue.pop();

		if ( pos == g_pos ) {
			cout << steps + (5 * (max_x - 1)) << '\n';
			return;
		}

		static const pos_type dirs[]{ { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };

		for ( const auto [dx, dy]: dirs ) {
			const auto [x, y] = pos;
			const pos_type new_pos{ x + dx, y + dy };

			if ( !grid.contains(new_pos) || grid.at(new_pos) == '#' || seen.contains(new_pos) ) {
				continue;
			}

			seen.insert(new_pos);
			queue.emplace(new_pos, steps + 1);
		}
	}
}

} // namespace

int
main()
{
	auto nodes = readNodes("data/day22.txt");
	part1(nodes);
	part2(nodes);
}
