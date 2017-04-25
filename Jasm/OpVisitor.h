#pragma once
#include "op_desc.h"
#include "opcode.h"
#include "types.h"

class OpVisitor
{
public:
	virtual ~OpVisitor() = 0 {};

	void handle_op(opcode op)
	{
		start_op(op);
		auto sig = signatures[op.optype];
		handle_parameter(op, op.r1, sig.small_pars[2]);
		handle_parameter(op, op.r2, sig.small_pars[1]);
		handle_parameter(op, op.r3, sig.small_pars[0]);
		handle_parameter(op, op.arg, sig.large_par);
		end_op(op);
	}

protected:
	virtual void start_op(opcode op) {}
	virtual void end_op(opcode op) {}

	virtual void handle_unknown(opcode op, int i){};
	virtual void handle_ignored(opcode op, int i) {};
	virtual void handle_register(opcode op, Register r) {};
	virtual void handle_variable(opcode op, VariableId v) {};
	virtual void handle_type(opcode op, Type t) {};
	virtual void handle_function(opcode op, FunctionId fun) {};
	virtual void handle_label(opcode op, LabelId lab) {};
	virtual void handle_string(opcode op, StringId str) {};
	virtual void handle_integer(opcode op, int32_t i) {};
	
	virtual void pre_parameter(opcode op) {};
private:
	void handle_parameter(opcode op, int p, par_type t)
	{
		pre_parameter(op);
		switch(t)
		{
		case par_type::unknown:		handle_unknown(op, p); break;
		case par_type::ignored:		handle_ignored(op, p); break;
		case par_type::reg:			handle_register(op, Register(p)); break;
		case par_type::variable:	handle_variable(op, VariableId(p)); break;
		case par_type::type:		handle_type(op, Type(p)); break;
		case par_type::function:	handle_function(op, FunctionId(p)); break;
		case par_type::jmp_dest:	handle_label(op, LabelId(p)); break;
		case par_type::string:		handle_string(op, StringId(p)); break;
		case par_type::integer:		handle_integer(op, p); break;
		default: ;
		}
	}
};