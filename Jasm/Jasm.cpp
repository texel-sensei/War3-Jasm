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

int main()
{
	VM vm;
	vm.load_natives("natives.txt");

	auto bytecode = vm.load_bytecode("jasm_example.txt");
	
	ofstream out("output.txt");
	for(auto op : bytecode)
	{
		assert(signatures[op.optype].code == op.optype);
		out << vm.format_opcode(op) << endl;
	}
    return 0;
}

