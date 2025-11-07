#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

namespace {

vector<string>
split(const string& line, const string& delimiters)
{
	vector<string> result;

	size_t start = 0;
	size_t end   = 0;

	while ( (end = line.find_first_of(delimiters, start)) != string::npos ) {
		if ( end != start ) {
			result.emplace_back(line.substr(start, end - start));
		}

		start = end + 1;
	}

	if ( start != line.size() ) {
		result.emplace_back(line.substr(start));
	}

	return result;
}

vector<string>
rotate90(const vector<string>& input)
{
	if ( input.empty() ) {
		return {};
	}

	const auto rows = input.size();
	const auto cols = input[0].size();

	vector<string> rotated(cols, string(rows, ' '));

	for ( size_t row = 0; row != rows; ++row ) {
		for ( size_t col = 0; col != cols; ++col ) {
			rotated[col][rows - 1 - row] = input[row][col];
		}
	}

	return rotated;
}

vector<string>
flip(const vector<string>& input)
{
	vector<string> result = input;

	// oder: ranges::reverse(result);
	for ( auto& row: result ) {
		ranges::reverse(row);
	}

	return result;
}

vector<vector<string>>
generate_variants(vector<string> pattern)
{
	vector<vector<string>> variants;

	for ( int flip_count = 0; flip_count < 2; ++flip_count ) {
		for ( int round = 0; round < 4; ++round ) {
			variants.push_back(pattern);
			pattern = rotate90(pattern);
		}
		pattern = flip(pattern);
	}

	return variants;
}

map<vector<string>, vector<string>>
read_file(const filesystem::path& filename)
{
	ifstream                            file{ filename };
	map<vector<string>, vector<string>> data;

	for ( string line; getline(file, line); ) {
		auto parts = split(line, " ");
		auto lhs   = split(parts.at(0), "/");
		auto rhs   = split(parts.at(2), "/");

		for ( const auto& variant: generate_variants(lhs) ) {
			data[variant] = rhs;
		}
	}

	return data;
}

vector<string>
enhance(const vector<string>& image, const map<vector<string>, vector<string>>& rules)
{
	const auto size           = image.size();
	const auto block_size     = (size % 2 == 0) ? 2U : 3U;
	const auto new_block_size = block_size + 1;
	const auto blocks_per_row = size / block_size;
	const auto new_size       = blocks_per_row * new_block_size;

	vector<string> new_image(new_size, string(new_size, '.'));

	// Für jeden Block...
	for ( size_t block_row = 0; block_row != blocks_per_row; ++block_row ) {
		for ( size_t block_col = 0; block_col != blocks_per_row; ++block_col ) {
			// ...Extrahiere den Block als vector<string>
			vector<string> block;
			block.reserve(block_size);
			for ( size_t row = 0; row != block_size; ++row ) {
				block.push_back(image[(block_row * block_size) + row].substr(block_col * block_size, block_size));
			}

			const auto& new_block = rules.at(block);

			// Füge neuen Block ins Bild ein
			for ( size_t row = 0; row != new_block_size; ++row ) {
				for ( size_t col = 0; col != new_block_size; ++col ) {
					new_image[(block_row * new_block_size) + row][(block_col * new_block_size) + col] = new_block[row][col];
				}
			}
		}
	}

	return new_image;
}

int
count(const vector<string>& image)
{
	int count = 0;
	for ( const auto& row: image ) {
		for ( char chr: row ) {
			if ( chr == '#' ) {
				++count;
			}
		}
	}
	return count;
}

int
solve(const map<vector<string>, vector<string>>& rules, size_t iterations)
{
	vector<string> image = { ".#.", "..#", "###" };

	while ( iterations-- > 0 ) {
		image = enhance(image, rules);
	}

	return count(image);
}

void
part1(const map<vector<string>, vector<string>>& rules)
{
	cout << "Part1: " << solve(rules, 5) << '\n';
}

void
part2(const map<vector<string>, vector<string>>& rules)
{
	cout << "Part2: " << solve(rules, 18) << '\n';
}

} // namespace

int
main()
{
	auto rules = read_file("data/day21.txt");
	part1(rules);
	part2(rules);
}
