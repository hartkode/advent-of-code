#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <tuple>
using namespace std;

using pos_type = tuple<size_t, size_t>;

map<pos_type, char>
read_file(string_view filename)
{
	fstream             input{ filename };
	map<pos_type, char> data;

	size_t yPos = 0;
	for ( string line; getline(input, line); ) {
		for ( size_t xPos = 0; xPos != line.size(); ++xPos ) {
			data[{ xPos, yPos }] = line[xPos];
		}
		++yPos;
	}

	return data;
}

void
part1(const map<pos_type, char>& data)
{
	map<char, vector<pos_type>> unique_frequencies;
	for ( const auto& [position, frequency]: data ) {
		if ( frequency == '.' ) {
			continue;
		}
		unique_frequencies[frequency].emplace_back(position);
	}

	set<pos_type> antinodes;
	for ( const auto& [frequency, positions]: unique_frequencies ) {
		for ( size_t i = 0; i != positions.size(); ++i ) {
			for ( size_t j = i + 1; j != positions.size(); ++j ) {
				const auto [lhs_x, lhs_y] = positions[i];
				const auto [rhs_x, rhs_y] = positions[j];

				const auto delta_x = rhs_x - lhs_x;
				const auto delta_y = rhs_y - lhs_y;

				pos_type new_position1{ lhs_x - delta_x, lhs_y - delta_y };
				if ( data.contains(new_position1) ) {
					antinodes.emplace(new_position1);
				}

				pos_type new_position2{ rhs_x + delta_x, rhs_y + delta_y };
				if ( data.contains(new_position2) ) {
					antinodes.emplace(new_position2);
				}
			}
		}
	}
	cout << antinodes.size() << endl;
}

void
part2(const map<pos_type, char>& data)
{
	map<char, vector<pos_type>> unique_frequencies;
	for ( const auto& [position, frequency]: data ) {
		if ( frequency == '.' ) {
			continue;
		}
		unique_frequencies[frequency].emplace_back(position);
	}

	set<pos_type> antinodes;
	for ( const auto& [frequency, positions]: unique_frequencies ) {
		for ( size_t i = 0; i != positions.size(); ++i ) {
			for ( size_t j = i + 1; j != positions.size(); ++j ) {
				const auto [lhs_x, lhs_y] = positions[i];
				const auto [rhs_x, rhs_y] = positions[j];

				const auto delta_x = rhs_x - lhs_x;
				const auto delta_y = rhs_y - lhs_y;

				for ( size_t count = 0;; ++count ) {
					pos_type new_position{ lhs_x - count * delta_x, lhs_y - count * delta_y };
					if ( !data.contains(new_position) ) {
						break;
					}
					antinodes.emplace(new_position);
				}

				for ( size_t count = 0;; ++count ) {
					pos_type new_position{ rhs_x + count * delta_x, rhs_y + count * delta_y };
					if ( !data.contains(new_position) ) {
						break;
					}
					antinodes.emplace(new_position);
				}
			}
		}
	}
	cout << antinodes.size() << endl;
}

int
main()
{
	auto data = read_file("data/day08.txt");
	part1(data);
	part2(data);
}
