#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

static const auto MAX_BITS = 45UL;

vector<string>
split(string_view line, string_view delimiter)
{
	size_t pos_start = 0;
	size_t pos_end   = 0;

	vector<string> res;

	while ( (pos_end = line.find(delimiter, pos_start)) != std::string::npos ) {
		auto token = line.substr(pos_start, pos_end - pos_start);
		pos_start  = pos_end + delimiter.length();

		res.emplace_back(token);
	}

	res.emplace_back(line.substr(pos_start));
	return res;
}

template<typename Container>
string
join(const Container& container, const string& delimiter)
{
	ostringstream oss;
	auto          iter = container.begin();
	if ( iter != container.end() ) {
		oss << *iter;
		++iter;
	}
	while ( iter != container.end() ) {
		oss << delimiter << *iter;
		++iter;
	}
	return oss.str();
}

tuple<map<string, unsigned long>, map<string, tuple<string, string, string>>>
read_file(string_view filename)
{
	fstream    input{ filename };
	const auto parts = split(string{ istreambuf_iterator<char>{ input }, {} }, "\n\n");

	map<string, unsigned long> inputs;

	// register => { reg1 operator reg2 }
	map<string, tuple<string, string, string>> deps;

	stringstream spart1{ parts[0] };
	for ( string line; getline(spart1, line); ) {
		const auto foo = split(line, ": ");
		inputs[foo[0]] = stoul(foo[1]);
	}

	stringstream spart2{ parts[1] };
	for ( string line; getline(spart2, line); ) {
		const auto foo = split(line, " ");
		deps[foo[4]]   = { foo[0], foo[1], foo[2] };
	}

	return { inputs, deps };
}

void
part1(const tuple<map<string, unsigned long>, map<string, tuple<string, string, string>>>& data)
{
	auto        inputs = get<0>(data);
	const auto& deps   = get<1>(data);

	function<unsigned long(const string&)> eval = [&](const string& input) -> unsigned long {
		if ( inputs.contains(input) ) {
			return inputs.at(input);
		}

		const auto& [lhs, op, rhs] = deps.at(input);

		unsigned long value = 0;

		if ( op == "XOR" ) {
			value = eval(lhs) ^ eval(rhs);
		}
		else if ( op == "AND" ) {
			value = eval(lhs) & eval(rhs);
		}
		else if ( op == "OR" ) {
			value = eval(lhs) | eval(rhs);
		}

		inputs[input] = value;

		return value;
	};

	unsigned long value = 0;
	for ( const auto& [first, second]: deps ) {
		if ( first.starts_with("z") ) {
			if ( eval(first) != 0 ) {
				auto bit = stoul(first.substr(1));
				value |= 1UL << bit;
			}
		}
	}
	cout << value << endl;
}

class RippleCarryAdder {
public:
	explicit RippleCarryAdder(const tuple<map<string, unsigned long>, map<string, tuple<string, string, string>>>& data)
	    : inputs_(get<0>(data))
	    , deps_(get<1>(data))
	{
	}

	void swap_wire(const string& lhs, const string& rhs)
	{
		swap(deps_.at(lhs), deps_.at(rhs));
	}

	bool eval_z(unsigned long x_value, unsigned long y_value)
	{
		build_inputs(x_value, y_value);

		bool overflow = false;
		for ( auto bit = 0UL; bit != MAX_BITS + 1; ++bit ) {
			static const int MAX_RECURSION_DEPTH = 99;
			eval(make_wire('z', bit), MAX_RECURSION_DEPTH, overflow);
		}
		return !overflow;
	}

	[[nodiscard]] vector<string> test(unsigned long x_value, unsigned long y_value) const
	{
		const auto z_value = x_value + y_value;

		vector<string> failed_bits;
		auto           bit = 0UL;
		for ( ; bit != MAX_BITS + 1; ++bit ) {
			const auto z_bit = (z_value & (1UL << bit)) != 0 ? 1UL : 0UL;
			const auto wire  = make_wire('z', bit);
			if ( inputs_.at(wire) != z_bit ) {
				failed_bits.push_back(wire);
			}
		}
		return failed_bits;
	};

	void print(unsigned long x_value, unsigned long y_value, ostream& out) const
	{
		const auto z_value = x_value + y_value;

		out << "  x:  " << bitset<MAX_BITS>{ x_value } << '\n'
		    << "+ y:  " << bitset<MAX_BITS>{ y_value } << '\n'
		    << "= z: " << bitset<MAX_BITS + 1>{ z_value } << '\n';

		out << "===: ";
		for ( auto bit = MAX_BITS + 1; bit-- > 0; ) {
			const auto wire = make_wire('z', bit);
			if ( inputs_.contains(wire) ) {
				out << inputs_.at(wire);
			}
			else {
				out << '_';
			}
		}
		out << endl;
	};

	void collect(const string& input, set<string>& possible_fails) const
	{
		collect(input, 2, possible_fails);
	}

private:
	static string make_wire(char chr, unsigned long num)
	{
		static const size_t  MAX_LEN = 10;
		array<char, MAX_LEN> tmp{};
		snprintf(tmp.data(), tmp.size(), "%c%02lu", chr, num); // NOLINT
		return tmp.data();
	}

	void build_inputs(unsigned long x_value, unsigned long y_value)
	{
		inputs_.clear();

		for ( auto bit = 0UL; bit != MAX_BITS; ++bit ) {
			inputs_.emplace(make_wire('x', bit), (x_value & (1UL << bit)) != 0 ? 1 : 0);
			inputs_.emplace(make_wire('y', bit), (y_value & (1UL << bit)) != 0 ? 1 : 0);
		}
	}

	unsigned long eval(const string& input, size_t depth, bool& overflow)
	{
		if ( depth == 0 ) {
			overflow = true;
			return 0;
		}

		if ( inputs_.contains(input) ) {
			return inputs_.at(input);
		}

		const auto& [lhs, op, rhs] = deps_.at(input);

		unsigned long value = 0;

		if ( op == "XOR" ) {
			value = eval(lhs, depth - 1, overflow) ^ eval(rhs, depth - 1, overflow);
		}
		else if ( op == "AND" ) {
			value = eval(lhs, depth - 1, overflow) & eval(rhs, depth - 1, overflow);
		}
		else if ( op == "OR" ) {
			value = eval(lhs, depth - 1, overflow) | eval(rhs, depth - 1, overflow);
		}

		inputs_[input] = value;

		return value;
	}

	void collect(const string& input, size_t depth, set<string>& possible_fails) const
	{
		if ( !deps_.contains(input) ) {
			return;
		}

		possible_fails.insert(input);

		if ( depth == 0 ) {
			return;
		}

		const auto& [lhs, op, rhs] = deps_.at(input);

		collect(lhs, depth - 1, possible_fails);
		collect(rhs, depth - 1, possible_fails);
	};

	map<string, unsigned long> inputs_;

	// register => { reg1 operator reg2 }
	map<string, tuple<string, string, string>> deps_;
};

void
part2(const tuple<map<string, unsigned long>, map<string, tuple<string, string, string>>>& data)
{
	RippleCarryAdder rca(data);

	set<string> solution;

	for ( auto bit = 0UL; bit != MAX_BITS; ++bit ) {
		const auto x_value = 1UL << bit;
		const auto y_value = 0UL;

		if ( !rca.eval_z(x_value, y_value) ) {
			return;
		}

		const auto failed_bits = rca.test(x_value, y_value);
		if ( failed_bits.empty() ) {
			continue;
		}

		set<string> unique_candidates;
		for ( const auto& wire: failed_bits ) {
			rca.collect(wire, unique_candidates);
		}

		const vector<string> candidates{ unique_candidates.begin(), unique_candidates.end() };

		for ( auto i = candidates.begin(); i != candidates.end(); ++i ) {
			for ( auto j = i + 1; j != candidates.end(); ++j ) {
				auto do_test = [&](const tuple<unsigned long, unsigned long>& test_value) -> bool {
					const auto [lhs, rhs] = test_value;

					RippleCarryAdder rca_tester{ data };

					rca_tester.swap_wire(*i, *j);

					if ( !rca_tester.eval_z(lhs, rhs) ) {
						return false;
					}
					return rca_tester.test(lhs, rhs).empty();
				};

				const vector<tuple<unsigned long, unsigned long>> test_values = {
					{ x_value, y_value },
					{ 7UL << (bit - 1), 3UL << (bit - 1) },
					{ 1UL << bit, 1UL << bit },
					{ 5UL << (bit - 1), 3UL << (bit - 1) }
				};

				if ( ranges::all_of(test_values, do_test) ) {
					solution.insert(*i);
					solution.insert(*j);
				}
			}
		}
	}
	cout << join(solution, ",") << endl;
}

int
main()
{
	const auto data = read_file("data/day24.txt");
	part1(data);
	part2(data);
}
