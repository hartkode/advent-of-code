#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

namespace {

using Grid = vector<vector<long>>;
using Ops  = vector<char>;

vector<string>
read_file(const filesystem::path& filename)
{
	ifstream       file{ filename };
	vector<string> lines;

	for ( string line; getline(file, line); ) {
		lines.emplace_back(line);
	}

	return lines;
}

vector<char>
read_ops(const string& line)
{
	stringstream strm{ line };
	vector<char> ops;
	for ( char chr{}; strm >> chr; ) {
		ops.emplace_back(chr);
	}
	return ops;
}

vector<long>
read_numbers(const string& line)
{
	stringstream strm{ line };
	vector<long> numbers;
	for ( long number{}; strm >> number; ) {
		numbers.emplace_back(number);
	}
	return numbers;
}

void
part1(vector<string> lines)
{
	ranges::reverse(lines);

	const auto ops = read_ops(lines[0]);

	vector<vector<long>> grid;
	for ( size_t i = 1; i < lines.size(); ++i ) {
		grid.emplace_back(read_numbers(lines[i]));
	}

	long sum = 0;
	for ( size_t col = 0; col != ops.size(); ++col ) {
		long value = ops[col] == '*' ? 1 : 0;
		for ( auto& row: grid ) {
			if ( ops[col] == '*' ) {
				value *= row[col];
			}
			else {
				value += row[col];
			}
		}
		sum += value;
	}
	cout << "Part 1: " << sum << '\n';
}

vector<string>
rotateLeft90(const vector<string>& matrix)
{
	if ( matrix.empty() ) {
		return {};
	}

	auto rows = matrix.size();
	auto cols = matrix[0].size();

	vector<string> res(cols, string(rows, ' '));

	for ( size_t row = 0; row != rows; ++row ) {
		for ( size_t col = 0; col != cols; ++col ) {
			// Position aus ursprünglicher Matrix auf neue übertragen
			res[cols - 1 - col][row] = matrix[row][col];
		}
	}

	return res;
}

void
part2(vector<string> lines)
{
	auto ops = read_ops(lines.back());
	ranges::reverse(ops);
	lines.pop_back(); // ops entfernen

	// Matrix drehen
	lines = rotateLeft90(lines);
	lines.emplace_back("");

	vector<vector<long>> grid;
	vector<long>         curr;

	// Matrix neu aufbauen
	for ( const auto& line: lines ) {
		if ( long number{}; sscanf(line.data(), "%ld", &number) == 1 ) {
			curr.emplace_back(number);
		}
		else {
			grid.emplace_back(curr);
			curr.clear();
		}
	}

	// ausrechnen
	long sum = 0;
	for ( size_t row = 0; row != ops.size(); ++row ) {
		long value = ops[row] == '*' ? 1 : 0;
		for ( const auto number: grid[row] ) {
			if ( ops[row] == '*' ) {
				value *= number;
			}
			else {
				value += number;
			}
		}
		sum += value;
	}
	cout << "Part 2: " << sum << '\n';
}

} // namespace

int
main()
{
	const auto lines = read_file("data/day06.txt");
	part1(lines);
	part2(lines);
}
