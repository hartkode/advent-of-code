#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
using namespace std;

enum Instruction {
	Half_A,
	Half_B,
	Triple_A,
	Triple_B,
	Inc_A,
	Inc_B,
	Jmp,
	Jmp_Even_A,
	Jmp_Even_B,
	Jmp_One_A,
	Jmp_One_B
};

vector<tuple<Instruction, int>>
read_file(string_view filename)
{
	fstream input{ filename };

	vector<tuple<Instruction, int>> instructions;
	for ( string line; getline(input, line); ) {
		if ( line == "hlf a" ) {
			instructions.emplace_back(Half_A, 0);
		}
		else if ( line == "hlf b" ) {
			instructions.emplace_back(Half_B, 0);
		}
		else if ( line == "tpl a" ) {
			instructions.emplace_back(Triple_A, 0);
		}
		else if ( line == "tpl b" ) {
			instructions.emplace_back(Triple_B, 0);
		}
		else if ( line == "inc a" ) {
			instructions.emplace_back(Inc_A, 0);
		}
		else if ( line == "inc b" ) {
			instructions.emplace_back(Inc_B, 0);
		}
		else if ( line.starts_with("jmp ") ) {
			auto offset = stoi(line.substr(4));
			instructions.emplace_back(Jmp, offset);
		}
		else if ( line.starts_with("jie a, ") ) {
			auto offset = stoi(line.substr(7));
			instructions.emplace_back(Jmp_Even_A, offset);
		}
		else if ( line.starts_with("jie b, ") ) {
			auto offset = stoi(line.substr(7));
			instructions.emplace_back(Jmp_Even_B, offset);
		}
		else if ( line.starts_with("jio a, ") ) {
			auto offset = stoi(line.substr(7));
			instructions.emplace_back(Jmp_One_A, offset);
		}
		else if ( line.starts_with("jio b, ") ) {
			auto offset = stoi(line.substr(7));
			instructions.emplace_back(Jmp_One_B, offset);
		}
		else {
			cerr << "unknown instruction: " << line << endl;
		}
	}
	return instructions;
}

int
eval(const vector<tuple<Instruction, int>>& instructions, int a, int b)
{
	for ( size_t ip = 0; ip < instructions.size(); ) {
		const auto [instruction, offset] = instructions[ip++];
		switch ( instruction ) {
		case Half_A:
			a /= 2;
			break;
		case Half_B:
			b /= 2;
			break;
		case Triple_A:
			a *= 3;
			break;
		case Triple_B:
			b *= 3;
			break;
		case Inc_A:
			a++;
			break;
		case Inc_B:
			b++;
			break;
		case Jmp:
			ip = (ip - 1) + size_t(offset);
			break;
		case Jmp_Even_A:
			if ( a % 2 == 0 ) {
				ip = (ip - 1) + size_t(offset);
			}
			break;
		case Jmp_Even_B:
			if ( b % 2 == 0 ) {
				ip = (ip - 1) + size_t(offset);
			}
			break;
		case Jmp_One_A:
			if ( a == 1 ) {
				ip = (ip - 1) + size_t(offset);
			}
			break;
		case Jmp_One_B:
			if ( b == 1 ) {
				ip = (ip - 1) + size_t(offset);
			}
			break;
		}
	}
	return b;
}

void
part1(const vector<tuple<Instruction, int>>& instructions)
{
	cout << eval(instructions, 0, 0) << endl;
}

void
part2(const vector<tuple<Instruction, int>>& instructions)
{
	cout << eval(instructions, 1, 0) << endl;
}

int
main()
{
	auto instructions = read_file("data/day23.txt");
	part1(instructions);
	part2(instructions);
}
