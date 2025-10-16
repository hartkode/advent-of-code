#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>

using namespace std;

struct Item {
	Item() = default;
	Item(size_t chip_floor, size_t generator_floor) // NOLINT
	    : chip_floor_{ chip_floor }
	    , generator_floor_{ generator_floor }
	{
	}

	size_t chip_floor_;
	size_t generator_floor_;

	bool operator<(const Item& rhs) const
	{
		if ( chip_floor_ != rhs.chip_floor_ ) {
			return chip_floor_ < rhs.chip_floor_;
		}
		return generator_floor_ < rhs.generator_floor_;
	}
};

struct State {
	explicit State(const vector<Item>& items)
	    : items_{ items }
	{
	}

	[[nodiscard]]
	bool is_valid() const
	{
		for ( size_t i = 0; i != items_.size(); ++i ) {
			const auto chip_floor = items_[i].chip_floor_; // Etage des Mikrochips
			if ( chip_floor == items_[i].generator_floor_ ) {
				continue; // Mikrochip ist mit seinem Generator zusammen
			}
			// Prüfe, ob ein fremder Generator auf derselben Etage ist
			for ( size_t j = 0; j != items_.size(); ++j ) {
				if ( j != i && chip_floor == items_[j].generator_floor_ ) {
					return false; // Ungültig: Mikrochip mit fremdem Generator ohne eigenen Generator
				}
			}
		}
		return true;
	}

	[[nodiscard]]
	bool is_goal() const
	{
		for ( const auto& item: items_ ) {
			if ( item.chip_floor_ != 3 || item.generator_floor_ != 3 ) {
				return false;
			}
		}
		return elevator_ == 3;
	}

	bool operator<(const State& rhs) const
	{
		if ( elevator_ != rhs.elevator_ ) {
			return elevator_ < rhs.elevator_;
		}
		return items_ < rhs.items_;
	}

	size_t       elevator_{};
	vector<Item> items_;
};

namespace {

set<string>
split(const string& line, char sep)
{
	set<string>  parts;
	stringstream input{ line };

	for ( string part; getline(input, part, sep); ) {
		parts.insert(part);
	}

	for ( const auto& word: { "The", "a", "floor", "and", "contains", "microchip",
	                          "microchip.", "microchip,", "generator", "generator.", "generator,", "first",
	                          "second", "third", "fourth", "nothing", "relevant." } ) {
		parts.erase(word);
	}

	return parts;
}

vector<Item>
read_file(const filesystem::path& filename)
{
	fstream           input{ filename };
	map<string, Item> data;

	auto is_microchip = [](string_view item) {
		return item.ends_with("-compatible");
	};

	string line;
	for ( size_t floor = 0; getline(input, line); ++floor ) {
		auto items = split(line, ' ');

		for ( auto item: items ) {
			if ( is_microchip(item) ) {
				item = item.erase(item.find('-'));

				data[item].chip_floor_ = floor;
			}
			else {
				data[item].generator_floor_ = floor;
			}
		}
	}

	vector<Item> result;
	result.reserve(data.size());
	for ( const auto& item: data ) {
		result.emplace_back(item.second);
	}

	return result;
}

int
part1(const State& initial)
{
	queue<pair<State, int>> queue; // {Zustand, Schritte}
	set<State>              visited;

	queue.emplace(initial, 0);
	visited.insert(initial);

	while ( !queue.empty() ) {
		auto [current, steps] = queue.front();
		queue.pop();

		if ( current.is_goal() ) {
			return steps;
		}

		// Mögliche Aufzugbewegungen: hoch oder runter
		for ( int dir: { -1, 1 } ) {
			auto new_elevator = current.elevator_ + size_t(dir);
			if ( new_elevator < 0 || new_elevator > 3 ) {
				continue;
			}

			// Wähle 1 oder 2 Objekte
			vector<size_t> items_on_floor;
			for ( size_t i = 0; i < current.items_.size(); ++i ) {
				if ( current.items_[i].generator_floor_ == current.elevator_ ) {
					items_on_floor.emplace_back(i * 2); // Generator
				}
				if ( current.items_[i].chip_floor_ == current.elevator_ ) {
					items_on_floor.emplace_back((i * 2) + 1); // Mikrochip
				}
			}

			// Alle Kombinationen von 1 oder 2 Objekten
			for ( size_t i = 0; i < items_on_floor.size(); ++i ) {
				// 1 Objekt
				State next     = current;
				next.elevator_ = new_elevator;
				auto item      = items_on_floor[i];
				if ( item % 2 == 0 ) {
					next.items_[item / 2].generator_floor_ = new_elevator; // Generator
				}
				else {
					next.items_[item / 2].chip_floor_ = new_elevator; // Mikrochip
				}
				if ( next.is_valid() && !visited.contains(next) ) {
					visited.emplace(next);
					queue.emplace(next, steps + 1);
				}

				// 2 Objekte
				for ( size_t j = i + 1; j < items_on_floor.size(); ++j ) {
					State next2     = current;
					next2.elevator_ = new_elevator;
					auto item1      = items_on_floor[i];
					auto item2      = items_on_floor[j];
					if ( item1 % 2 == 0 ) {
						next2.items_[item1 / 2].generator_floor_ = new_elevator;
					}
					else {
						next2.items_[item1 / 2].chip_floor_ = new_elevator;
					}
					if ( item2 % 2 == 0 ) {
						next2.items_[item2 / 2].generator_floor_ = new_elevator;
					}
					else {
						next2.items_[item2 / 2].chip_floor_ = new_elevator;
					}
					if ( next2.is_valid() && !visited.contains(next2) ) {
						visited.insert(next2);
						queue.emplace(next2, steps + 1);
					}
				}
			}
		}
	}
	return -1; // Keine Lösung gefunden
}

int
part2(State initial)
{
	initial.items_.emplace_back(0, 0);
	initial.items_.emplace_back(0, 0);
	return part1(initial);
}

} // namespace

int
main()
{
	auto  floors = read_file("data/day11.txt");
	State state{ floors };

	cout << "Part1: " << part1(state) << '\n';
	cout << "Part2: " << part2(state) << '\n';
}
