#pragma once
#include "op_desc.h"
#include "opcode.h"
#include "OpVisitor.h"
#include "SymbolTable.h"
#include "VM.h"

class op_format : public OpVisitor
{
public:
	explicit op_format(VM const& vm, std::ostream& os) : vm(vm), out(os){}
	~op_format() override{}

protected:
	void pre_parameter(opcode&) override { out << " "; }

	void handle_unknown(opcode&, int& i) override {
		out << "0x" << std::hex << std::setw(4) << i;
	}
	
	void handle_ignored(opcode&, int& i) override {
		if (i != 0)
			out << "!0x" << std::setw(4) << std::hex << i;
		else
			out << std::setw(6) << '-';
	}
	void handle_register(opcode&, Register& r) override {
		out << "R" << std::setw(5) << int(r.value());
	}
	void handle_variable(opcode&, VariableId& v) override {
		out << "V_" << v;
	}
	
	void handle_type(opcode&, Type& t) override {
		out << std::setw(6) << t;
	}
	void handle_function(opcode& op, FunctionId& fun) override {
		out << "F_" << fun;
		if(op.optype == OP_NATIVE)
		{
			auto const& native = vm.natives().lookup(fun);
			if(native)
			{
				out << " ["<< *native << ']';
			}
		}
	}
	void handle_label(opcode&, LabelId& lab) override {
		out << "L_" << lab;
	}
	void handle_integer(opcode&, int32_t& i) override {
		out << std::setw(6) << i;
	}
private:
	VM const& vm;
	std::ostream& out;
};