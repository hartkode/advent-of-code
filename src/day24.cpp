#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <tuple>
#include <vector>
using namespace std;

using line_type   = tuple<double, double, double, double, double, double>;
using point_type  = tuple<double, double>;
using puzzle_type = vector<line_type>;

template<typename T>
int
sgn(T val)
{
	return (T(0) < val) - (val < T(0));
}

puzzle_type
read_file(string_view filename)
{
	fstream     input{ filename };
	puzzle_type result;

	for ( string line; getline(input, line); ) {
		const auto NFIELDS = 6;

		double x, y, z, dx, dy, dz; // NOLINT

		if ( sscanf(line.data(), "%lf, %lf, %lf @ %lf, %lf, %lf", &x, &y, &z, &dx, &dy, &dz) != NFIELDS ) {
			continue;
		}

		result.emplace_back(make_tuple(x, y, z, dx, dy, dz));
	}

	return result;
}

point_type
get_direction(line_type line)
{
	const auto [x, y, z, dx, dy, dz] = line;

	return make_tuple(sgn(dx), sgn(dy));
}

point_type
get_direction(line_type from, point_type to)
{
	const auto [x, y, z, dx, dy, dz] = from;
	const auto [to_x, to_y]          = to;

	return make_tuple(sgn(to_x - x), sgn(to_y - y));
}

bool
lint(line_type stone_a, line_type stone_b, point_type& cross_point)
{
	const auto p_1 = make_tuple(get<0>(stone_a), get<1>(stone_a));
	const auto p_2 = make_tuple(get<0>(stone_a) + get<3>(stone_a), get<1>(stone_a) + get<4>(stone_a));

	const auto p_3 = make_tuple(get<0>(stone_b), get<1>(stone_b));
	const auto p_4 = make_tuple(get<0>(stone_b) + get<3>(stone_b), get<1>(stone_b) + get<4>(stone_b));

	const auto a_1 = get<1>(p_2) - get<1>(p_1);
	const auto b_1 = get<0>(p_1) - get<0>(p_2);
	const auto c_1 = a_1 * (get<0>(p_1)) + b_1 * (get<1>(p_1));

	const auto a_2 = get<1>(p_4) - get<1>(p_3);
	const auto b_2 = get<0>(p_3) - get<0>(p_4);
	const auto c_2 = a_2 * (get<0>(p_3)) + b_2 * (get<1>(p_3));

	const auto determinant = a_1 * b_2 - a_2 * b_1;

	if ( determinant == 0 ) {
		return false;
	}

	auto p_x = (b_2 * c_1 - b_1 * c_2) / determinant;
	auto p_y = (a_1 * c_2 - a_2 * c_1) / determinant;

	cross_point = make_tuple(p_x, p_y);

	return true;
}

void
part1(const puzzle_type& input)
{
	const auto in_range = [](point_type point) -> bool {
		static const auto lower_limit = 200000000000000.0;
		static const auto upper_limit = 400000000000000.0;

		const auto [x, y] = point;

		return x > lower_limit && x < upper_limit && y > lower_limit && y < upper_limit;
	};

	const auto in_future = [](line_type stone, point_type point) -> bool {
		const auto sgn_stone = get_direction(stone);
		const auto sgn_point = get_direction(stone, point);

		return sgn_stone == sgn_point;
	};

	long sum = 0;
	for ( size_t idx = 0; idx != input.size(); ++idx ) {
		const auto& stone_a = input[idx];

		for ( size_t idx2 = idx + 1; idx2 < input.size(); ++idx2 ) {
			const auto& stone_b = input[idx2];

			point_type cross_point;
			if ( lint(stone_a, stone_b, cross_point) && in_range(cross_point) && in_future(stone_a, cross_point) && in_future(stone_b, cross_point) ) {
				++sum;
			}
		}
	}
	cout << sum << endl;
}

int
main()
{
	const auto input = read_file("data/day24.txt");

	part1(input);
}
