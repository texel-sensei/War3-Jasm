#include "VM.h"
#include "op_format.h"

std::string VM::format_opcode(opcode op) const
{
	using namespace std;
	auto sig = signatures[op.optype];

	stringstream ss;
	ss << left << setw(12)
		<< op.name();

	op_format of(*this, ss);
	of.handle_op(op);


	return ss.str();
}
