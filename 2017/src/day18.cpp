#include <charconv>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
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

	void add(const string& ref, T value)
	{
		memory[ref] += value;
	}

	void mul(const string& ref, T value)
	{
		memory[ref] *= value;
	}

	void mod(const string& ref, T value)
	{
		memory[ref] %= value;
	}

	T execute(const vector<vector<string>>& code)
	{
		long played_sound{};

		for ( size_t ip = 0; ip < code.size(); ++ip ) {
			const auto& line   = code.at(ip);
			const auto& opcode = line.at(0);

			if ( opcode == "snd" ) {
				played_sound = get(line.at(1));
			}
			else if ( opcode == "rcv" ) {
				auto value = get(line.at(1));
				if ( value != 0 ) {
					return played_sound;
				}
			}
			else if ( opcode == "set" ) {
				set(line.at(1), get(line.at(2)));
			}
			else if ( opcode == "add" ) {
				add(line.at(1), get(line.at(2)));
			}
			else if ( opcode == "mul" ) {
				mul(line.at(1), get(line.at(2)));
			}
			else if ( opcode == "mod" ) {
				mod(line.at(1), get(line.at(2)));
			}
			else if ( opcode == "jgz" ) {
				auto value = get(line.at(1));
				if ( value > 0 ) {
					auto offset = get(line.at(2));
					ip += static_cast<size_t>(offset - 1);
				}
			}
			else {
				cerr << "unknown opcode: " << opcode << '\n';
				return -1;
			}
		}
		return -1;
	}

	map<string, T> memory;
};

template<typename T>
struct CPU2 {
	CPU2(queue<T>& in, queue<T>& out)
	    : in{ in }
	    , out{ out }
	{
	}

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

	void add(const string& ref, T value)
	{
		memory[ref] += value;
	}

	void mul(const string& ref, T value)
	{
		memory[ref] *= value;
	}

	void mod(const string& ref, T value)
	{
		memory[ref] %= value;
	}

	bool step(const vector<vector<string>>& code)
	{
		if ( ip >= code.size() ) {
			return false;
		}

		const auto& line   = code.at(ip);
		const auto& opcode = line.at(0);

		if ( opcode == "snd" ) {
			out.push(get(line.at(1)));
			++send_count;
		}
		else if ( opcode == "rcv" ) {
			if ( in.empty() ) {
				waiting = true;
				return true;
			}
			else {
				auto value = in.front();
				in.pop();
				set(line.at(1), value);
				waiting = false;
			}
		}
		else if ( opcode == "set" ) {
			set(line.at(1), get(line.at(2)));
		}
		else if ( opcode == "add" ) {
			add(line.at(1), get(line.at(2)));
		}
		else if ( opcode == "mul" ) {
			mul(line.at(1), get(line.at(2)));
		}
		else if ( opcode == "mod" ) {
			mod(line.at(1), get(line.at(2)));
		}
		else if ( opcode == "jgz" ) {
			auto value = get(line.at(1));
			if ( value > 0 ) {
				auto offset = get(line.at(2));
				ip += static_cast<size_t>(offset);
				return true;
			}
		}
		else {
			cerr << "unknown opcode: " << opcode << '\n';
			return false;
		}

		++ip;

		return true;
	}

	size_t         ip{};
	queue<T>&      in;
	queue<T>&      out;
	bool           waiting{ false };
	T              send_count{};
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
	queue<long> queue1;
	queue<long> queue2;
	CPU2<long>  cpu1{ queue1, queue2 };
	CPU2<long>  cpu2{ queue2, queue1 };

	cpu1.set("p", 0);
	cpu2.set("p", 1);

	for ( ;; ) {
		auto cpu1_running = cpu1.step(code);
		auto cpu2_running = cpu2.step(code);

		if ( (cpu1.waiting || !cpu1_running) && (cpu2.waiting || !cpu2_running) ) {
			break;
		}
	}
	cout << "Part2: " << cpu2.send_count << '\n';
}

} // namespace

int
main()
{
	auto instr = read_file("data/day18.txt");
	part1(instr);
	part2(instr);
}
