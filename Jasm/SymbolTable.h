#pragma once
#include "optional.h"
#include <map>
#include <unordered_map>
#include <fstream>
#include <sstream>

template<typename Key, typename Hash = std::hash<Key>>
class SymbolTable
{
public:
	SymbolTable() = default;

	void load_file(std::string const& filename)
	{
		using namespace std;
		ifstream in(filename);
		string str;
		while(getline(in,str))
		{
			stringstream line(str);
			int id;
			string name;
			line >> hex >> id >> name;
			register_symbol(Key(id), name);
		}
	}

	void register_symbol(Key id, std::string const& name)
	{
		names[id] = name;
		ids[name] = id;
	}

	optional<std::string> lookup(Key key) const
	{
		auto it = names.find(key);
		if (it == names.end())
			return{};
		return it->second;
	}

	optional<Key> lookup(std::string const& name) const
	{
		auto it = ids.find(name);
		if (it == ids.end())
			return{};
		return it->second;
	}

	auto begin() const {
		return names.begin();
	}

	auto end() const {
		return names.end();
	}

private:
	std::unordered_map<Key, std::string, Hash> names;
	std::unordered_map<std::string, Key> ids;
};