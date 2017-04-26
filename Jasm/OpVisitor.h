#pragma once
#include "op_desc.h"
#include "opcode.h"
#include "types.h"

class OpVisitor
{
public:
	virtual ~OpVisitor() = 0 {};

	void handle_op(opcode& op)
	{
		start_op(op);
		auto sig = signatures[op.optype];
		uint32_t r1 = op.r1, r2 = op.r2, r3 = op.r3;
		
		id_ = 0; handle_parameter(op, r1, sig.small_pars[2]); op.r1 = r1;
		id_ = 1; handle_parameter(op, r2, sig.small_pars[1]); op.r2 = r2;
		id_ = 2; handle_parameter(op, r3, sig.small_pars[0]); op.r3 = r3;
		id_ = 3; handle_parameter(op, op.arg, sig.large_par);

		end_op(op);
	}

protected:
	int current_parameter_id() const { return id_; }

	virtual void start_op(opcode& op) {}
	virtual void end_op(opcode& op) {}

	virtual void handle_unknown(opcode& op, uint32_t& i){};
	virtual void handle_ignored(opcode& op, uint32_t& i) {};
	virtual void handle_register(opcode& op, Register& r) {};
	virtual void handle_variable(opcode& op, VariableId& v) {};
	virtual void handle_type(opcode& op, Type& t) {};
	virtual void handle_function(opcode& op, FunctionId& fun) {};
	virtual void handle_label(opcode& op, LabelId& lab) {};
	virtual void handle_integer(opcode& op, uint32_t& i) {};
	
	virtual void pre_parameter(opcode& op) {};
private:
	int id_;

	void handle_parameter(opcode& op, uint32_t& p, par_type t)
	{
		pre_parameter(op);
		switch(t)
		{
		case par_type::unknown: {handle_unknown(op, p); break; }
		case par_type::ignored:		{handle_ignored(op, p); break; }
		case par_type::reg:			{Register r( p ); handle_register(op, r); p = r.value(); break; }
		case par_type::variable:	{VariableId v( p ); handle_variable(op, v); p = v.value(); break; }
		case par_type::type:		{Type tt( p );  handle_type(op, tt); p = tt.value(); break; }
		case par_type::function:	{FunctionId f( p ); handle_function(op, f); p = f.value(); break; }
		case par_type::jmp_dest:	{LabelId l( p ); handle_label(op, l); p = l.value(); break; }
		case par_type::integer:		{handle_integer(op, p); break; }
		default: ;
		}
	}
};