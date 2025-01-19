#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <tuple>

using namespace std;

using Sink = tuple<string, int>;

vector<string>
split(const string& line, char sep)
{
	vector<string> parts;
	stringstream   input{ line };

	for ( string part; getline(input, part, sep); ) {
		parts.emplace_back(part);
	}

	return parts;
}

map<int, tuple<Sink, Sink, vector<int>>>
read_file(string_view filename)
{
	fstream input{ filename };

	map<int, tuple<Sink, Sink, vector<int>>> bots;

	for ( string line; getline(input, line); ) {
		const auto parts = split(line, ' ');

		if ( parts.at(0) == "bot" ) {
			const Sink low_sink = { parts.at(5), stoi(parts.at(6)) };
			const Sink hi_sink  = { parts.at(10), stoi(parts.at(11)) };

			auto& bot = bots[stoi(parts.at(1))];

			get<0>(bot) = low_sink;
			get<1>(bot) = hi_sink;
		}
		else if ( parts.at(0) == "value" ) {
			auto& bot = bots[stoi(parts.at(5))];

			get<2>(bot).push_back(stoi(parts.at(1)));
		}
	}

	return bots;
}

void
solve(map<int, tuple<Sink, Sink, vector<int>>> bots)
{
	map<int, int> outputs;

	const auto store = [&](const Sink& sink, int value) {
		if ( get<0>(sink) == "bot" ) {
			auto& to_bot = bots[get<1>(sink)];
			get<2>(to_bot).push_back(value);
		}
		else {
			outputs[get<1>(sink)] = value;
		}
	};

	while ( true ) {
		auto it = ranges::find_if(bots, [](auto& bot) {
			const auto& [id, values] = bot;
			return (get<2>(values).size() == 2);
		});
		if ( it == bots.end() ) {
			break;
		}

		auto& [id, values]               = *it;
		auto& [low_sink, hi_sink, chips] = values;

		ranges::sort(chips);
		store(low_sink, chips.at(0));
		store(hi_sink, chips.at(1));

		// part1
		if ( chips.at(0) == 17 && chips.at(1) == 61 ) {
			cout << id << endl;
		}

		chips.clear();
	}

	// part2
	cout << outputs[0] * outputs[1] * outputs[2] << endl;
}

int
main()
{
	auto bots = read_file("data/day10.txt");
	solve(bots);
}
