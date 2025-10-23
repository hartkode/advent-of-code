#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

namespace {

enum class OpCode : uint8_t {
	CPY,
	INC,
	DEC,
	JNZ,
	TGL
};

using memory_type = map<char, int>;

struct Argument {
	explicit Argument(string value)
	    : value{ std::move(value) }
	{
	}

	[[nodiscard]]
	int eval(const memory_type& memory) const
	{
		const auto chr = value.at(0);

		if ( chr >= 'a' && chr <= 'z' ) {
			return memory.at(chr);
		}
		else {
			return stoi(value);
		}
	}

	[[nodiscard]]
	char reg() const
	{
		if ( !is_reg() ) {
			throw runtime_error("invalid register access! "s + value);
		}

		return value.at(0);
	}

	[[nodiscard]]
	bool is_reg() const
	{
		const auto chr = value.at(0);

		return chr >= 'a' && chr <= 'z';
	}

	string value;
};

struct Instruction {
	Instruction(OpCode opCode, const Argument& x)
	    : opCode{ opCode }
	    , x{ x }
	{
	}

	Instruction(OpCode opCode, const Argument& x, const Argument& y)
	    : opCode{ opCode }
	    , x{ x }
	    , y{ y }
	{
	}

	OpCode             opCode;
	optional<Argument> x;
	optional<Argument> y;
};

vector<string>
split_words(const string& line, char sep = ' ')
{
	vector<string> words;
	stringstream   strm{ line };

	for ( string word; getline(strm, word, sep); ) {
		words.emplace_back(word);
	}

	return words;
}

vector<Instruction>
read_file(const filesystem::path& filename)
{
	ifstream input{ filename };

	vector<Instruction> instructions;

	for ( string line; getline(input, line); ) {
		const auto words = split_words(line);

		if ( words.at(0) == "cpy" ) {
			instructions.emplace_back(OpCode::CPY, Argument(words.at(1)), Argument(words.at(2)));
		}
		else if ( words.at(0) == "inc" ) {
			instructions.emplace_back(OpCode::INC, Argument(words.at(1)));
		}
		else if ( words.at(0) == "dec" ) {
			instructions.emplace_back(OpCode::DEC, Argument(words.at(1)));
		}
		else if ( words.at(0) == "jnz" ) {
			instructions.emplace_back(OpCode::JNZ, Argument(words.at(1)), Argument(words.at(2)));
		}
		else if ( words.at(0) == "tgl" ) {
			instructions.emplace_back(OpCode::TGL, Argument(words.at(1)));
		}
		else {
			cerr << "unknown opcode..." << '\n';
		}
	}

	return instructions;
}

#define JIT

void
execute(memory_type& memory, vector<Instruction> instructions)
{
	for ( size_t ip = 0; ip < instructions.size(); ) {
#if defined(JIT)
		if ( ip + 2 < instructions.size() ) {
			const auto& i0 = instructions[ip];
			const auto& i1 = instructions[ip + 1];
			const auto& i2 = instructions[ip + 2];

			if ( i0.opCode == OpCode::INC &&
			     i1.opCode == OpCode::DEC &&
			     i2.opCode == OpCode::JNZ &&
			     i2.x && i2.x->value == i1.x->value &&
			     i2.y && i2.y->value == "-2" ) {
				char X = i0.x->reg();
				char Y = i1.x->reg();
				memory[X] += memory[Y];
				memory[Y] = 0;
				ip += 3;
				continue;
			}
		}
#endif

#if defined(JIT)
		if ( ip + 5 < instructions.size() ) {
			const auto& i0 = instructions[ip];
			const auto& i1 = instructions[ip + 1];
			const auto& i2 = instructions[ip + 2];
			const auto& i3 = instructions[ip + 3];
			const auto& i4 = instructions[ip + 4];
			const auto& i5 = instructions[ip + 5];

			if ( i0.opCode == OpCode::CPY &&
			     i1.opCode == OpCode::INC &&
			     i2.opCode == OpCode::DEC &&
			     i3.opCode == OpCode::JNZ &&
			     i4.opCode == OpCode::DEC &&
			     i5.opCode == OpCode::JNZ &&
			     i3.x && i3.y && i3.x->value == i2.x->value && i3.y->value == "-2" &&
			     i5.x && i5.y && i5.x->value == i4.x->value && i5.y->value == "-5" &&
			     i0.y->is_reg() && i1.x->is_reg() && i2.x->is_reg() && i4.x->is_reg() ) {
				char X    = i0.x->is_reg() ? i0.x->reg() : 0;
				int  valX = i0.x->is_reg() ? memory[X] : stoi(i0.x->value);

				char Y = i0.y->reg();
				char Z = i1.x->reg();
				char W = i4.x->reg();

				memory[Z] += valX * memory[W];
				memory[Y] = 0;
				memory[W] = 0;
				ip += 6;
				continue;
			}
		}
#endif

		const auto& instruction = instructions.at(ip);

		// cout << "#" << ip << " - " << memory['a'] << ", " << memory['b'] << ", " << memory['c'] << ", " << memory['d'] << ": " << (int) instruction.opCode << '\n';

		switch ( instruction.opCode ) {
			using enum OpCode;
		case CPY:
			memory[instruction.y->reg()] = instruction.x->eval(memory);
			++ip;
			break;
		case INC:
			memory[instruction.x->reg()]++;
			++ip;
			break;
		case DEC:
			memory[instruction.x->reg()]--;
			++ip;
			break;
		case JNZ:
			if ( instruction.x->eval(memory) != 0 ) {
				ip += static_cast<size_t>(instruction.y->eval(memory));
			}
			else {
				++ip;
			}
			break;
		case TGL:
			if ( auto foo = ip + static_cast<size_t>(instruction.x->eval(memory)); foo < instructions.size() ) {
				switch ( instructions[foo].opCode ) {
				case CPY:
					instructions[foo].opCode = JNZ;
					break;
				case INC:
					instructions[foo].opCode = DEC;
					break;
				case DEC:
					instructions[foo].opCode = INC;
					break;
				case JNZ:
					instructions[foo].opCode = CPY;
					break;
				case TGL:
					instructions[foo].opCode = INC;
					break;
				}
			}

			++ip;
			break;
		}
	}
}

void
part1(const vector<Instruction>& instructions)
{
	map<char, int> memory;

	memory['a'] = 7;
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

	memory['a'] = 12;
	memory['b'] = 0;
	memory['c'] = 0;
	memory['d'] = 0;

	execute(memory, instructions);

	cout << "Part2: " << memory['a'] << '\n';
}

} // namespace

int
main()
{
	const auto instructions = read_file("data/day23.txt");
	try {
		part1(instructions);
		part2(instructions);
	}
	catch ( exception& e ) {
		cerr << e.what() << '\n';
	}
}
