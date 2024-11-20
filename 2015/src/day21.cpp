#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

struct Item {
	Item(string_view name, int cost, int damage, int armor)
	    : name_(name)
	    , cost_(cost)
	    , damage_(damage)
	    , armor_(armor)
	{
	}

	const string name_;
	const int    cost_;
	const int    damage_;
	const int    armor_;
};

struct Boss {
	int hit_points_;
	int damage_;
	int armor_;
};

struct Player {
	Player(const Item& weapon, const Item& armor, const tuple<Item, Item>& rings)
	    : damage_(weapon.damage_ + armor.damage_ + get<0>(rings).damage_ + get<1>(rings).damage_)
	    , armor_(weapon.armor_ + armor.armor_ + get<0>(rings).armor_ + get<1>(rings).armor_)
	{
	}

	int hit_points_{ 100 };
	int damage_;
	int armor_;

	bool would_win_against(Boss boss)
	{
		while ( true ) {
			auto player_damage = max(1, damage_ - boss.armor_);
			boss.hit_points_ -= player_damage;
			if ( boss.hit_points_ <= 0 ) {
				return true;
			}

			auto boss_damage = max(1, boss.damage_ - armor_);
			hit_points_ -= boss_damage;
			if ( hit_points_ <= 0 ) {
				return false;
			}
		}
	}
};

const array<Item, 5> weapons = {
	Item("Dagger", 8, 4, 0),
	Item("Shortsword", 10, 5, 0),
	Item("Warhammer", 25, 6, 0),
	Item("Longsword", 40, 7, 0),
	Item("Greataxe", 74, 8, 0)
};

const array<Item, 6> armors = {
	Item("Leather", 13, 0, 1),
	Item("Chainmail", 31, 0, 2),
	Item("Splintmail", 53, 0, 3),
	Item("Bandedmail", 75, 0, 4),
	Item("Platemail", 102, 0, 5),
	Item("Noting", 0, 0, 0)
};

vector<tuple<Item, Item>>
generate_ring_pairs()
{
	const array<Item, 7> rings = {
		Item("Damage +1", 25, 1, 0),
		Item("Damage +2", 50, 2, 0),
		Item("Damage +3", 100, 3, 0),
		Item("Defense +1", 20, 0, 1),
		Item("Defense +2", 40, 0, 2),
		Item("Defense +3", 80, 0, 3),
		Item("Nothing 0", 0, 0, 0)
	};

	vector<tuple<Item, Item>> pairs;

	for ( size_t i = 0; i < rings.size(); ++i ) {
		for ( size_t j = i + 1; j < rings.size(); ++j ) {
			pairs.emplace_back(rings.at(i), rings.at(j));
		}
	}

	pairs.emplace_back(rings.back(), rings.back());

	return pairs;
}

auto
split(const string& line, char sep = ' ')
{
	vector<string> parts;
	stringstream   input{ line };

	for ( string part; getline(input, part, sep); ) {
		parts.emplace_back(part);
	}

	return parts;
}

Boss
read_file(string_view filename)
{
	fstream input{ filename };
	Boss    boss{};

	for ( string line; getline(input, line); ) {
		auto parts = split(line);

		if ( parts[0] == "Hit" ) {
			boss.hit_points_ = stoi(parts[2]);
		}
		else if ( parts[0] == "Damage:" ) {
			boss.damage_ = stoi(parts[1]);
		}
		else if ( parts[0] == "Armor:" ) {
			boss.armor_ = stoi(parts[1]);
		}
	}
	return boss;
}

void
part1(const Boss& boss)
{
	auto rings = generate_ring_pairs();

	auto min_cost = numeric_limits<int>::max();
	for ( const auto& weapon: weapons ) {
		for ( const auto& armor: armors ) {
			for ( const auto& ring_pair: rings ) {
				Player player(weapon, armor, ring_pair);

				if (player.would_win_against(boss)) {
					min_cost = min(min_cost, weapon.cost_ + armor.cost_ + get<0>(ring_pair).cost_ + get<1>(ring_pair).cost_);
				}
			}
		}
	}
	cout << min_cost << endl;
}

void
part2(const Boss& boss)
{
	auto rings = generate_ring_pairs();

	auto max_cost = numeric_limits<int>::min();
	for ( const auto& weapon: weapons ) {
		for ( const auto& armor: armors ) {
			for ( const auto& ring_pair: rings ) {
				Player player(weapon, armor, ring_pair);

				if (!player.would_win_against(boss)) {
					max_cost = max(max_cost, weapon.cost_ + armor.cost_ + get<0>(ring_pair).cost_ + get<1>(ring_pair).cost_);
				}
			}
		}
	}
	cout << max_cost << endl;
}

int
main()
{
	auto boss = read_file("data/day21.txt");
	part1(boss);
	part2(boss);
}
