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

	bool show_native_ids = true;

protected:
	void start_op(opcode& op) override{
		out << std::left << std::setw(12) << get_opcode_name(op.optype, vm);
	}
	void pre_parameter(opcode&) override { out << " "; }

	void handle_unknown(opcode&, uint32_t& i) override {
		out << "0x" << std::hex << std::setw(4) << i;
	}
	
	void handle_ignored(opcode&, uint32_t& i) override {
		if (i != 0)
			out << "!0x" << std::setw(4) << std::hex << i;
		else
			out << std::setw(6) << '-';
	}
	void handle_register(opcode&, Register& r) override {
		out << "R" << std::setw(5) << std::dec << int(r.value());
	}
	void handle_variable(opcode&, VariableId& v) override {
		out << "V_" << v;
	}
	
	void handle_type(opcode&, Type& t) override {
		out << std::setw(6) << t;
	}
	void handle_function(opcode& op, FunctionId& fun) override {
		if(op.optype == OP_NATIVE)
		{
			auto const& native = vm.natives().lookup(fun);
			if(native)
			{
				if(show_native_ids) {
					out << "F_" << fun << " ["<< *native << ']';
				} else
				{
					out << "&" << *native;
					return;
				}
			}
		}

		out << "F_" << fun;
	}
	void handle_label(opcode&, LabelId& lab) override {
		out << "L_" << lab;
	}
	void handle_integer(opcode& o, uint32_t& i) override {
		
		auto t = get_jass_type(o);
		if(t)
		{
			switch(static_cast<TYPES>(t->value()))
			{
			case J_NOTHING: break;
			case J_NULL: break;
			case J_CODE: break;
			case J_INTEGER: out << std::setw(6) << i; return;
			case J_REAL: {
				auto r = *reinterpret_cast<float*>(&i);
				out << std::setw(6) << r;
				return;
			}
			case J_STRING: break;
			case J_HANDLE: break;
			case J_BOOLEAN: break;
			case J_INTARRAY: break;
			case J_REALARRAY: break;
			case J_STRARRAY: break;
			case J_HARRAY: break;
			case J_BOOLARRAY: break;
			default: ;
			}
		}
		out << "0x" << std::setw(4) << std::hex << i;
	}

	VM const& vm;
	std::ostream& out;
};