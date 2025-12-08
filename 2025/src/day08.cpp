#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>

using namespace std;

namespace {

class UnionFind {
public:
	explicit UnionFind(size_t size)
	    : parent_(size)
	    , size_(size, 1)
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

		if ( a == b ) {
			return false;
		}

		if ( size_[a] < size_[b] ) {
			swap(a, b);
		}

		parent_[b] = a;
		size_[a] += size_[b];

		return true;
	}

	[[nodiscard]]
	vector<size_t> get_component_sizes() const
	{
		vector<size_t> sizes;
		for ( size_t i = 0; i != parent_.size(); ++i ) {
			if ( parent_[i] == i ) {
				sizes.emplace_back(size_[i]);
			}
		}
		return sizes;
	}

private:
	vector<size_t> parent_;
	vector<size_t> size_;
};

using Pos  = tuple<long, long, long>;       // (x, y, z)
using Dist = tuple<double, size_t, size_t>; // (Distanz, Von, Nach)

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

vector<Dist>
get_distances(const vector<Pos>& list)
{
	// Distanzen paarweise erfassen...
	vector<Dist> dists;
	for ( size_t i = 0; i != list.size(); ++i ) {
		for ( size_t j = i + 1; j != list.size(); ++j ) {
			dists.emplace_back(get_distance(list[i], list[j]), i, j);
		}
	}

	// ... und nach Distanz sortieren
	ranges::sort(dists);

	return dists;
}

void
part1(const vector<Pos>& list)
{
	const auto dists = get_distances(list);

	UnionFind unionFind(list.size());

	// die nächsten 1000 Paare verbinden
	for ( size_t i = 0; i != 1000; ++i ) {
		const auto [dist, a, b] = dists[i];
		unionFind.unite(a, b);
	}

	auto sizes = unionFind.get_component_sizes();

	// Sortieren
	nth_element(sizes.begin(), sizes.begin() + 3, sizes.end(), greater<>());

	cout << "Part 1: " << sizes[0] * sizes[1] * sizes[2] << '\n';
}

void
part2(const vector<Pos>& list)
{
	const auto dists = get_distances(list);

	UnionFind unionFind(list.size());

	// Alle Paare verbinden ...
	size_t unions = 0;
	for ( auto [dist, a, b]: dists ) {
		if ( unionFind.unite(a, b) ) {
			// ... beim Letzten abbrechen
			++unions;
			if ( unions == list.size() - 1 ) {
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
