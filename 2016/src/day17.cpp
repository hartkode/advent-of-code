// Standard C++
#include <algorithm>
#include <array>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <tuple>

// System
#include <md5.h>

using namespace std;

namespace {

using pos_type = tuple<int, int, string>;

string
md5(string_view input)
{
	array<char, MD5_DIGEST_STRING_LENGTH> digest{};
	MD5Data(input.data(), input.size(), digest.data());
	return digest.data();
}

void
part1(const string& passcode)
{
	static const map<size_t, tuple<char, int, int>> dirs{
		{ 0, { 'U', 0, -1 } },
		{ 1, { 'D', 0, 1 } },
		{ 2, { 'L', -1, 0 } },
		{ 3, { 'R', 1, 0 } }
	};

	auto is_open = [](auto chr) {
		return strchr("bcdef", chr) != nullptr;
	};

	queue<pos_type> queue;
	queue.emplace(0, 0, "");

	while ( !queue.empty() ) {
		auto [x, y, path] = queue.front();
		queue.pop();

		if ( x == 3 && y == 3 ) {
			cout << path << '\n';
			return;
		}

		auto hash = md5(passcode + path);

		for ( size_t i = 0; i != 4; ++i ) {
			if ( !is_open(hash.at(i)) ) {
				continue;
			}

			auto [chr, dx, dy] = dirs.at(i);

			auto new_x = x + dx;
			auto new_y = y + dy;

			if ( new_x < 0 || new_y < 0 || new_x >= 4 || new_y >= 4 ) {
				continue;
			}

			queue.emplace(new_x, new_y, path + chr);
		}
	}
}

void
part2(const string& passcode)
{
	static const map<size_t, tuple<char, int, int>> dirs{
		{ 0, { 'U', 0, -1 } },
		{ 1, { 'D', 0, 1 } },
		{ 2, { 'L', -1, 0 } },
		{ 3, { 'R', 1, 0 } }
	};

	auto is_open = [](auto chr) {
		return strchr("bcdef", chr) != nullptr;
	};

	size_t max_length = 0;

	queue<pos_type> queue;
	queue.emplace(0, 0, "");

	while ( !queue.empty() ) {
		auto [x, y, path] = queue.front();
		queue.pop();

		if ( x == 3 && y == 3 ) {
			max_length = max(max_length, path.size());
			continue;
		}

		auto hash = md5(passcode + path);

		for ( size_t i = 0; i != 4; ++i ) {
			if ( !is_open(hash.at(i)) ) {
				continue;
			}

			auto [chr, dx, dy] = dirs.at(i);

			auto new_x = x + dx;
			auto new_y = y + dy;

			if ( new_x < 0 || new_y < 0 || new_x >= 4 || new_y >= 4 ) {
				continue;
			}

			queue.emplace(new_x, new_y, path + chr);
		}
	}

	cout << max_length << '\n';
}

} // namespace

int
main()
{
	part1("awrkjxxr");
	part2("awrkjxxr");
}
