#include "VM.h"
#include "op_format.h"

std::string VM::format_opcode(opcode op) const
{
	using namespace std;
	auto sig = signatures[op.optype];

	stringstream ss;
	ss << left << setw(12)
		<< get_opcode_name(op.optype, *this);

	op_format of(*this, ss);
	of.handle_op(op);

	return ss.str();
}
