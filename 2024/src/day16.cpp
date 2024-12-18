#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <queue>
#include <set>
#include <string>
#include <tuple>
using namespace std;

using pos_type   = tuple<size_t, size_t>;
using walls_type = set<pos_type>;

// pos, direction, score
using index_type = tuple<pos_type, size_t, int>;

struct index_compare {
	bool operator()(const index_type& lhs, const index_type& rhs)
	{
		// compare score
		return get<2>(lhs) > get<2>(rhs);
	}
};

using index_pq = priority_queue<index_type, vector<index_type>, index_compare>;

static const array<pos_type, 4> dirs = {
	pos_type{ -1, 0 },
	pos_type{ 0, 1 },
	pos_type{ 1, 0 },
	pos_type{ 0, -1 }
};

tuple<walls_type, pos_type, pos_type>
read_file(string_view filename)
{
	fstream    input{ filename };
	walls_type walls;
	pos_type   start;
	pos_type   finish;

	size_t yPos = 0;
	for ( string line; getline(input, line); ) {
		for ( size_t xPos = 0; xPos != line.size(); ++xPos ) {
			auto chr = line.at(xPos);
			if ( chr == '#' ) {
				walls.emplace(xPos, yPos);
			}
			else if ( chr == 'S' ) {
				start = { xPos, yPos };
			}
			else if ( chr == 'E' ) {
				finish = { xPos, yPos };
			}
		}
		++yPos;
	}

	return { walls, start, finish };
}

void
print(const walls_type& walls, pos_type start, pos_type finish)
{
	size_t width  = 0;
	size_t height = 0;

	for ( const auto& [x, y]: walls ) {
		width  = max(width, x + 1);
		height = max(height, y + 1);
	}

	for ( size_t yPos = 0; yPos != height; ++yPos ) {
		for ( size_t xPos = 0; xPos != width; ++xPos ) {
			const pos_type pos{ xPos, yPos };
			if ( pos == start ) {
				cout << 'S';
			}
			else if ( pos == finish ) {
				cout << 'E';
			}
			else if ( walls.contains(pos) ) {
				cout << '#';
			}
			else {
				cout << '.';
			}
		}
		cout << '\n';
	}
	cout << endl;
}

int
part1(const tuple<walls_type, pos_type, pos_type>& data)
{
	const auto& [walls, start, finish] = data;

	// pos, direction
	set<tuple<pos_type, size_t>> seen;

	index_pq queue;
	queue.push({ start, 2, 0 });

	while ( !queue.empty() ) {
		const auto [pos, dir, score] = queue.top();
		queue.pop();

		if ( pos == finish ) {
			return score;
		}

		if ( seen.contains({ pos, dir }) ) {
			continue;
		}
		seen.insert({ pos, dir });

		const auto     delta = dirs.at(dir);
		const pos_type next  = { get<0>(pos) + get<0>(delta),
			                     get<1>(pos) + get<1>(delta) };

		if ( !walls.contains(next) ) {
			queue.push({ next, dir, score + 1 });
		}
		queue.push({ pos, (dir + 1) % 4, score + 1000 });
		queue.push({ pos, (dir + 3) % 4, score + 1000 });
	}
	return -1;
}

map<tuple<pos_type, size_t>, int>
walk(const tuple<walls_type, pos_type, pos_type>& data, const vector<index_type>& starts, size_t offset)
{
	const auto& [walls, start, finish] = data;

	set<tuple<pos_type, size_t>> seen;

	map<tuple<pos_type, size_t>, int> scores;

	index_pq queue{ starts.begin(), starts.end() };

	while ( !queue.empty() ) {
		const auto [pos, dir, score] = queue.top();
		queue.pop();

		if ( !scores.contains({ pos, dir }) ) {
			scores[{ pos, dir }] = score;
		}

		if ( seen.contains({ pos, dir }) ) {
			continue;
		}
		seen.insert({ pos, dir });

		const auto     delta = dirs.at((dir + offset) % 4);
		const pos_type next  = { get<0>(pos) + get<0>(delta),
			                     get<1>(pos) + get<1>(delta) };

		if ( !walls.contains(next) ) {
			queue.push({ next, dir, score + 1 });
		}
		queue.push({ pos, (dir + 1) % 4, score + 1000 });
		queue.push({ pos, (dir + 3) % 4, score + 1000 });
	}
	return scores;
}

size_t
part2(const tuple<walls_type, pos_type, pos_type>& data, int best)
{
	const auto& [walls, start, finish] = data;

	auto scores_forwards  = walk({ walls, start, finish }, { { start, 2, 0 } }, 0);
	auto scores_backwards = walk({ walls, finish, start }, { { finish, 0, 0 }, { finish, 1, 0 }, { finish, 2, 0 }, { finish, 3, 0 } }, 2);

	set<pos_type> match;
	for ( const auto& [pos_dir, score]: scores_forwards ) {
		if ( scores_backwards.contains(pos_dir) && scores_backwards.at(pos_dir) + score == best ) {
			const auto [pos, dir] = pos_dir;
			match.insert(pos);
		}
	}
	return match.size();
}

int
main()
{
	auto data = read_file("data/day16.txt");

	auto score = part1(data);
	cout << score << endl;

	cout << part2(data, score) << endl;
}
