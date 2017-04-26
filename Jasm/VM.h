#pragma once
#include "SymbolTable.h"
#include "opcode.h"

using FunctionTable = SymbolTable<FunctionId, NumHasher<FunctionId>>;
using OpNameTable = SymbolTable<OPCODES>;

class VM
{
public:
	VM() = default;

	void load_natives(std::string const& filename)
	{
		natives_.load_file(filename);
	}

	void load_op_names(std::string const& filename)
	{
		op_names_.load_file(filename);
	}

	std::vector<opcode> load_bytecode(std::string const& filename) const
	{
		using namespace std;
		ifstream in(filename);
		return load_bytecode(in);
	}

	std::vector<opcode> load_bytecode(std::istream& in) const
	{
		using namespace std;
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

	auto const& natives() const { return natives_; }
	auto const& op_names() const { return op_names_; }

private:
		FunctionTable natives_;
		OpNameTable op_names_;

};

inline std::string get_opcode_name(OPCODES code, VM const& vm)
{
	auto n = vm.op_names().lookup(code);
	if (n) return *n;
	std::stringstream ss;
	ss << "0x" << std::hex << static_cast<int>(code);
	return ss.str();
}