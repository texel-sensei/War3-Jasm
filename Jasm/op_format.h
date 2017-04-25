#pragma once
#include "op_desc.h"
#include "opcode.h"
#include "OpVisitor.h"

class op_format : public OpVisitor
{
public:
	explicit op_format(std::ostream& os) : out(os){}
	~op_format() override{}

protected:
	void pre_parameter() override { out << " "; }

	void handle_unknown(int i) override {
		out << std::hex << std::setw(6) << i;
	}
	
	void handle_ignored(int i) override {
		out << std::setw(6) << '-';
	}
	void handle_register(Register r) override {
		out << "R" << std::setw(5) << int(r.value());
	}
	void handle_variable(VariableId v) override {
		out << "V_" << v;
	}
	
	void handle_type(Type t) override {
		out << std::setw(6) << t;
	}
	void handle_function(FunctionId fun) override {
		out << "F_" << fun;
	}
	void handle_label(LabelId lab) override {
		out << "L_" << lab;
	}
	void handle_string(StringId str) override {
		out << "S_" << str;
	}
	void handle_integer(int32_t i) override {
		out << std::setw(6) << i;
	}
private:
	std::ostream& out;
};

inline std::string format_opcode(opcode op)
{
	using namespace std;
	auto sig = signatures[op.optype];

	stringstream ss;
	ss	<< left << setw(12)
		<< op.name();

	op_format of(ss);
	of.handle_op(op);
	

	return ss.str();
}
