#pragma once
#include "types.h"
#include "optional.h"
#include <map>
#include <unordered_map>
#include <fstream>
#include <sstream>

template<typename Key>
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
			names[Key(id)] = name;
		}
	}

	optional<std::string> lookup(Key key) const
	{
		auto it = names.find(key);
		if (it == names.end())
			return{};
		return it->second;
	}

private:
	std::unordered_map<Key, std::string, NumHasher<Key>> names;
};

using NativeTable = SymbolTable<FunctionId>;