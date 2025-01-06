#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <memory>
#include <sstream>
#include <string>

#undef NDEBUG
#include <cassert>

using namespace std;

#if 0
auto& debug = cerr;
#else
struct NullBuffer : streambuf {
	int overflow(int chr) override { return chr; }
};
NullBuffer nullBuffer;           // NOLINT
ostream    debug{ &nullBuffer }; // NOLINT
#endif

using Mana      = int;
using HitPoints = int;
using Damage    = int;
using Health    = int;
using Armor     = int;

class Player;
class Boss;
class Spell;

shared_ptr<Spell> chooseRandomSpell(Player& player);

class Spell {
protected:
	Spell(string_view name, Mana mana)
	    : name_(name)
	    , mana_(mana)
	{
	}

public:
	Spell(const Spell&)            = default;
	Spell(Spell&&)                 = delete;
	Spell& operator=(const Spell&) = delete;
	Spell& operator=(Spell&&)      = delete;
	virtual ~Spell()               = default;

	virtual void Play(Player& player, Boss& boss) = 0;

	virtual int Timer()
	{
		return 0;
	}

	const string name_;
	const Mana   mana_;
};

class Player {
public:
	// NOLINTBEGIN
	Player(Mana mana, HitPoints hit_points)
	    : mana_(mana)
	    , hit_points_(hit_points)
	{
	}

	void AddMana(Mana mana)
	{
		assert(mana > 0);
		mana_ += mana;
	}

	void SubMana(Mana mana)
	{
		assert(mana > 0);
		mana_ -= mana;
	}

	void AddArmor(Armor armor)
	{
		assert(armor > 0);
		armor_ += armor;
	}

	void SubArmor(Armor armor)
	{
		assert(armor > 0);
		armor_ -= armor;
	}

	void AddHitPoints(HitPoints hit_points)
	{
		assert(hit_points > 0);
		hit_points_ += hit_points;
	}

	void SubHitPoints(HitPoints hit_points)
	{
		assert(hit_points > 0);
		hit_points_ -= hit_points;
	}
	// NOLINTEND

	void Damage(Damage damage)
	{
		SubHitPoints(max(1, damage - armor_));
	}

	[[nodiscard]] bool CanCastSpell(const shared_ptr<Spell>& spell) const
	{
		return mana_ >= spell->mana_;
	}

	[[nodiscard]] bool HasLost() const
	{
		return hit_points_ <= 0;
	}

	Mana      mana_;
	Mana      mana_bought_{};
	HitPoints hit_points_;
	Armor     armor_{};
};

class Spells {
public:
	Spells() = default;

	void AddSpell(const shared_ptr<Spell>& spell, Player& player, Boss& boos)
	{
		debug << "Player casts " << spell->name_ << endl;
		player.SubMana(spell->mana_);
		player.mana_bought_ += spell->mana_;

		if ( spell->Timer() == 0 ) {
			spell->Play(player, boos);
		}
		else {
			effects_.push_back(spell);
		}
	}

	[[nodiscard]] bool TryAddRandomSpell(Player& player, Boss& boss)
	{
		while ( true ) {
			auto randomSpell = chooseRandomSpell(player);
			if ( randomSpell == nullptr ) {
				return false;
			}

			auto iter = ranges::find_if(effects_, [&randomSpell](const auto& effect) { return effect->name_ == randomSpell->name_; });
			if ( iter != effects_.end() ) {
				continue;
			}

			AddSpell(randomSpell, player, boss);

			return true;
		}
	}

	void Play(Player& player, Boss& boss)
	{
		for ( const auto& effect: effects_ ) {
			effect->Play(player, boss);
		}

		effects_.remove_if([](const auto& effect) {
			return effect->Timer() == 0;
		});
	}

private:
	list<shared_ptr<Spell>> effects_;
};

class Boss {
	static auto
	split(const string& line, char sep = ' ')
	{
		vector<string> parts;
		stringstream   input{ line };

		for ( string part; getline(input, part, sep); ) {
			parts.emplace_back(part);
		}

		return parts;
	}

public:
	explicit Boss(string_view filename)
	{
		fstream input{ filename };

		for ( string line; getline(input, line); ) {
			auto parts = Boss::split(line);

			if ( parts[0] == "Hit" ) {
				hit_points_ = stoi(parts[2]);
			}
			else if ( parts[0] == "Damage:" ) {
				damage_ = stoi(parts[1]);
			}
		}
	}

	Boss(HitPoints hit_points, Damage damage) // NOLINT
	    : hit_points_(hit_points)
	    , damage_(damage)
	{
	}

	void Play(Player& player) const
	{
		debug << "Boss attacks for " << damage_ << " damage" << endl;
		player.Damage(damage_);
	}

	void SubHitPoints(HitPoints hit_points)
	{
		assert(hit_points > 0); // NOLINT
		hit_points_ -= hit_points;
	}

	[[nodiscard]] bool HasLost() const
	{
		return hit_points_ <= 0;
	}

	HitPoints hit_points_;
	Damage    damage_;
};

ostream&
operator<<(ostream& ostream, const Player& player)
{
	ostream << "- Player has " << player.hit_points_ << " hit points, " << player.armor_ << " armor, " << player.mana_ << " mana";
	return ostream;
}

ostream&
operator<<(ostream& ostream, const Boss& boss)
{
	ostream << "- Boss has " << boss.hit_points_ << " hit points";
	return ostream;
}

class MagicMissile : public Spell {
public:
	MagicMissile()
	    : Spell("Magic Missile", Mana(Cost))
	{
	}

	void Play(Player& /*player*/, Boss& boss) override
	{
		boss.SubHitPoints(4);

		debug << "Magic Missile deals 4 damage" << endl;
	}

	static const int Cost = 53;
};

class Drain : public Spell {
public:
	Drain()
	    : Spell("Drain", Mana(Cost))
	{
	}

	void Play(Player& player, Boss& boss) override
	{
		player.AddHitPoints(2);
		boss.SubHitPoints(2);
		debug << "Drain deals 2 damage, and heals 2 hit points" << endl;
	}

	static const int Cost = 73;
};

class Effect : public Spell {
protected:
	Effect(string_view name, Mana mana, int timer) // NOLINT
	    : Spell(name, mana)
	    , timer_(timer)
	{
	}

public:
	int Timer() final
	{
		return timer_;
	}

protected:
	int timer_;
};

class Shield : public Effect {
public:
	explicit Shield(Player& player)
	    : Effect("Shield", Mana(Cost), RunTime)
	    , player_(player)
	{
		debug << "Increasing Armor by " << Armor << endl;
		player_.AddArmor(Armor);
	}

	~Shield() override
	{
		debug << "Decreasing Armor by " << Armor << endl;
		player_.SubArmor(Armor);
	}

	void Play(Player& /*player*/, Boss& /*boss*/) override
	{
		timer_--;
		debug << "Shields timer is now " << timer_ << endl;
	}

	static const Armor Armor   = 7;
	static const int   Cost    = 113;
	static const int   RunTime = 6;

private:
	Player& player_;
};

class Poison : public Effect {
public:
	Poison()
	    : Effect("Poison", Mana(Cost), RunTime)
	{
	}

	void Play(Player& /*player*/, Boss& boss) override
	{
		timer_--;

		debug << name_ << " deals 3 damage; its timer is now " << timer_ << endl;
		boss.SubHitPoints(3);
	}

	static const int Cost    = 173;
	static const int RunTime = 6;
};

class Recharge : public Effect {
public:
	Recharge()
	    : Effect("Recharge", Mana(Cost), RunTime)
	{
	}

	~Recharge() override
	{
		debug << "Recharge wears off" << endl;
	}

	void Play(Player& player, Boss& /*boss*/) override
	{
		static const Mana AdditionalMana = 101;

		timer_--;
		player.AddMana(AdditionalMana);
		debug << name_ << " provides 101 mana; its timer is now " << timer_ << endl;
	}

	static const int Cost    = 229;
	static const int RunTime = 5;
};

shared_ptr<Spell>
chooseRandomSpell(Player& player)
{
	if ( player.mana_ < min({ MagicMissile::Cost, Drain::Cost, Shield::Cost, Poison::Cost, Recharge::Cost }) ) {
		return nullptr;
	}

	while ( true ) {
		static const int NumSpells = 5;

		auto selection = rand() % NumSpells;

		if ( selection == 0 && player.mana_ >= MagicMissile::Cost ) {
			return make_shared<MagicMissile>();
		}
		if ( selection == 1 && player.mana_ >= Drain::Cost ) {
			return make_shared<Drain>();
		}
		if ( selection == 2 && player.mana_ >= Shield::Cost ) {
			return make_shared<Shield>(player);
		}
		if ( selection == 3 && player.mana_ >= Poison::Cost ) {
			return make_shared<Poison>();
		}
		if ( selection == 4 && player.mana_ >= Recharge::Cost ) {
			return make_shared<Recharge>();
		}
	}
}

void
battle1()
{
	Player player(250, 10);
	Boss   boss(13, 8);
	Spells spells;

	// ----------------------

	cout << "\n-- Player turn --\n";
	cout << player << '\n'
	     << boss << endl;

	spells.Play(player, boss);
	spells.AddSpell(make_shared<Poison>(), player, boss);

	// ----------------------

	cout << "\n-- Boss turn --\n";
	cout << player << '\n'
	     << boss << endl;

	spells.Play(player, boss);
	boss.Play(player);

	// ----------------------

	cout << "\n-- Player turn --\n";
	cout << player << '\n'
	     << boss << endl;

	spells.Play(player, boss);
	spells.AddSpell(make_shared<MagicMissile>(), player, boss);

	// ----------------------

	cout << "\n-- Boss turn --\n";
	cout << player << '\n'
	     << boss << endl;

	spells.Play(player, boss);

	if ( boss.HasLost() ) {
		cout << "Boss killed" << endl;
	}
}

void
battle2()
{
	Player player(250, 10);
	Boss   boss(14, 8);
	Spells spells;

	// ----------------------

	cout << "\n-- Player turn --\n";
	cout << player << '\n'
	     << boss << endl;

	spells.Play(player, boss);
	spells.AddSpell(make_shared<Recharge>(), player, boss);

	// ----------------------

	cout << "\n-- Boss turn --\n";
	cout << player << '\n'
	     << boss << endl;

	spells.Play(player, boss);
	boss.Play(player);

	// ----------------------

	cout << "\n-- Player turn --\n";
	cout << player << '\n'
	     << boss << endl;

	spells.Play(player, boss);
	spells.AddSpell(make_shared<Shield>(player), player, boss);

	// ----------------------

	cout << "\n-- Boss turn --\n";
	cout << player << '\n'
	     << boss << endl;

	spells.Play(player, boss);
	boss.Play(player);

	// ----------------------

	cout << "\n-- Player turn --\n";
	cout << player << '\n'
	     << boss << endl;

	spells.Play(player, boss);
	spells.AddSpell(make_shared<Drain>(), player, boss);

	// ----------------------

	cout << "\n-- Boss turn --\n";
	cout << player << '\n'
	     << boss << endl;

	spells.Play(player, boss);
	boss.Play(player);

	// ----------------------

	cout << "\n-- Player turn --\n";
	cout << player << '\n'
	     << boss << endl;

	spells.Play(player, boss);
	spells.AddSpell(make_shared<Poison>(), player, boss);

	// ----------------------

	cout << "\n-- Boss turn --\n";
	cout << player << '\n'
	     << boss << endl;

	spells.Play(player, boss);
	boss.Play(player);

	// ----------------------

	cout << "\n-- Player turn --\n";
	cout << player << '\n'
	     << boss << endl;

	spells.Play(player, boss);
	spells.AddSpell(make_shared<MagicMissile>(), player, boss);

	// ----------------------

	cout << "\n-- Boss turn --\n";
	cout << player << '\n'
	     << boss << endl;

	spells.Play(player, boss);
	boss.Play(player);

	if ( boss.HasLost() ) {
		cout << "Boss killed" << endl;
	}

	cout << "End of Battle" << endl;
}

enum RoundState {
	Player_Lost,
	Boss_Lost,
	Undecided
};

RoundState
battle_round(Player& player, Boss& boss, Spells& spells, bool hard_mode)
{
	debug << "\n-- Player turn --\n"
	      << player << '\n'
	      << boss << endl;

	if ( hard_mode ) {
		player.SubHitPoints(1);
	}

	spells.Play(player, boss);
	if ( player.HasLost() ) {
		return Player_Lost;
	}
	if ( boss.HasLost() ) {
		return Boss_Lost;
	}
	if ( !spells.TryAddRandomSpell(player, boss) ) {
		return Player_Lost;
	}

	// ----------------------

	debug << "\n-- Boss turn --\n"
	      << player << '\n'
	      << boss << endl;

	spells.Play(player, boss);
	if ( player.HasLost() ) {
		return Player_Lost;
	}
	if ( boss.HasLost() ) {
		return Boss_Lost;
	}
	boss.Play(player);
	if ( player.HasLost() ) {
		return Player_Lost;
	}
	return Undecided;
}

void
play(bool hard_mode = false)
{
	const Boss boss_master("data/day22.txt");

	static const Mana      start_mana      = 500;
	static const HitPoints start_hitpoints = 50;

	auto min_mana{ 999999999 };

	for ( int i = 0; i != 1000000; ++i ) {
		Boss   boss{ boss_master };
		Player player(start_mana, start_hitpoints);
		Spells spells;

		auto roundState = battle_round(player, boss, spells, hard_mode);
		while ( roundState == Undecided ) {
			roundState = battle_round(player, boss, spells, hard_mode);
		}

		if ( roundState == Boss_Lost ) {
			min_mana = min(min_mana, player.mana_bought_);
		}
	}
	cout << min_mana << endl;
}

void
part1()
{
	play();
}

void
part2()
{
	play(true);
}

int
main()
{
	srand((unsigned int) time(NULL));
	part1();
	part2();
}
