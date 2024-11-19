#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
using namespace std;

auto
split(const string& line, char sep = ' ')
{
	vector<string> parts;
	stringstream   input{ line };

	for ( string part; getline(input, part, sep); ) {
		parts.emplace_back(part);
	}

	return parts;
}

tuple<string, vector<tuple<string, string>>>
read_file(string_view filename)
{
	fstream                       input{ filename };
	vector<tuple<string, string>> table;
	string                        molecule;

	for ( string line; getline(input, line); ) {
		auto parts = split(line);

		if ( parts.size() == 3 && parts[1] == "=>" ) {
			table.emplace_back(parts[0], parts[2]);
		}

		if ( parts.size() == 1 ) {
			molecule = parts[0];
		}
	}
	return { molecule, table };
}

void
part1(const string& molecule, const vector<tuple<string, string>>& table)
{
	set<string> set;
	for ( const auto& [left, right]: table ) {
		size_t pos = 0;
		while ( true ) {
			string temp = molecule;
			pos         = temp.find(left, pos);
			if ( pos == string::npos ) {
				break;
			}
			temp.replace(pos, left.length(), right);
			++pos;

			set.insert(temp);
		}
	}
	cout << set.size() << endl;
}

size_t
replace_all(string& str, string_view replace, string_view replaceWith)
{
	size_t count = 0;
	auto   pos   = str.find(replace);

	while ( pos != string::npos ) {
		str.replace(pos, replace.size(), replaceWith);
		++count;
		pos = str.find(replace, pos + replace.size());
	}

	return count;
}

void
part2(string molecule)
{
	replace_all(molecule, "Rn", "(");
	replace_all(molecule, "Y", ",");
	replace_all(molecule, "Ar", ")");

	molecule.erase(remove_if(molecule.begin(), molecule.end(), [](auto chr) -> bool {
		               return islower(chr);
	               }),
	               molecule.end());

	transform(molecule.begin(), molecule.end(), molecule.begin(), [](auto chr) {
		return isalpha(chr) ? 'X' : chr;
	});

	size_t count = 0;
	while ( molecule != "X" ) {
		for ( const auto& item: { "XX", "X(X)", "X(X,X)", "X(X,X,X)" } ) {
			count += replace_all(molecule, item, "X");
		}
	}

	cout << count << endl;
}

int
main()
{
	const auto [molecule, table] = read_file("data/day19.txt");
	part1(molecule, table);
	part2(molecule);
}
