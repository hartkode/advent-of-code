#include <iostream>
#include <limits>
#include <map>
#include <string>

using namespace std;

struct Ingredient {
	long capacity;
	long durability;
	long flavor;
	long texture;
	long calories;
};

// NOLINTBEGIN
map<string, Ingredient> Ingredients = {
	{ "Frosting", { 4, -2, 0, 0, 5 } },
	{ "Candy", { 0, 5, -1, 0, 8 } },
	{ "Butterscotch", { -1, 0, 5, 0, 6 } },
	{ "Sugar", { 0, 0, -2, 2, 1 } }
};
// NOLINTEND

void
part1()
{
	long max_score = numeric_limits<long>::min();

	for ( int frosting = 0; frosting != 100; ++frosting ) {
		for ( int candy = 0; candy != 100 - frosting; ++candy ) {
			for ( int butterscotch = 0; butterscotch != 100 - frosting - candy; ++butterscotch ) {
				int sugar = 100 - butterscotch - candy - frosting;

				if ( sugar < 0 ) {
					continue;
				}

				auto capacity   = Ingredients["Frosting"].capacity * frosting + Ingredients["Candy"].capacity * candy + Ingredients["Butterscotch"].capacity * butterscotch + Ingredients["Sugar"].capacity * sugar;
				auto durability = Ingredients["Frosting"].durability * frosting + Ingredients["Candy"].durability * candy + Ingredients["Butterscotch"].durability * butterscotch + Ingredients["Sugar"].durability * sugar;
				auto flavor     = Ingredients["Frosting"].flavor * frosting + Ingredients["Candy"].flavor * candy + Ingredients["Butterscotch"].flavor * butterscotch + Ingredients["Sugar"].flavor * sugar;
				auto texture    = Ingredients["Frosting"].texture * frosting + Ingredients["Candy"].texture * candy + Ingredients["Butterscotch"].texture * butterscotch + Ingredients["Sugar"].texture * sugar;
				// auto calories = Ingredients["Frosting"].calories * frosting + Ingredients["Candy"].calories * candy + Ingredients["Butterscotch"].calories * butterscotch + Ingredients["Sugar"].calories * sugar;

				auto score = max(0L, capacity) * max(0L, durability) * max(0L, flavor) * max(0L, texture);

				max_score = max(max_score, score);
			}
		}
	}

	cout << max_score << endl;
}

void
part2()
{
	long max_score = numeric_limits<long>::min();

	for ( int frosting = 0; frosting != 100; ++frosting ) {
		for ( int candy = 0; candy != 100 - frosting; ++candy ) {
			for ( int butterscotch = 0; butterscotch != 100 - frosting - candy; ++butterscotch ) {
				int sugar = 100 - butterscotch - candy - frosting;

				if ( sugar < 0 ) {
					continue;
				}

				auto capacity   = Ingredients["Frosting"].capacity * frosting + Ingredients["Candy"].capacity * candy + Ingredients["Butterscotch"].capacity * butterscotch + Ingredients["Sugar"].capacity * sugar;
				auto durability = Ingredients["Frosting"].durability * frosting + Ingredients["Candy"].durability * candy + Ingredients["Butterscotch"].durability * butterscotch + Ingredients["Sugar"].durability * sugar;
				auto flavor     = Ingredients["Frosting"].flavor * frosting + Ingredients["Candy"].flavor * candy + Ingredients["Butterscotch"].flavor * butterscotch + Ingredients["Sugar"].flavor * sugar;
				auto texture    = Ingredients["Frosting"].texture * frosting + Ingredients["Candy"].texture * candy + Ingredients["Butterscotch"].texture * butterscotch + Ingredients["Sugar"].texture * sugar;
				auto calories   = Ingredients["Frosting"].calories * frosting + Ingredients["Candy"].calories * candy + Ingredients["Butterscotch"].calories * butterscotch + Ingredients["Sugar"].calories * sugar;

				auto score = max(0L, capacity) * max(0L, durability) * max(0L, flavor) * max(0L, texture);

				if ( calories == 500 ) {
					max_score = max(max_score, score);
				}
			}
		}
	}
	cout << max_score << endl;
}

int
main()
{
	part1();
	part2();
}
