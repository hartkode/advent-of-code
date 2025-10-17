// Standard C++
#include <array>
#include <iostream>
#include <map>
#include <optional>
#include <string>

// Standard C
#include <cstring>

// System
#include <md5.h>

using namespace std;

namespace {

optional<char>
contains_repeating_chars(string_view str)
{
	for ( size_t i = 2; i < str.size(); ++i ) {
		if ( str[i] == str[i - 1] && str[i] == str[i - 2] ) {
			return str[i];
		}
	}

	return {};
}

string
md5(const string& input)
{
	array<char, MD5_DIGEST_STRING_LENGTH> digest{};
	MD5Data(input.data(), input.size(), digest.data());
	return digest.data();
}

int
solve(const function<string(int)>& get_hash)
{
	int keys = 0;

	for ( int index = 0;; ++index ) {
		auto hash = get_hash(index);

		auto repeat = contains_repeating_chars(hash);

		if ( !repeat.has_value() ) {
			continue;
		}

		const string pattern(5, repeat.value());

		for ( auto index2 = index + 1; index2 != index + 1000; ++index2 ) {
			hash = get_hash(index2);
			if ( hash.find(pattern) != string::npos ) {
				++keys;
				break;
			}
		}

		if ( keys == 64 ) {
			return index;
		}
	}
}

void
part1(const string& input)
{
	map<int, string> cache;

	auto get_hash = [&](int index) {
		if ( cache.contains(index) ) {
			return cache.at(index);
		}

		auto hash = md5(input + to_string(index));

		cache.emplace(index, hash);

		return hash;
	};

	cout << solve(get_hash) << '\n';
}

void
part2(const string& input)
{
	map<int, string> cache;

	auto get_hash = [&](int index) {
		if ( cache.contains(index) ) {
			return cache.at(index);
		}

		auto hash = input + to_string(index);

		for ( int i = 0; i != 2017; ++i ) {
			hash = md5(hash);
		}

		cache.emplace(index, hash);

		return hash;
	};

	cout << solve(get_hash) << '\n';
}

} // namespace

int
main()
{
	// part1("abc");
	// part2("abc");
	part1("qzyelonm");
	part2("qzyelonm");
}
