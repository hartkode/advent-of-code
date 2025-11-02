#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

namespace {

string
read_line(const filesystem::path& filename)
{
	ifstream file{ filename };
	string   line;
	getline(file, line);
	return line;
}

long
calculate(string_view captcha, size_t offset)
{
	long sum = 0;
	for ( size_t i = 0; i != captcha.size(); ++i ) {
		if ( captcha[i] == captcha[(i + offset) % captcha.size()] ) {
			sum += captcha[i] - '0';
		}
	}
	return sum;
}

void
part1(string_view captcha)
{
	cout << "Part1: " << calculate(captcha, 1) << '\n';
}

void
part2(string_view captcha)
{
	cout << "Part2: " << calculate(captcha, captcha.size() / 2) << '\n';
}

} // namespace

int
main()
{
	auto captcha = read_line("data/day01.txt");
	part1(captcha);
	part2(captcha);
}
