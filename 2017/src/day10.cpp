#include <filesystem>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace {

string
read_file(const filesystem::path& filename)
{
	ifstream file{ filename };
	string   line;
	getline(file, line);
	return line;
}

vector<size_t>
split(const string& line)
{
	stringstream   strm{ line };
	vector<size_t> data;

	for ( string word; getline(strm, word, ','); ) {
		data.emplace_back(stoul(word));
	}

	return data;
}

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
hash(vector<unsigned>& data, const vector<size_t>& lengths)
{
	size_t pos = 0;
	for ( size_t skip = 0; skip != lengths.size(); ++skip ) {
		reverse(data, pos, lengths.at(skip));
		pos += lengths.at(skip) + skip;
	}
}

void
part1(size_t len, const string& line)
{
	auto data    = gen_data(len);
	auto lengths = split(line);

	hash(data, lengths);

	cout << "Part1: " << data.at(0) * data.at(1) << '\n';
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

void
part2(string line)
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

	cout << "Part2: ";
	for ( size_t i = 0; i != 256; i += 16 ) {
		size_t value = 0;
		for ( size_t j = 0; j != 16; ++j ) {
			value ^= data.at(i + j);
		}
		cout << setw(2) << setfill('0') << hex << (value & 0xFF);
	}
	cout << '\n';
}

} // namespace

int
main()
{
	auto line = read_file("data/day10.txt");
	part1(256, line);
	part2(line);
}
