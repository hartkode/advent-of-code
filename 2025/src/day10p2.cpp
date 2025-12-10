#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <z3++.h>

using namespace std;

namespace {

struct Machine {
	Machine(const vector<vector<unsigned long>>& buttons, const vector<unsigned long>& joltages)
	    : buttons_{ buttons }
	    , joltages_{ joltages }
	{
	}

	vector<vector<unsigned long>> buttons_;
	vector<unsigned long>         joltages_;
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

vector<Machine>
read_file(const filesystem::path& filename)
{
	ifstream file{ filename };

	vector<Machine> machines;

	for ( string line; getline(file, line); ) {
		const auto parts = split(line, "[] (){}");

		vector<vector<unsigned long>> buttons;
		for ( size_t i = 1; i < parts.size() - 1; ++i ) {
			buttons.emplace_back(split(parts[i]));
		}

		const auto joltages = split(parts[parts.size() - 1]);

		machines.emplace_back(buttons, joltages);
	}

	return machines;
}

unsigned long
simulate(const Machine& machine)
{
	using namespace z3;

	context  ctx;
	optimize opt(ctx);

	const size_t num_buttons  = machine.buttons_.size();
	const size_t num_counters = machine.joltages_.size();

	// Z3-Variablen: Anzahl der Button-Presses
	vector<expr> vars;
	vars.reserve(num_buttons);

	for ( size_t i = 0; i != num_buttons; ++i ) {
		expr var = ctx.int_const(("x_" + to_string(i)).c_str());
		opt.add(var >= 0); // natürliche Zahlen
		vars.emplace_back(var);
	}

	// Joltage-Constraints: Für jeden Counter i
	for ( size_t i = 0; i != num_counters; ++i ) {
		expr sum = ctx.int_val(0);

		for ( size_t j = 0; j != num_buttons; ++j ) {
			// Prüfen: affectet Button j den Counter i?
			const auto& btn = machine.buttons_[j];
			if ( ranges::find(btn, i) != btn.end() ) {
				sum = sum + vars[j];
			}
		}

		opt.add(sum == (int) machine.joltages_[i]);
	}

	// Zielfunktion: Minimale Gesamtzahl der Presses
	expr total = ctx.int_val(0);
	for ( size_t i = 0; i != num_buttons; ++i ) {
		total = total + vars[i];
	}

	opt.minimize(total);

	// Solve
	if ( opt.check() != sat ) {
		throw runtime_error("Machine has no valid solution!");
	}

	auto model = opt.get_model();

	unsigned long presses = 0;
	for ( size_t j = 0; j < num_buttons; ++j ) {
		presses += model.eval(vars[j]).get_numeral_uint();
	}
	return presses;
}

void
part2(const vector<Machine>& machines)
{
	unsigned long count = 0;
	for ( const auto& machine: machines ) {
		count += simulate(machine);
	}
	cout << "Part 2: " << count << '\n';
}

} // namespace

int
main()
{
	auto machines = read_file("data/day10.txt");
	part2(machines);
}
