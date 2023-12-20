#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <string>
#include <vector>

using namespace std;

vector<string>
read_file(string_view filename)
{
	fstream        input{ filename };
	vector<string> data;

	for ( string line; getline(input, line); ) {
		data.emplace_back(line);
	}

	return data;
}

vector<string>
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

struct Module { // NOLINT
protected:
	explicit Module(string_view name, queue<tuple<string, int, string>>& queue)
	    : name_(name)
	    , queue_(queue)
	{
	}
	virtual ~Module() = default;

public:
	virtual void trigger(string_view sender, int signal) = 0;

	void register_sender(string_view sender) { senders_.emplace(sender, 0); }

protected:
	void send_signal(int signal) const
	{
		for ( const auto& link: links_ ) {
			queue_.emplace(name_, signal, link);
		}
	}

public:
	void add_link(string_view link) { links_.emplace_back(link); }

protected:
	const string                       name_;
	queue<tuple<string, int, string>>& queue_;

public:
	vector<string>   links_;
	map<string, int> senders_;
};

struct BroadcasterModuler : public Module {
	explicit BroadcasterModuler(string_view name, queue<tuple<string, int, string>>& queue)
	    : Module(name, queue)
	{
	}
	void trigger(string_view, int signal) override
	{
		send_signal(signal);
	}
};

struct FlipFlopModule : public Module {
	explicit FlipFlopModule(string_view name, queue<tuple<string, int, string>>& queue)
	    : Module(name, queue)
	{
	}
	void trigger(string_view, int signal) override
	{
		if ( signal == 1 ) {
			return;
		}
		state_ = (state_ + 1) % 2;
		send_signal(state_);
	}
	int state_{ 0 };
};

struct ConjunctionModule : public Module {
	explicit ConjunctionModule(string_view name, queue<tuple<string, int, string>>& queue)
	    : Module(name, queue)
	{
	}
	void trigger(string_view sender, int signal) override
	{
		senders_[string(sender)] = signal;
		if ( all_of(senders_.begin(), senders_.end(), [](const auto& link) { return link.second == 1; }) ) {
			send_signal(0);
		}
		else {
			send_signal(1);
		}
	}
};

struct OutputModule : public Module {
	explicit OutputModule(string_view name, queue<tuple<string, int, string>>& queue)
	    : Module(name, queue)
	{
	}
	void trigger(string_view, int) override
	{
	}
};

void
part1()
{
	queue<tuple<string, int, string>> queue;
	map<string, shared_ptr<Module>>   modules;

	modules["output"] = make_shared<OutputModule>("output", queue);

	const auto input = read_file("data/day20.txt");
	for ( const auto& line: input ) {
		const auto parts  = split(line, " -> ");
		auto       module = parts[0];
		const auto dests  = split(parts[1], ", ");

		shared_ptr<Module> ptr;
		if ( module == "broadcaster" ) {
			ptr = make_shared<BroadcasterModuler>(module, queue);
		}
		else if ( module.starts_with('%') ) {
			module.erase(0, 1);
			ptr = make_shared<FlipFlopModule>(module, queue);
		}
		else if ( module.starts_with('&') ) {
			module.erase(0, 1);
			ptr = make_shared<ConjunctionModule>(module, queue);
		}
		else {
			cerr << "unknown module type: " << module << endl;
			return;
		}

		for ( const auto& dest: dests ) {
			ptr->add_link(dest);
		}

		modules[module] = ptr;
	}

	// register senders
	for ( auto& module: modules ) {
		auto [name, ptr] = module;

		for ( const auto& link: ptr->links_ ) {
			if ( modules.contains(link) ) {
				modules[link]->register_sender(name);
			}
		}
	}

	long sums[2] = { 0, 0 };
	for ( int i = 0; i != 1000; ++i ) {
		queue.emplace("button", 0, "broadcaster");
		while ( !queue.empty() ) {
			const auto [sender, signal, receiver] = queue.front();
			queue.pop();

			sums[signal]++;

			if ( modules.contains(receiver) ) {
				modules[receiver]->trigger(sender, signal);
			}
		}
	}
	cout << sums[0] * sums[1] << endl;
}

void
part2()
{
	queue<tuple<string, int, string>> queue;
	map<string, shared_ptr<Module>>   modules;

	modules["output"] = make_shared<OutputModule>("output", queue);

	const auto input = read_file("data/day20.txt");
	for ( const auto& line: input ) {
		const auto parts  = split(line, " -> ");
		auto       module = parts[0];
		const auto dests  = split(parts[1], ", ");

		shared_ptr<Module> ptr;
		if ( module == "broadcaster" ) {
			ptr = make_shared<BroadcasterModuler>(module, queue);
		}
		else if ( module.starts_with('%') ) {
			module.erase(0, 1);
			ptr = make_shared<FlipFlopModule>(module, queue);
		}
		else if ( module.starts_with('&') ) {
			module.erase(0, 1);
			ptr = make_shared<ConjunctionModule>(module, queue);
		}
		else {
			cerr << "unknown module type: " << module << endl;
			return;
		}

		for ( const auto& dest: dests ) {
			ptr->add_link(dest);
		}

		modules[module] = ptr;
	}

	// register senders
	string to_rx;
	for ( auto& module: modules ) {
		auto [name, ptr] = module;

		for ( const auto& link: ptr->links_ ) {
			if ( modules.contains(link) ) { // link == "rx"
				modules[link]->register_sender(name);
			}
			else {
				to_rx = name;
			}
		}
	}

	map<string, long> cycle_modules;
	for ( auto& module: modules ) {
		auto [name, ptr] = module;
		const auto links = ptr->links_;

		if ( find(links.begin(), links.end(), to_rx) != links.end() ) {
			cycle_modules[name] = 0;
		}
	}

	for ( long i = 1;; ++i ) {
		queue.emplace("button", 0, "broadcaster");
		while ( !queue.empty() ) {
			const auto [sender, signal, receiver] = queue.front();
			queue.pop();

			if ( signal == 1 && cycle_modules.contains(sender) ) {
				cycle_modules[sender] = i;
			}

			if ( modules.contains(receiver) ) {
				modules[receiver]->trigger(sender, signal);
			}
		}
		if ( all_of(cycle_modules.begin(), cycle_modules.end(), [](const auto& module) { return module.second != 0; }) ) {
			long result = 1;
			for ( const auto& module: cycle_modules ) {
				result = lcm(result, module.second);
			}
			cout << result << endl;
			break;
		}
	}
}

int
main()
{
	part1();
	part2();
}
