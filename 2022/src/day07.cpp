#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <string>
#include <vector>
using namespace std;

struct Node {
	Node* parent = nullptr;

	map<string, shared_ptr<Node>> sub;
	map<string, long>             files;
};

Node
read_file(string_view filename, ostream& debug)
{
	Node  root;
	Node* current = &root;

	fstream input{ filename };

	string line;
	getline(input, line);

	while ( input ) {
		if ( line.starts_with("$ ") ) {
			if ( line == "$ cd /" ) {
				debug << "cd root" << endl;
				current = &root;
				getline(input, line);
			}
			else if ( line == "$ cd .." ) {
				debug << "cd one up" << endl;
				current = current->parent;
				getline(input, line);
			}
			else if ( line == "$ ls" ) {
				debug << "list dir" << endl;
				getline(input, line);

				while ( input && !line.starts_with("$ ") ) {
					if ( line.starts_with("dir ") ) {
						const auto name    = line.substr(4);
						const auto node    = make_shared<Node>();
						node->parent       = current;
						current->sub[name] = node;
						debug << "new subdir " << name << endl;
					}
					else {
						const auto pos       = line.find(' ');
						const auto size      = stol(line.substr(0, pos));
						const auto name      = line.substr(pos + 1);
						current->files[name] = size;
						debug << "new file " << name << " (" << size << ")" << endl;
					}

					getline(input, line);
				}
			}
			else if ( line.starts_with("$ cd ") ) {
				const auto dirname = line.substr(5);
				current            = current->sub[dirname].get();
				debug << "cd into " << dirname << endl;
				getline(input, line);
			}
			else {
				cerr << "error in line: " << line << endl;
				exit(EXIT_FAILURE);
			}
		}
		else {
			cerr << "error II in line: " << line << endl;
			exit(EXIT_FAILURE);
		}
	}

	return root;
}

void
print(const Node& node, const string& indent = "")
{
	if ( node.parent == nullptr ) {
		cout << indent << "/" << endl;
	}

	for ( const auto& [name, sub]: node.sub ) {
		cout << indent << name << endl;
		print(*sub.get(), indent + "  ");
	}

	for ( const auto& [filename, filesize]: node.files ) {
		cout << indent << filename << " (" << filesize << ")" << endl;
	}
}

long
part1_rec(const Node& node, long& all)
{
	long size = 0;

	for ( const auto& [name, sub]: node.sub ) {
		const auto sub_size = part1_rec(*sub.get(), all);
		if ( sub_size <= 100000 ) {
			all += sub_size;
		}
		size += sub_size;
	}

	for ( const auto& [filename, filesize]: node.files ) {
		size += filesize;
	}

	return size;
}

void
part1(const Node& node)
{
	long all = 0;
	part1_rec(node, all);
	cout << all << endl;
}

long
part2_rec(const Node& node, vector<long>& sizes)
{
	long size = 0;

	for ( const auto& [name, sub]: node.sub ) {
		const auto sub_size = part2_rec(*sub.get(), sizes);
		sizes.emplace_back(sub_size);
		size += sub_size;
	}

	for ( const auto& [filename, filesize]: node.files ) {
		size += filesize;
	}

	return size;
}

void
part2(const Node& node)
{
	vector<long> sizes;
	const auto   needed = 30000000 - (70000000 - part2_rec(node, sizes));

	vector<long> candidates;
	copy_if(begin(sizes), end(sizes), back_inserter(candidates), [&](auto size) {
		return size >= needed;
	});

	sort(begin(candidates), end(candidates));
	cout << candidates[0] << endl;
}

int
main()
{
	struct NullBuffer : public streambuf {
		int overflow(int chr) final { return chr; }
	};

	NullBuffer nullBuffer;
	ostream    nullStream(&nullBuffer);

	const auto node = read_file("data/day07.txt", nullStream);

	part1(node);
	part2(node);
}
