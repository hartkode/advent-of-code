#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

struct Sue {
	optional<long> children;
	optional<long> cats;
	optional<long> samoyeds;
	optional<long> pomeranians;
	optional<long> akitas;
	optional<long> vizslas;
	optional<long> goldfish;
	optional<long> trees;
	optional<long> cars;
	optional<long> perfumes;
};

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

auto
read_file(string_view filename)
{
	fstream     input{ filename };
	vector<Sue> records;

	auto invalidChar = [](char chr) {
		return chr == ':' || chr == ',';
	};

	for ( string line; getline(input, line); ) {
		line.erase(remove_if(line.begin(), line.end(), invalidChar), line.end());
		auto parts = split(line);

		Sue sue;
		for ( size_t idx = 2; idx != parts.size(); idx += 2 ) {
			auto key   = parts[idx];
			auto value = stol(parts[idx + 1]);
			if ( key == "children" ) {
				sue.children = value;
			}
			else if ( key == "cats" ) {
				sue.cats = value;
			}
			else if ( key == "samoyeds" ) {
				sue.samoyeds = value;
			}
			else if ( key == "pomeranians" ) {
				sue.pomeranians = value;
			}
			else if ( key == "akitas" ) {
				sue.akitas = value;
			}
			else if ( key == "vizslas" ) {
				sue.vizslas = value;
			}
			else if ( key == "goldfish" ) {
				sue.goldfish = value;
			}
			else if ( key == "trees" ) {
				sue.trees = value;
			}
			else if ( key == "cars" ) {
				sue.cars = value;
			}
			else if ( key == "perfumes" ) {
				sue.perfumes = value;
			}
			else {
				cerr << "unknown property: " << key << ": " << value << endl;
			}
		}
		records.emplace_back(sue);
	}
	return records;
}

void
part1(const vector<Sue>& sues)
{
	Sue testSue{ 3, 7, 2, 3, 0, 0, 5, 3, 2, 1 }; // NOLINT

	for ( size_t idx = 0; idx != sues.size(); ++idx ) {
		const auto& sue = sues[idx];
		if ( (!sue.children.has_value() || sue.children == testSue.children) &&
		     (!sue.cats.has_value() || sue.cats == testSue.cats) &&
		     (!sue.samoyeds.has_value() || sue.samoyeds == testSue.samoyeds) &&
		     (!sue.pomeranians.has_value() || sue.pomeranians == testSue.pomeranians) &&
		     (!sue.akitas.has_value() || sue.akitas == testSue.akitas) &&
		     (!sue.vizslas.has_value() || sue.vizslas == testSue.vizslas) &&
		     (!sue.goldfish.has_value() || sue.goldfish == testSue.goldfish) &&
		     (!sue.trees.has_value() || sue.trees == testSue.trees) &&
		     (!sue.cars.has_value() || sue.cars == testSue.cars) &&
		     (!sue.perfumes.has_value() || sue.perfumes == testSue.perfumes) ) {
			cout << idx + 1 << endl;
		}
	}
}

void
part2(const vector<Sue>& sues)
{
	Sue testSue{ 3, 7, 2, 3, 0, 0, 5, 3, 2, 1 }; // NOLINT

	for ( size_t idx = 0; idx != sues.size(); ++idx ) {
		const auto& sue = sues[idx];
		if ( (!sue.children.has_value() || sue.children == testSue.children) &&
		     (!sue.cats.has_value() || sue.cats > testSue.cats) &&
		     (!sue.samoyeds.has_value() || sue.samoyeds == testSue.samoyeds) &&
		     (!sue.pomeranians.has_value() || sue.pomeranians < testSue.pomeranians) &&
		     (!sue.akitas.has_value() || sue.akitas == testSue.akitas) &&
		     (!sue.vizslas.has_value() || sue.vizslas == testSue.vizslas) &&
		     (!sue.goldfish.has_value() || sue.goldfish < testSue.goldfish) &&
		     (!sue.trees.has_value() || sue.trees > testSue.trees) &&
		     (!sue.cars.has_value() || sue.cars == testSue.cars) &&
		     (!sue.perfumes.has_value() || sue.perfumes == testSue.perfumes) ) {
			cout << idx + 1 << endl;
		}
	}
}

int
main()
{
	auto sues = read_file("data/day16.txt");
	part1(sues);
	part2(sues);
}
