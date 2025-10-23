#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace {

void
swap_at_pos(string& str, size_t x, size_t y)
{
	if ( str.empty() ) {
		return;
	}

	swap(str.at(x), str.at(y));
}

void
swap_letter(string& str, char x, char y)
{
	if ( str.empty() ) {
		return;
	}

	for ( auto& chr: str ) {
		if ( chr == x ) {
			chr = y;
		}
		else if ( chr == y ) {
			chr = x;
		}
	}
}

void
rotate_left(string& str, size_t n)
{
	if ( str.empty() ) {
		return;
	}

	ranges::rotate(str, str.begin() + static_cast<int>(n % str.size()));
}

void
rotate_right(string& str, size_t n)
{
	if ( str.empty() ) {
		return;
	}

	rotate_left(str, str.size() - n);
}

void
rotate(string& str, char chr)
{
	if ( str.empty() ) {
		return;
	}

	const auto idx = str.find(chr);
	if ( idx == string::npos ) {
		return;
	}

	const auto steps = 1 + idx + (idx >= 4 ? 1 : 0);
	rotate_right(str, steps % str.size());
}

void
reverse(string& str, size_t x, size_t y)
{
	if ( str.empty() ) {
		return;
	}

	if ( x > y ) {
		swap(x, y);
	}

	reverse(str.begin() + static_cast<string::difference_type>(x),
	        str.begin() + static_cast<string::difference_type>(y) + 1);
}

void
move(string& str, size_t x, size_t y)
{
	if ( str.empty() ) {
		return;
	}

	const auto chr = str.at(x);

	str.erase(x, 1);
	str.insert(y, 1, chr);
}

vector<string>
readLines(const filesystem::path& filename)
{
	ifstream       file{ filename };
	vector<string> lines;

	for ( string line; getline(file, line); ) {
		lines.emplace_back(line);
	}

	return lines;
}

vector<string>
split(const string& line, char sep = ' ')
{
	vector<string> words;
	stringstream   input{ line };

	for ( string word; getline(input, word, sep); ) {
		words.emplace_back(word);
	}

	return words;
}

void
part1(const vector<string>& lines, string str)
{
	for ( const auto& line: lines ) {
		auto words = split(line);

		if ( line.starts_with("swap position") ) {
			swap_at_pos(str, stoull(words.at(2)), stoull(words.at(5)));
		}
		else if ( line.starts_with("swap letter") ) {
			swap_letter(str, words.at(2).at(0), words.at(5).at(0));
		}
		else if ( line.starts_with("rotate left") ) {
			rotate_left(str, stoull(words.at(2)));
		}
		else if ( line.starts_with("rotate right") ) {
			rotate_right(str, stoull(words.at(2)));
		}
		else if ( line.starts_with("rotate based") ) {
			rotate(str, words.at(6).at(0));
		}
		else if ( line.starts_with("reverse") ) {
			reverse(str, stoull(words.at(2)), stoull(words.at(4)));
		}
		else if ( line.starts_with("move") ) {
			move(str, stoull(words.at(2)), stoull(words.at(5)));
		}
		else {
			cerr << "unknown command: " << line << '\n';
			return;
		}
	}
	cout << str << '\n';
}

void
part2(const vector<string>& lines, string str)
{
	for ( const auto& line: ranges::reverse_view(lines) ) {
		auto words = split(line);

		if ( line.starts_with("swap position") ) {
			swap_at_pos(str, stoull(words.at(2)), stoull(words.at(5)));
		}
		else if ( line.starts_with("swap letter") ) {
			swap_letter(str, words.at(2).at(0), words.at(5).at(0));
		}
		else if ( line.starts_with("rotate left") ) {
			rotate_right(str, stoull(words.at(2)));
		}
		else if ( line.starts_with("rotate right") ) {
			rotate_left(str, stoull(words.at(2)));
		}
		else if ( line.starts_with("rotate based") ) {
			auto chr = words.at(6).at(0);
			for ( size_t i = 0; i != str.size(); ++i ) {
				string tmp = str;
				rotate_left(tmp, i);

				auto idx   = tmp.find(chr);
				auto steps = (1 + idx + (idx >= 4 ? 1 : 0));

				if ( ((steps - i + str.size()) % str.size()) == 0 ) {
					str = tmp;
					break;
				}
			}
		}
		else if ( line.starts_with("reverse") ) {
			reverse(str, stoull(words.at(2)), stoull(words.at(4)));
		}
		else if ( line.starts_with("move") ) {
			move(str, stoull(words.at(5)), stoull(words.at(2)));
		}
		else {
			cerr << "unknown command: " << line << '\n';
			return;
		}
	}
	cout << str << '\n';
}

} // namespace

int
main()
{
	auto lines = readLines("data/day21.txt");

	part1(lines, "abcdefgh");
	part2(lines, "fbgdceah");
}
