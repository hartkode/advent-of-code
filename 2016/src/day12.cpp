#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <vector>

using namespace std;

namespace {

enum class OpCode : uint8_t {
	CPY_VAL,
	CPY_REG,
	INC,
	DEC,
	JMP,
	JNZ,
};

struct Instruction {
	Instruction(OpCode opCode, char reg)
	    : opCode{ opCode }
	    , reg{ reg }
	{
	}

	Instruction(OpCode opCode, int val)
	    : opCode{ opCode }
	    , val{ val }
	{
	}

	Instruction(OpCode opCode, char reg, int val)
	    : opCode{ opCode }
	    , reg{ reg }
	    , val{ val }
	{
	}

	OpCode         opCode;
	optional<char> reg;
	optional<int>  val;
};

/*
ostream&
operator<<(ostream& os, const Instruction& instruction)
{
    cout << (int) instruction.opCode << ":";
    if ( instruction.reg.has_value() ) {
        cout << " reg: " << instruction.reg.value();
    }
    if ( instruction.val.has_value() ) {
        cout << " val: " << instruction.val.value();
    }
    return os;
}
*/

vector<Instruction>
read_file(const filesystem::path& filename)
{
	ifstream input{ filename };

	vector<Instruction> instructions;

	for ( string line; getline(input, line); ) {
		if ( line.starts_with("cpy") ) {
			if ( isalpha(line.at(4)) != 0 ) {
				auto from_reg = line.at(4);
				auto to_reg   = line.at(6);
				instructions.emplace_back(OpCode::CPY_REG, to_reg, from_reg);
			}
			else {
				auto val = stoi(&line.at(strlen("cpy ")));
				auto reg = line.back();
				instructions.emplace_back(OpCode::CPY_VAL, reg, val);
			}
		}
		else if ( line.starts_with("inc") ) {
			auto reg = line.at(4);
			instructions.emplace_back(OpCode::INC, reg);
		}
		else if ( line.starts_with("dec") ) {
			auto reg = line.at(4);
			instructions.emplace_back(OpCode::DEC, reg);
		}
		else if ( line.starts_with("jnz 1") ) { // always jump!, WTF??
			auto off = stoi(&line.at(strlen("jnz 1 ")));
			instructions.emplace_back(OpCode::JMP, off);
		}
		else if ( line.starts_with("jnz") ) {
			auto reg = line.at(4);
			auto off = stoi(&line.at(strlen("jnz a ")));
			instructions.emplace_back(OpCode::JNZ, reg, off);
		}
		else {
			cerr << "unknown opcode..." << '\n';
		}

		// cout << line << " -> " << instructions.back() << '\n';
	}

	return instructions;
}

void
execute(map<char, int>& memory, const vector<Instruction>& instructions)
{
	for ( size_t ip = 0; ip < instructions.size(); ) {
		auto instruction = instructions.at(ip);

		// cout << "#" << ip << " - " << memory['a'] << ", " << memory['b'] << ", " << memory['c'] << ", " << memory['d'] << ": " << (int) instruction.opCode << '\n';

		switch ( instruction.opCode ) {
			using enum OpCode;
		case CPY_VAL:
			memory[instruction.reg.value()] = instruction.val.value();
			++ip;
			break;
		case CPY_REG:
			memory[instruction.reg.value()] = memory[static_cast<char>(instruction.val.value())];
			++ip;
			break;
		case INC:
			memory[instruction.reg.value()]++;
			++ip;
			break;
		case DEC:
			memory[instruction.reg.value()]--;
			++ip;
			break;
		case JMP:
			ip += static_cast<size_t>(instruction.val.value());
			break;
		case JNZ:
			if ( memory[instruction.reg.value()] != 0 ) {
				ip += static_cast<size_t>(instruction.val.value());
			}
			else {
				++ip;
			}
			break;
		}
	}
}

void
part1(const vector<Instruction>& instructions)
{
	map<char, int> memory;

	memory['a'] = 0;
	memory['b'] = 0;
	memory['c'] = 0;
	memory['d'] = 0;

	execute(memory, instructions);

	cout << "Part1: " << memory['a'] << '\n';
}

void
part2(const vector<Instruction>& instructions)
{
	map<char, int> memory;

	memory['a'] = 0;
	memory['b'] = 0;
	memory['c'] = 1;
	memory['d'] = 0;

	execute(memory, instructions);

	cout << "Part2: " << memory['a'] << '\n';
}

} // namespace

int
main()
{
	const auto instructions = read_file("data/day12.txt");
	part1(instructions);
	part2(instructions);
}
