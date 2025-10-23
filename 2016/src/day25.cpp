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
	OUT,
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
		else if ( words.at(0) == "out" ) {
			instructions.emplace_back(OpCode::OUT, Argument(words.at(1)));
		}
		else {
			cerr << "unknown opcode..." << '\n';
		}
	}

	return instructions;
}

#define JIT

string
execute(memory_type& memory, vector<Instruction> instructions)
{
	string result;

	for ( size_t ip = 0; ip < instructions.size(); ) {
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
		case OUT:
			result += to_string(instruction.x->eval(memory));
			if ( result.size() >= 20 ) {
				ip = instructions.size();
			}
			else {
				++ip;
			}
		}
	}

	return result;
}

void
part1(const vector<Instruction>& instructions)
{
	for ( int seed = 0;; ++seed ) {
		map<char, int> memory;

		memory['a'] = seed;
		memory['b'] = 0;
		memory['c'] = 0;
		memory['d'] = 0;

		auto result = execute(memory, instructions);

		if ( result == "01010101010101010101" ) {
			cout << "Part1: " << seed << '\n';
			return;
		}
	}
}

} // namespace

int
main()
{
	const auto instructions = read_file("data/day25.txt");
	try {
		part1(instructions);
	}
	catch ( exception& e ) {
		cerr << e.what() << '\n';
	}
}
