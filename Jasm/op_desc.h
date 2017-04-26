#pragma once
#include "opcode.h"
#include <sstream>
#include <iomanip>
#include <vector>
#include "VM.h"

class VM;

enum class par_type
{
	unknown,
	ignored,
	reg,
	variable,
	type,
	function,
	jmp_dest,
	integer
};

inline std::ostream& operator<<(std::ostream& os, par_type pt) {
	switch (pt)
	{
	case par_type::unknown: os << "???"; break;
	case par_type::ignored: os << " - "; break;
	case par_type::reg: os << "reg"; break;
	case par_type::variable: os << "var"; break;
	case par_type::type: os << "typ"; break;
	case par_type::function: os << "fun"; break;
	case par_type::jmp_dest: os << "lab"; break;
	case par_type::integer: os << "int"; break;
	default: ;
	}
	return os;
}

struct op_sig
{
	OPCODES code;
	par_type small_pars[3];
	par_type large_par;

	std::string str(VM const& vm) const
	{
		using namespace std;
		stringstream ss;
		ss	<< left << setw(12)
			<< get_opcode_name(code, vm)
			<< '\t';
		for (const auto small_par : small_pars)
		{
			ss << small_par << '\t';
		}
		ss << large_par;
		return ss.str();
	}
};

extern std::vector<op_sig> signatures;

inline optional<Type> get_jass_type(opcode op)
{
	auto sig = signatures[op.optype];
	if (sig.small_pars[0] == par_type::type) return Type(op.r3);
	if (sig.small_pars[1] == par_type::type) return Type(op.r2);
	if (sig.small_pars[2] == par_type::type) return Type(op.r1);
	return{};
}