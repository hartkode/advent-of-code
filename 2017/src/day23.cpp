#include <charconv>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace {

template<typename T>
struct CPU {
	[[nodiscard]]
	T get(const string& ref)
	{
		T value{};
		auto [ptr, ec] = from_chars(ref.data(), ref.data() + ref.size(), value);

		if ( ec == errc() ) {
			return value;
		}

		return memory[ref];
	}

	void set(const string& ref, T value)
	{
		memory[ref] = value;
	}

	void sub(const string& ref, T value)
	{
		memory[ref] -= value;
	}

	void mul(const string& ref, T value)
	{
		memory[ref] *= value;
	}

	T execute(const vector<vector<string>>& code)
	{
		int mul_executed = 0;

		for ( size_t ip = 0; ip < code.size(); ++ip ) {
			const auto& line   = code.at(ip);
			const auto& opcode = line.at(0);

			if ( opcode == "set" ) {
				set(line.at(1), get(line.at(2)));
			}
			else if ( opcode == "sub" ) {
				sub(line.at(1), get(line.at(2)));
			}
			else if ( opcode == "mul" ) {
				++mul_executed;
				mul(line.at(1), get(line.at(2)));
			}
			else if ( opcode == "jnz" ) {
				auto value = get(line.at(1));
				if ( value != 0 ) {
					auto offset = get(line.at(2));
					ip += static_cast<size_t>(offset - 1);
				}
			}
			else {
				cerr << "unknown opcode: " << opcode << '\n';
				return -1;
			}
		}
		return mul_executed;
	}

	map<string, T> memory;
};

vector<string>
split(const string& line)
{
	stringstream   strm{ line };
	vector<string> data;

	for ( string line; strm >> line; ) {
		data.emplace_back(line);
	}

	return data;
}

vector<vector<string>>
read_file(const filesystem::path& filename)
{
	ifstream               file{ filename };
	vector<vector<string>> data;

	for ( string line; getline(file, line); ) {
		const auto parts = split(line);
		data.emplace_back(parts);
	}

	return data;
}

void
part1(const vector<vector<string>>& code)
{
	CPU<long> cpu{};

	cout << "Part1: " << cpu.execute(code) << '\n';
}

void
part2(const vector<vector<string>>& code)
{
	CPU<long> cpu{};
	cpu.memory["a"] = 1;

	// execute only initialization code
	cpu.execute({ code.begin(), code.begin() + 8 });

	auto b_start = cpu.memory["b"];
	auto c_end   = cpu.memory["c"];

	auto is_prime = [](long n) {
		if ( n < 2 ) {
			return false;
		}
		for ( long i = 2; i * i <= n; ++i ) {
			if ( n % i == 0 ) {
				return false;
			}
		}
		return true;
	};

	const long step     = 17;
	long       memory_h = 0;
	for ( long b = b_start; b <= c_end; b += step ) {
		if ( !is_prime(b) ) {
			++memory_h;
		}
	}

	cout << "Part2: " << memory_h << '\n';
}

} // namespace

int
main()
{
	auto instr = read_file("data/day23.txt");
	part1(instr);
	part2(instr);
}
