#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <ranges>
#include <string>
#include <vector>
using namespace std;

vector<int>
read_file(string_view filename)
{
	fstream input{ filename };
	string  line;
	int     fid = 0;

	getline(input, line);

	vector<int> data;
	for ( size_t i = 0; i != line.size(); ++i ) {
		vector<int> values(size_t(line.at(i) - '0'), (i % 2 == 0) ? fid++ : -1);
		data.insert(data.end(), values.begin(), values.end());
	}
	return data;
}

void
part1(const vector<int>& data)
{
	vector<int> result;

	auto lhs = data.begin();
	auto rhs = data.end();

	while ( lhs != rhs ) {
		if ( *lhs != -1 ) {
			result.push_back(*lhs);
		}
		else {
			--rhs;
			while ( *rhs == -1 ) {
				--rhs;
			}
			result.push_back(*rhs);
		}
		++lhs;
	}

	long sum = 0;
	for ( size_t i = 0; i != result.size(); ++i ) {
		sum += long(i) * result.at(i);
	}
	cout << sum << endl;
}

void
part2(const vector<int>& data)
{
	// id, pos, len
	vector<tuple<int, size_t, size_t>> files;

	// pos, len
	list<tuple<size_t, size_t>> spaces;

	for ( size_t i = 0; i != data.size(); ) {
		// skip space
		auto j = i;
		while ( j != data.size() && data.at(j) == -1 ) {
			++j;
		}
		if ( i != j ) {
			spaces.emplace_back(i, j - i);
		}
		i = j;

		while ( j != data.size() && data.at(i) == data.at(j) ) {
			++j;
		}
		if ( i != j ) {
			files.emplace_back(data.at(i), i, j - i);
		}
		i = j;
	}

	for ( auto& file: std::ranges::reverse_view(files) ) {
		for ( auto it = spaces.begin(); it != spaces.end(); ++it ) {
			if ( get<0>(*it) >= get<1>(file) ) {
				spaces.erase(it, spaces.end());
				break;
			}
			if ( get<1>(*it) >= get<2>(file) ) {
				// update pos
				get<1>(file) = get<0>(*it);

				// update space len
				get<1>(*it) -= get<2>(file);

				// update space pos
				get<0>(*it) += get<2>(file);

				if ( get<1>(*it) == 0 ) {
					spaces.erase(it);
				}
				break;
			}
		}
	}

	long sum = 0;
	for ( const auto& [fid, pos, size]: files ) {
		for ( size_t x = pos; x != pos + size; ++x ) {
			sum += long(x) * fid;
		}
	}
	cout << sum << endl;
}

int
main()
{
	auto data = read_file("data/day09.txt");
	part1(data);
	part2(data);
}
