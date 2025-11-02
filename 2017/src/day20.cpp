#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

namespace {

using point_type    = tuple<long, long, long>;
using particle_type = tuple<point_type, point_type, point_type>;

#if 0
ostream&
operator<<(ostream& strm, const point_type& point)
{
	const auto [x, y, z] = point;
	strm << '<' << x << ',' << y << ',' << z << '>';
	return strm;
}

ostream&
operator<<(ostream& strm, const particle_type& particle)
{
	const auto [pos, velo, accel] = particle;
	strm << "p=" << pos << ", v=" << velo << ", a=" << accel;
	return strm;
}
#endif

vector<long>
split(const string& line, const string& delimiters)
{
	vector<long> result;

	size_t start = 0;
	size_t end   = 0;

	while ( (end = line.find_first_of(delimiters, start)) != string::npos ) {
		if ( end != start ) {
			result.emplace_back(stol(line.substr(start, end - start)));
		}

		start = end + 1;
	}

	if ( start != line.size() ) {
		result.emplace_back(stol(line.substr(start)));
	}

	return result;
}

vector<particle_type>
read_file(const filesystem::path& filename)
{
	ifstream              file{ filename };
	vector<particle_type> data;

	for ( string line; getline(file, line); ) {
		auto parts = split(line, "p=<>,va ");

		point_type position{ parts.at(0), parts.at(1), parts.at(2) };
		point_type velocity{ parts.at(3), parts.at(4), parts.at(5) };
		point_type acceleration{ parts.at(6), parts.at(7), parts.at(8) };

		data.emplace_back(position, velocity, acceleration);
	}

	return data;
}

point_type
add(const point_type& lhs, const point_type& rhs)
{
	return { get<0>(lhs) + get<0>(rhs), get<1>(lhs) + get<1>(rhs), get<2>(lhs) + get<2>(rhs) };
}

void
update(particle_type& particle)
{
	auto [pos, vel, acc] = particle;

	vel = add(vel, acc);
	pos = add(pos, vel);

	particle = { pos, vel, acc };
}

long
distance(const point_type& point)
{
	return abs(get<0>(point)) + abs(get<1>(point)) + abs(get<2>(point));
}

void
part1(vector<particle_type> particles)
{
	size_t current_min_index = particles.size();

	for ( int round = 0; round != 10000; ++round ) {
		// Alle updaten
		ranges::for_each(particles, update);

		long   step_min_dist  = numeric_limits<long>::max();
		size_t step_min_index = current_min_index;

		// kleinste Distanz suchen
		for ( size_t i = 0; i != particles.size(); ++i ) {
			auto min_dist = distance(get<0>(particles.at(i)));

			if ( min_dist < step_min_dist ) {
				step_min_dist  = min_dist;
				step_min_index = i;
			}
		}

		// Prüfen, ob stabil
		if ( step_min_index != current_min_index ) {
			current_min_index = step_min_index;
			round             = 0;
		}
	}

	cout << "Part1: " << current_min_index << '\n';
}

void
part2(vector<particle_type> particles)
{
	for ( int round = 0; round != 10000; ++round ) {
		// Alle updaten
		ranges::for_each(particles, update);

		map<point_type, size_t> collision_counter;

		for ( const auto& [pos, vel, accl]: particles ) {
			collision_counter[pos]++;
		}

		for ( const auto& [pos, count]: collision_counter ) {
			if ( count > 1 ) {
				erase_if(particles, [&pos](const auto& particle) { return get<0>(particle) == pos; });
				round = 0;
			}
		}
	}

	cout << "Part2: " << particles.size() << '\n';
}

} // namespace

int
main()
{
	auto data = read_file("data/day20.txt");
	part1(data);
	part2(data);
}
