#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace {

class Machine {
public:
	Machine(unsigned long dest, const vector<unsigned long>& buttons)
	    : dest_{ dest }
	    , buttons_{ buttons }
	{
	}

	void push(size_t button)
	{
		curr_ ^= buttons_.at(button);
		++pushes_;
	}

	[[nodiscard]]
	bool test() const
	{
		return curr_ == dest_;
	}

	void reset()
	{
		curr_   = 0;
		pushes_ = 0;
	}

	[[nodiscard]]
	unsigned long button_count() const
	{
		return buttons_.size();
	}

	[[nodiscard]]
	unsigned long get_pushes() const
	{
		return pushes_;
	}

private:
	unsigned long dest_;
	unsigned long curr_{ 0 };
	unsigned long pushes_{ 0 };

	vector<unsigned long> buttons_;
};

vector<string>
split(const string& str, const string& delims)
{
	vector<string> result;

	size_t start = str.find_first_not_of(delims);
	while ( start != string::npos ) {
		size_t end = str.find_first_of(delims, start);

		if ( end == string::npos ) {
			// letzter Teil
			result.push_back(str.substr(start));
			break;
		}

		result.push_back(str.substr(start, end - start));

		start = str.find_first_not_of(delims, end);
	}

	return result;
}

vector<unsigned long>
split(const string& str)
{
	stringstream          strm{ str };
	vector<unsigned long> values;

	for ( string part; getline(strm, part, ','); ) {
		values.emplace_back(stoul(part));
	}

	return values;
}

unsigned long
from_pattern(string pattern)
{
	unsigned long value = 0;
	ranges::reverse(pattern);
	for ( const char chr: pattern ) {
		value <<= 1UL;
		value |= (chr == '#') ? 1U : 0U;
	}
	return value;
}

unsigned long
from_button_string(const string& pattern)
{
	unsigned long value = 0;
	for ( const auto pos: split(pattern) ) {
		value |= (1UL << pos);
	}
	return value;
}

vector<Machine>
read_file(const filesystem::path& filename)
{
	ifstream file{ filename };

	vector<Machine> machines;

	for ( string line; getline(file, line); ) {
		const auto parts = split(line, "[] (){}");

		const auto dest = from_pattern(parts[0]);

		vector<unsigned long> buttons;
		for ( size_t i = 1; i < parts.size() - 1; ++i ) {
			buttons.emplace_back(from_button_string(parts[i]));
		}

		machines.emplace_back(dest, buttons);
	}

	return machines;
}

unsigned long
simulate(Machine machine)
{
	auto min_so_far = numeric_limits<unsigned long>::max();

	const auto button_count = machine.button_count();

	for ( unsigned long i = 0; i != (1UL << button_count); ++i ) {
		machine.reset();

		for ( unsigned long button = 0; button != button_count; ++button ) {
			if ( (i & (1UL << button)) != 0 ) {
				machine.push(button);
			}
		}

		if ( machine.test() ) {
			min_so_far = min(min_so_far, machine.get_pushes());
		}
	}

	return min_so_far;
}

void
part1(const vector<Machine>& machines)
{
	unsigned long count = 0;
	for ( const auto& machine: machines ) {
		count += simulate(machine);
	}
	cout << "Part 1: " << count << '\n';
}

} // namespace

int
main()
{
	auto machines = read_file("data/day10.txt");
	part1(machines);
}
