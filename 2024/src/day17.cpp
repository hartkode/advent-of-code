#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
using namespace std;

using program_type   = vector<unsigned long>;
using registers_type = map<char, unsigned long>;

vector<string>
split(const string& line, char sep = ' ')
{
	vector<string> parts;
	stringstream   input{ line };

	for ( string part; getline(input, part, sep); ) {
		parts.emplace_back(part);
	}

	return parts;
}

tuple<registers_type, program_type>
read_file(string_view filename)
{
	fstream input{ filename };

	map<char, unsigned long> registers;
	vector<unsigned long>    program;

	for ( string line; getline(input, line); ) {
		if ( line.empty() ) {
			continue;
		}

		const auto parts = split(line, ':');

		if ( parts[0] == "Register A" ) {
			registers['A'] = stoul(parts[1]);
		}
		else if ( parts[0] == "Register B" ) {
			registers['B'] = stoul(parts[1]);
		}
		else if ( parts[0] == "Register C" ) {
			registers['C'] = stoul(parts[1]);
		}
		else if ( parts[0] == "Program" ) {
			for ( const auto& value: split(parts[1], ',') ) {
				program.emplace_back(stoul(value));
			}
		}
	}

	return { registers, program };
}

vector<unsigned long>
run(registers_type registers, const program_type& program)
{
	vector<unsigned long> out;

	for ( unsigned long ip = 0; ip < program.size(); ) {
		const auto opcode  = program.at(ip++);
		const auto operand = program.at(ip++);
		const auto value   = (operand < 4) ? operand : registers[char('A' + operand - 4)];

		switch ( opcode ) {
		case 0: // adv
			registers['A'] >>= value;
			break;
		case 1: // bxl
			registers['B'] ^= operand;
			break;
		case 2: // bst
			registers['B'] = value % 8;
			break;
		case 3: // jnz
			if ( registers['A'] != 0 ) {
				ip = operand;
			}
			break;
		case 4:
			registers['B'] ^= registers['C'];
			break;
		case 5:
			out.push_back(value & 7);
			break;
		case 6:
			registers['B'] = registers['A'] >> value;
			break;
		case 7:
			registers['C'] = registers['A'] >> value;
			break;
		default:
			throw runtime_error("Error");
		}
	}
	return out;
}

void
part1(const tuple<registers_type, program_type>& data)
{
	auto [registers, program] = data;

	auto result = run(registers, program);

	copy(result.begin(), result.end(), ostream_iterator<unsigned long>(cout, ","));
	cout << endl;
}

optional<unsigned long>
find_register_a(const registers_type& registers, const program_type& program, size_t pos)
{
	if ( pos > program.size() ) {
		return registers.at('A');
	}

	for ( unsigned long i = 0; i != 8; ++i ) {
		auto registers_copy = registers;

		registers_copy['A'] <<= 3UL;
		registers_copy['A'] |= i;

		const auto try_run = run(registers_copy, program);

		if ( try_run.front() == program[program.size() - pos] ) {
			if ( auto result = find_register_a(registers_copy, program, pos + 1) ) {
				return *result;
			}
		}
	}
	return nullopt;
}

void
part2(const tuple<registers_type, program_type>& data)
{
	auto [registers, program] = data;

	registers['A'] = 0;
	if ( auto result = find_register_a(registers, program, 1) ) {
		cout << *result << endl;
	}
}

int
main()
{
	auto data = read_file("data/day17.txt");
	part1(data);
	part2(data);
}
