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
		handle_parameter(op.r1, sig.small_pars[2]);
		handle_parameter(op.r2, sig.small_pars[1]);
		handle_parameter(op.r3, sig.small_pars[0]);
		handle_parameter(op.arg, sig.large_par);
		end_op(op);
	}

protected:
	virtual void start_op(opcode op) {}
	virtual void end_op(opcode op) {}

	virtual void handle_unknown(int i){};
	virtual void handle_ignored(int i) {};
	virtual void handle_register(Register r) {};
	virtual void handle_variable(VariableId v) {};
	virtual void handle_type(Type t) {};
	virtual void handle_function(FunctionId fun) {};
	virtual void handle_label(LabelId lab) {};
	virtual void handle_string(StringId str) {};
	virtual void handle_integer(int32_t i) {};
	
	virtual void pre_parameter() {};
private:
	void handle_parameter(int p, par_type t)
	{
		pre_parameter();
		switch(t)
		{
		case par_type::unknown:		handle_unknown(p); break;
		case par_type::ignored:		handle_ignored(p); break;
		case par_type::reg:			handle_register(Register(p)); break;
		case par_type::variable:	handle_variable(VariableId(p)); break;
		case par_type::type:		handle_type(Type(p)); break;
		case par_type::function:	handle_function(FunctionId(p)); break;
		case par_type::jmp_dest:	handle_label(LabelId(p)); break;
		case par_type::string:		handle_string(StringId(p)); break;
		case par_type::integer:		handle_integer(p); break;
		default: ;
		}
	}
};