#include <iostream>
#include <string_view>

using namespace std;

namespace {

size_t
smallest_dragon_length(string_view start_bits, size_t disc_size)
{
	auto length = start_bits.size();
	while ( length < disc_size ) {
		length = 2 * length + 1;
	}
	return length;
}

char
bit_at(size_t pos, size_t total_length, string_view start_bits)
{
	size_t inversions = 0;

	auto invert_if_needed = [&](char chr) {
		if ( (inversions % 2) == 1 ) {
			return (chr == '0') ? '1' : '0';
		}
		else {
			return chr;
		}
	};

	while ( total_length > start_bits.size() ) {
		auto prev_length = (total_length - 1) / 2;

		if ( pos == prev_length ) {
			return invert_if_needed('0');
		}
		else if ( pos > prev_length ) {
			pos = prev_length - 1 - (pos - prev_length - 1);
			inversions++;
			total_length = prev_length;
		}
		else {
			total_length = prev_length;
		}
	}

	auto bit = start_bits[pos];

	return invert_if_needed(bit);
}

void
checksum(string_view start_bits, size_t disc_size)
{
	auto data_length = smallest_dragon_length(start_bits, disc_size);

	string bits;
	for ( size_t i = 0; i != disc_size; ++i ) {
		bits += bit_at(i, data_length, start_bits);
	}

	while ( bits.size() % 2 == 0 ) {
		string new_bits;

		for ( size_t i = 0; i != bits.size(); i += 2 ) {
			new_bits += (bits[i] == bits[i + 1]) ? '1' : '0';
		}

		bits = std::move(new_bits);
	}

	cout << bits << '\n';
}

} // namespace

int
main()
{
	// checksum("10000", 20);
	checksum("00111101111101000", 272);
	checksum("00111101111101000", 35651584);
}
