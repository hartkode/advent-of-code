#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

auto
split(string_view line, string_view delimiter)
{
	size_t pos_start = 0;
	size_t pos_end   = 0;

	vector<string> res;

	while ( (pos_end = line.find(delimiter, pos_start)) != string::npos ) {
		auto token = line.substr(pos_start, pos_end - pos_start);
		pos_start  = pos_end + delimiter.length();

		res.emplace_back(token);
	}

	if ( pos_start != line.size() ) {
		res.emplace_back(line.substr(pos_start));
	}
	return res;
}

auto
read_file(string_view filename)
{
	fstream    input{ filename };
	const auto passports = split(string{ istreambuf_iterator<char>{ input }, istreambuf_iterator<char>{} }, "\n\n");

	vector<map<string, string>> result;

	for ( const auto& passport: passports ) {
		map<string, string> bar;
		stringstream        foo{ passport };
		for ( string keyval; foo >> keyval; ) {
			auto colon = keyval.find(':');
			auto key   = keyval.substr(0, colon);
			auto val   = keyval.substr(colon + 1);
			bar[key]   = val;
		}
		result.emplace_back(bar);
	}

	return result;
}

auto
contains_all_fields(const map<string, string>& passport)
{
	static const array<string, 7> keys = { "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid" };
	return all_of(keys.begin(), keys.end(), [&](auto key) { return passport.contains(key); });
}

void
part1(const vector<map<string, string>>& passports)
{
	cout << count_if(passports.begin(), passports.end(), contains_all_fields) << endl;
}

void
part2(const vector<map<string, string>>& passports)
{
	auto check_birth_year = [](const map<string, string>& passport) {
		auto year = stoi(passport.at("byr"));
		return 1920 <= year && year <= 2002;
	};

	auto check_issue_year = [](const map<string, string>& passport) {
		auto year = stoi(passport.at("iyr"));
		return 2010 <= year && year <= 2020;
	};

	auto check_expiration_year = [](const map<string, string>& passport) {
		auto year = stoi(passport.at("eyr"));
		return 2020 <= year && year <= 2030;
	};

	auto check_height = [](const map<string, string>& passport) {
		auto height_str = passport.at("hgt");
		auto height     = stoi(height_str);
		return (height_str.ends_with("in") && 59 <= height && height <= 76) ||
		       (height_str.ends_with("cm") && 150 <= height && height <= 193);
	};

	auto check_hair_color = [](const map<string, string>& passport) {
		auto hair = passport.at("hcl");
		return hair.length() == 7 && hair.starts_with('#') && hair.find_first_not_of("0123456789abcdef", 1) == string::npos;
	};

	auto check_eye_color = [](const map<string, string>& passport) {
		const array<string, 7> valid = { "amb", "blu", "brn", "gry", "grn", "hzl", "oth" };
		return find(valid.begin(), valid.end(), passport.at("ecl")) != valid.end();
	};

	auto check_passport_id = [](const map<string, string>& passport) {
		auto pid = passport.at("pid");
		return pid.length() == 9 && pid.find_first_not_of("0123456789") == string::npos;
	};

	const array<function<bool(const map<string, string>&)>, 8> checks = {
		contains_all_fields,
		check_birth_year,
		check_issue_year,
		check_expiration_year,
		check_height,
		check_hair_color,
		check_eye_color,
		check_passport_id
	};

	cout << count_if(passports.begin(), passports.end(), [&checks](const map<string, string>& passport) {
		return all_of(checks.begin(), checks.end(), [&passport](auto check) { return check(passport); });
	}) << endl;
}

int
main()
{
	auto passports = read_file("data/day04.txt");

	part1(passports);
	part2(passports);
}
