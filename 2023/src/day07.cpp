#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <tuple>
#include <vector>
using namespace std;

enum kind_type {
	five_kind,
	four_kind,
	full_house_kind,
	three_kind,
	two_pair_kind,
	one_pair_kind,
	high_card_kind
};

kind_type
kind1(string hand)
{
	map<char, size_t> mapping{};

	for ( auto chr: hand ) {
		++mapping[chr];
	}

	sort(hand.begin(), hand.end());
	hand.erase(unique(hand.begin(), hand.end()), hand.end());

	if ( mapping.size() == 1 ) {
		return five_kind;
	}
	if ( mapping.size() == 2 ) {
		if ( mapping[hand[0]] == 4 || mapping[hand[1]] == 4 ) {
			return four_kind;
		}
		return full_house_kind;
	}
	if ( mapping.size() == 3 ) {
		if ( mapping[hand[0]] == 3 || mapping[hand[1]] == 3 || mapping[hand[2]] == 3 ) {
			return three_kind;
		}
		return two_pair_kind;
	}
	if ( mapping.size() == 4 ) {
		return one_pair_kind;
	}

	return high_card_kind;
}

kind_type
kind2(const string& hand)
{
	map<char, size_t> mapping{};

	for ( auto chr: hand ) {
		++mapping[chr];
	}

	auto joker_amount = mapping['J'];
	mapping.erase('J');

	vector<size_t> amounts{};
	amounts.reserve(mapping.size());
	for ( const auto& entry: mapping ) {
		amounts.emplace_back(entry.second);
	}

	sort(amounts.begin(), amounts.end(), greater<>());
	if ( amounts.empty() ) {
		amounts.emplace_back(joker_amount);
	}
	else {
		amounts[0] += joker_amount;
	}

	if ( amounts[0] == 5 ) {
		return five_kind;
	}
	if ( amounts[0] == 4 ) {
		return four_kind;
	}
	if ( amounts[0] == 3 and amounts[1] == 2 ) {
		return full_house_kind;
	}
	if ( amounts[0] == 3 ) {
		return three_kind;
	}
	if ( amounts[0] == 2 && amounts[1] == 2 ) {
		return two_pair_kind;
	}
	if ( amounts[0] == 2 ) {
		return one_pair_kind;
	}

	return high_card_kind;
}

void
solve(const function<kind_type(string)>& kind, map<char, int> weights)
{
	fstream                                input{ "data/day07.txt" };
	vector<tuple<string, kind_type, long>> values{};

	string hand;
	long   bid = 0;
	while ( input >> hand >> bid ) {
		values.emplace_back(hand, kind(hand), bid);
	}

	sort(values.begin(), values.end(), [&](const tuple<string, kind_type, long>& first, const tuple<string, kind_type, long>& second) {
		const auto& first_hand  = get<0>(first);
		const auto& second_hand = get<0>(second);

		auto first_kind  = get<1>(first);
		auto second_kind = get<1>(second);

		if ( first_kind == second_kind ) {
			for ( size_t idx = 0; idx != 5; ++idx ) {
				const auto first_weight  = weights[first_hand[idx]];
				const auto second_weight = weights[second_hand[idx]];

				if ( first_weight != second_weight ) {
					return first_weight < second_weight;
				}
			}
		}
		return first_kind > second_kind;
	});

	long total = 0;
	long rank  = 1;
	for ( const auto& value: values ) {
		total += rank * get<2>(value);
		++rank;
	}
	cout << total << endl;
}

int
main()
{
	assert(kind1("AAAAA") == five_kind);
	assert(kind1("AA8AA") == four_kind);
	assert(kind1("23332") == full_house_kind);
	assert(kind1("TTT98") == three_kind);
	assert(kind1("23432") == two_pair_kind);
	assert(kind1("A23A4") == one_pair_kind);
	assert(kind1("23456") == high_card_kind);

	map<char, int> weights1 = {
		{ '2', 2 },
		{ '3', 3 },
		{ '4', 4 },
		{ '5', 5 },
		{ '6', 6 },
		{ '7', 7 },
		{ '8', 8 },
		{ '9', 9 },
		{ 'T', 10 },
		{ 'J', 11 },
		{ 'Q', 12 },
		{ 'K', 13 },
		{ 'A', 14 },
	};

	map<char, int> weights2 = {
		{ 'J', 1 },
		{ '2', 2 },
		{ '3', 3 },
		{ '4', 4 },
		{ '5', 5 },
		{ '6', 6 },
		{ '7', 7 },
		{ '8', 8 },
		{ '9', 9 },
		{ 'T', 10 },
		{ 'Q', 11 },
		{ 'K', 12 },
		{ 'A', 13 },
	};

	solve(kind1, weights1);
	solve(kind2, weights2);
}
