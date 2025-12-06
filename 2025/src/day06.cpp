#include <filesystem>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
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

Ops
read_ops(const string& line)
{
	stringstream strm{ line };
	Ops          ops;
	for ( char chr{}; strm >> chr; ) {
		ops.emplace_back(chr);
	}
	return ops;
}

Ops
extract_ops(vector<string>& lines)
{
	auto ops = read_ops(lines.back());
	ranges::reverse(ops);
	lines.pop_back();

	return ops;
}

template<typename Matrix>
Matrix
rotateLeft90(const Matrix& matrix)
{
	using Row = typename Matrix::value_type;
	using T   = typename Row::value_type;

	if ( matrix.empty() ) {
		return Matrix{};
	}

	const size_t rows = matrix.size();
	const size_t cols = matrix[0].size();

	Matrix result(cols, Row(rows, T{}));

	for ( size_t row = 0; row != rows; ++row ) {
		for ( size_t col = 0; col != cols; ++col ) {
			result[cols - 1 - col][row] = matrix[row][col];
		}
	}
	return result;
}

long
calculate(const Grid& grid, const Ops& ops)
{
	long sum = 0;
	for ( size_t row = 0; row != ops.size(); ++row ) {
		const auto& line = grid[row];
		if ( ops[row] == '*' ) {
			sum += accumulate(line.begin(), line.end(), 1L, multiplies<>());
		}
		else {
			sum += accumulate(line.begin(), line.end(), 0L);
		}
	}
	return sum;
}

void
part1(vector<string> lines)
{
	const auto ops = extract_ops(lines);

	Grid grid;
	grid.reserve(lines.size());
	for ( const auto& line: lines ) {
		stringstream strm{ line };
		vector<long> numbers;
		for ( long number{}; strm >> number; ) {
			numbers.emplace_back(number);
		}

		grid.emplace_back(numbers);
	}

	grid = rotateLeft90(grid);

	cout << "Part 1: " << calculate(grid, ops) << '\n';
}

void
part2(vector<string> lines)
{
	const auto ops = extract_ops(lines);

	lines = rotateLeft90(lines);
	lines.emplace_back("");

	Grid         grid;
	vector<long> curr;

	for ( const auto& line: lines ) {
		if ( long number{}; sscanf(line.data(), "%ld", &number) == 1 ) {
			curr.emplace_back(number);
		}
		else {
			grid.emplace_back(curr);
			curr.clear();
		}
	}

	cout << "Part 2: " << calculate(grid, ops) << '\n';
}

} // namespace

int
main()
{
	const auto lines = read_file("data/day06.txt");
	part1(lines);
	part2(lines);
}
