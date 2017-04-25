#pragma once
#include "opcode.h"
#include <sstream>
#include <iomanip>
#include <vector>

enum class par_type
{
	unknown,
	ignored,
	reg,
	variable,
	type,
	function,
	jmp_dest,
	string,
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
	case par_type::string: os << "str"; break;
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

	std::string str() const
	{
		using namespace std;
		stringstream ss;
		ss	<< left << setw(12)
			<< code
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