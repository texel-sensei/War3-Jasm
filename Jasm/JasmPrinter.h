#pragma once

#include "op_format.h"

struct jasm_printer : op_format
{
	using op_format::op_format;

	virtual void prolog() {}
	virtual void line_start(int line, opcode op) {}
	virtual void line_end(int line, opcode op)
	{
		out << "\n";
	}
	virtual void epilog() {}
	

	void print_jasm(
		std::vector<opcode> const& ops
	){
		using namespace std;
		int line_nr = 0;
		prolog();
		for(auto op : ops) {
			auto sig = signatures[op.optype];

			line_start(line_nr, op);
			handle_op(op);
			line_end(line_nr, op);
			line_nr++;
		}
		epilog();
	}
};

struct jasm_jass_printer : jasm_printer
{
	jasm_jass_printer(VM const& vm, std::ostream& out, std::string const& array_name)
		: jasm_printer(vm, out), array(array_name)
	{
		show_native_ids = false;
	}

	void prolog() override {
		out << "function InitJasmArray_" << array << " takes nothing returns nothing\n";
	}

	void line_start(int line, opcode op) override {
		out << "    set " << array << "[" << std::right << std::setw(4) << std::dec << 2*line << "] = 0x"
			<< std::setw(8) << std::setfill('0') << std::hex << op.get_partA() << " // ";
		out << std::setfill(' ');
	}

	void line_end(int line, opcode op) override {
		out << "\n";
		out << "    set " << array << "[" << std::right << std::setw(4) << std::dec << 2 * line + 1 << "] = 0x"
			<< std::setw(8) << std::setfill('0') << std::hex << op.arg;
		out << "\n";
		out << std::setfill(' ');
	}

	void epilog() override {
		out << "endfunction\n";
	}

private:
	std::string array;
};