// Jasm.cpp : Defines the entry point for the console application.
//

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "opcode.h"
#include "op_desc.h"
#include <cassert>
#include "op_format.h"
#include "VM.h"
using namespace std;

vector<opcode> read_bytecode_from_preloader_file(std::string const& filename)
{
	ifstream in(filename);
	string line;
	vector<opcode> ops;
	while (getline(in, line))
	{
		int32_t partA, partB;
		
		auto n = sscanf_s(line.c_str(), "\tcall Preload(%*s %d %d %*s", &partA, &partB);

		if(n == 2) {
			auto op = build_opcode(partA, partB);
			ops.push_back(op);
		}
	}
	return ops;
}

void dump_jass_code(string const& filename, VM const& vm)
{
	ofstream jasscode(filename);
	jasscode << "private function init_arrays takes nothing returns nothing\n";
	for (auto sig : signatures)
	{
		if (!sig.code) continue;

		jasscode << "\tset Jasm_Op_Names  [0x" << hex << static_cast<int>(sig.code) << "] = \"" << sig.code << "\"\n";
		for (int i = 0; i < 3; ++i) {
			jasscode << "\tset Jasm_Op_types_" << i << "[0x"
				<< hex << static_cast<int>(sig.code)
				<< "] = " << static_cast<int>(sig.small_pars[2 - i]) << "\n";
		}
		jasscode << "\tset Jasm_Op_types_L[0x"
			<< hex << static_cast<int>(sig.code)
			<< "] = " << static_cast<int>(sig.large_par) << "\n";
	}
	jasscode << "endfunction\n";
	jasscode << "private function init_natives takes nothing returns nothing\n";
	for(auto& kvp : vm.natives())
	{
		jasscode << "\tcall SaveStr(Jasm_KnownSymbols, 0x"
			<< hex << static_cast<int>(OP_NATIVE)
			<< ", 0x" << hex << kvp.first.value()
			<< ", \"" << kvp.second << "\")\n";
	}
	jasscode << "endfunction\n";
}

int main()
{
	VM vm;
	vm.load_natives("natives.txt");

	auto bytecode = vm.load_bytecode("jasm_example.txt");
	//auto bytecode = read_bytecode_from_preloader_file("jasm_dump.txt");

	if(!bytecode.empty()) {
		ofstream out("output.txt");
		for(auto op : bytecode)
		{
			assert(signatures[op.optype].code == op.optype);
			out << vm.format_opcode(op) << endl;
		}
	} else
	{
		cerr << "File contains no bytecode!" << endl;
	}

	dump_jass_code("arrays.j", vm);

	//system("pause");
    return 0;
}

