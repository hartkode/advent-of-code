#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>

using namespace std;

namespace {

class UnionFind {
public:
	explicit UnionFind(size_t size)
	    : parent_(size)
	{
		for ( size_t i = 0; i != size; ++i ) {
			parent_[i] = i;
		}
	}

	size_t find(size_t n)
	{
		if ( parent_[n] != n ) {
			parent_[n] = find(parent_[n]);
		}
		return parent_[n];
	}

	bool unite(size_t a, size_t b)
	{
		a = find(a);
		b = find(b);

		if ( a != b ) {
			parent_[b] = a;
			return true;
		}
		return false;
	}

private:
	vector<size_t> parent_;
};

using Pos = tuple<long, long, long>;

vector<Pos>
read_file(const filesystem::path& filename)
{
	ifstream file{ filename };

	vector<Pos> list;

	long x{};
	long y{};
	long z{};
	char ch{};

	while ( file >> x >> ch >> y >> ch >> z ) {
		list.emplace_back(x, y, z);
	}

	return list;
}

double
get_distance(const Pos& pos1, const Pos& pos2)
{
	const auto [x1, y1, z1] = pos1;
	const auto [x2, y2, z2] = pos2;

	return hypot(x1 - x2, y1 - y2, z1 - z2);
}

void
part1(const vector<Pos>& list)
{
	using Dist = tuple<double, size_t, size_t>; // (Distanz, Von, Nach)

	// Distanzen paarweise erfassen...
	vector<Dist> dists;
	for ( size_t i = 0; i != list.size(); ++i ) {
		for ( size_t j = i + 1; j != list.size(); ++j ) {
			dists.emplace_back(get_distance(list[i], list[j]), i, j);
		}
	}

	// ... und nach Distanz sortieren
	ranges::sort(dists);

	UnionFind unionFind(list.size());

	// die kürzesten 1000 Elemente verbinden
	for ( size_t i = 0; i != 1000; ++i ) {
		const auto [dist, a, b] = dists[i];
		unionFind.unite(a, b);
	}

	// die Längen der Verbindungen zählen
	map<size_t, size_t> compSizes;
	for ( size_t i = 0; i != list.size(); ++i ) {
		compSizes[unionFind.find(i)]++;
	}

	// Größen finden
	vector<size_t> sizes;
	for ( const auto& [root, size]: compSizes ) {
		sizes.emplace_back(size);
	}

	// Sortieren
	ranges::sort(sizes, greater<>());

	cout << "Part 1: " << sizes[0] * sizes[1] * sizes[2] << '\n';
}

void
part2(const vector<Pos>& list)
{
	using Dist = tuple<double, size_t, size_t>; // (Distanz, Von, Nach)

	// Distanzen paarweise erfassen...
	vector<Dist> dists;
	for ( size_t i = 0; i != list.size(); ++i ) {
		for ( size_t j = i + 1; j != list.size(); ++j ) {
			dists.emplace_back(get_distance(list[i], list[j]), i, j);
		}
	}

	// ... und nach Distanz sortieren
	ranges::sort(dists);

	UnionFind unionFind(list.size());

	size_t count = 1;
	for ( auto [dist, a, b]: dists ) {
		if ( unionFind.unite(a, b) ) {
			if ( ++count == list.size() ) {
				cout << "Part 2: " << get<0>(list[a]) * get<0>(list[b]) << '\n';
				return;
			}
		}
	}
}

} // namespace

int
main()
{
	auto data = read_file("data/day08.txt");
	part1(data);
	part2(data);
}
