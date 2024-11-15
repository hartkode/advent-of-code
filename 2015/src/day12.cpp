#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

// JSON parsing
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

string
read_file(string_view filename)
{
	fstream input{ filename };
	return { istreambuf_iterator{ input }, {} };
}

template<class UnaryFunction>
void
recursive_iterate(const json& j, UnaryFunction f) // NOLINT
{
	for ( auto it = j.begin(); it != j.end(); ++it ) {
		if ( it->is_structured() ) {
			recursive_iterate(*it, f);
		}
		else {
			f(it);
		}
	}
}

void
part1(string_view document)
{
	auto json = json::parse(document);

	long sum = 0;
	recursive_iterate(json, [&](json::const_iterator iter) {
		if ( iter.value().is_number() ) {
			sum += iter.value().get<long>();
		}
	});
	cout << sum << endl;
}

void
part2(string_view document)
{
	json::parser_callback_t parser_callback = [](int /*depth*/, json::parse_event_t event, json& parsed) {
		if ( event == json::parse_event_t::object_end ) {
			for ( auto it = parsed.begin(); it != parsed.end(); ++it ) {
				if ( it.value().is_string() && it.value().get<string>() == "red" ) {
					return false;
				}
			}
		}
		return true;
	};

	auto json = json::parse(document, parser_callback);

	long sum = 0;
	recursive_iterate(json, [&](json::const_iterator iter) {
		if ( iter.value().is_number() ) {
			sum += iter.value().get<long>();
		}
	});
	cout << sum << endl;
}

int
main()
{
	auto document = read_file("data/day12.txt");
	part1(document);
	part2(document);
}
