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

int main()
{
	VM vm;
	vm.load_natives("natives.txt");

	//auto bytecode = vm.load_bytecode("jasm_example.txt");
	auto bytecode = read_bytecode_from_preloader_file("jasm_dump.txt");

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

	

	system("pause");
    return 0;
}

