#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

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
	auto          it = container.begin();
	if ( it != container.end() ) {
		oss << *it;
		++it;
	}
	while ( it != container.end() ) {
		oss << delimiter << *it;
		++it;
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
		else {
			cerr << "Das darf nicht passieren! Input: " << input << endl;
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

void
debug(const tuple<map<string, unsigned long>, map<string, tuple<string, string, string>>>& data)
{
	const auto& deps = get<1>(data);

	function<void(const string&, size_t)> eval = [&](const string& input, size_t depth) {
		if ( !deps.contains(input) ) {
			return;
		}

		if ( depth == 3 ) {
			return;
		}

		const auto& [lhs, op, rhs] = deps.at(input);

		const string indent(depth * 2, ' ');

		cout << indent << op << " (" << input << ")\n"
		     << indent << "  - " << lhs << "\n"
		     << indent << "  - " << rhs << endl;

		eval(lhs, depth + 1);
		eval(rhs, depth + 1);
	};

	eval("z02", 0);
	eval("z16", 0);
	eval("z17", 0);
}

void
part2(const tuple<map<string, unsigned long>, map<string, tuple<string, string, string>>>& data)
{
	auto deps = get<1>(data);

	set<string> wrong;
	for ( const auto& [res, formula]: deps ) {
		const auto& [op1, op, op2] = formula;

		if ( res.starts_with('z') && op != "XOR" && res != "z45" ) {
			wrong.insert(res);
		}

		static const set<char> starts{ 'x', 'y', 'z' };

		if ( op == "XOR" && !starts.contains(res[0]) && !starts.contains(op1[0]) && !starts.contains(op2[0]) ) {
			wrong.insert(res);
		}

		if ( op == "AND" && op1 != "x00" && op2 != "x00" ) {
			for ( const auto& [subres, subformula]: deps ) {
				const auto& [subop1, subop, subop2] = subformula;
				if ( (res == subop1 || res == subop2) and subop != "OR" ) {
					wrong.insert(res);
				}
			}
		}

		if ( op == "XOR" ) {
			for ( const auto& [subres, subformula]: deps ) {
				const auto& [subop1, subop, subop2] = subformula;
				if ( (res == subop1 || res == subop2) and subop == "OR" ) {
					wrong.insert(res);
				}
			}
		}
	}

	cout << join(wrong, ",") << endl;
}

int
main()
{
	const auto data = read_file("data/day24.txt");
	// debug(data);
	part1(data);
	part2(data);
}
