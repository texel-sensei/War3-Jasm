// Jasm.cpp : Defines the entry point for the console application.
//

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "opcode.h"
#include "op_desc.h"
#include <cassert>
using namespace std;

int main()
{
	ifstream in("jasm_example.txt");
	ofstream out("output.txt");
	string line_str;
	while(getline(in, line_str))
	{
		stringstream line(line_str);
		int32_t partA, partB;
		line >> partA >> partB;

		auto op = build_opcode( partA, partB );
		assert(signatures[op.optype].code == op.optype);
		out << signatures[op.optype].str() << endl;
	}
    return 0;
}

