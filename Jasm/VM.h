#pragma once
#include "SymbolTable.h"
#include "opcode.h"

class VM
{
public:
	VM() = default;

	void load_natives(std::string const& filename)
	{
		natives_.load_file(filename);
	}

	std::vector<opcode> load_bytecode(std::string const& filename) const
	{
		using namespace std;
		ifstream in(filename);
		string line_str;
		vector<opcode> ops;
		while (getline(in, line_str))
		{
			stringstream line(line_str);
			int32_t partA, partB;
			line >> partA >> partB;

			auto op = build_opcode(partA, partB);
			ops.push_back(op);
		}
		return ops;
	}

	std::string format_opcode(opcode op) const;

	NativeTable const& natives() const { return natives_; }

private:
		NativeTable natives_;

};

