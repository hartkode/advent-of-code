#include <array>
#include <iostream>
#include <numeric>
#include <queue>
#include <set>
#include <string>
#include <vector>

using namespace std;

namespace {

vector<unsigned>
gen_data(size_t len)
{
	vector<unsigned> data(len);
	iota(data.begin(), data.end(), 0UL);
	return data;
}

void
reverse(vector<unsigned>& data, size_t start, size_t len)
{
	auto end = start + len;

	while ( start < end ) {
		swap(data.at(start++ % data.size()), data.at(--end % data.size()));
	}
}

void
hash64(vector<unsigned>& data, const vector<size_t>& lengths)
{
	size_t pos  = 0;
	size_t skip = 0;
	for ( size_t i = 0; i != 64; ++i ) {
		for ( auto length: lengths ) {
			reverse(data, pos, length);
			pos += length + skip;
			++skip;
		}
	}
}

vector<uint8_t>
knothash(string line)
{
	line += char(17);
	line += char(31);
	line += char(73);
	line += char(47);
	line += char(23);

	vector<size_t> lengths;
	for ( const auto byte: line ) {
		lengths.emplace_back(static_cast<unsigned char>(byte));
	}

	auto data = gen_data(256);
	hash64(data, lengths);

	vector<uint8_t> result;
	for ( size_t i = 0; i != 256; i += 16 ) {
		size_t value = 0;
		for ( size_t j = 0; j != 16; ++j ) {
			value ^= data.at(i + j);
		}
		result.emplace_back(value & 0xFF);
	}
	return result;
}

void
part1(const string& line)
{
	int count = 0;
	for ( int i = 0; i != 128; ++i ) {
		auto result = knothash(line + "-"s + to_string(i));

		for ( const auto value: result ) {
			count += popcount(value);
		}
	}
	cout << "Part1: " << count << '\n';
}

void
part2(const string& line)
{
	set<tuple<size_t, size_t>> grid;

	for ( size_t y = 0; y != 128; ++y ) {
		auto result = knothash(line + "-"s + to_string(y));

		for ( size_t x = 0; x != 128; ++x ) {
			auto value = result.at(x / 8);
			auto mask  = 1u << (7 - (x % 8));

			if ( (value & mask) != 0 ) {
				grid.emplace(x, y);
			}
		}
	}

	int count = 0;

	for ( size_t x = 0; x != 128; ++x ) {
		for ( size_t y = 0; y != 128; ++y ) {
			if ( !grid.contains({ x, y }) ) {
				continue;
			}

			++count;

			queue<tuple<size_t, size_t>> queue;
			queue.emplace(x, y);

			while ( !queue.empty() ) {
				auto pos = queue.front();
				queue.pop();

				grid.erase(pos);

				static array<tuple<size_t, size_t>, 4> dirs{ make_tuple(-1, 0), make_tuple(1, 0), make_tuple(0, -1), make_tuple(0, 1) };

				for ( const auto [dx, dy]: dirs ) {
					const auto new_pos = make_tuple(dx + get<0>(pos), dy + get<1>(pos));
					if ( grid.contains(new_pos) ) {
						queue.emplace(new_pos);
					}
				}
			}
		}
	}
	cout << "Part2: " << count << '\n';
}

} // namespace

int
main()
{
	const string input = "ffayrhll";
	part1(input);
	part2(input);
}
