#pragma once
#include "opcode.h"
#include <sstream>
#include <iomanip>

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

op_sig signatures[] = {
	{}, // 0
	{OP_ENDPROGRAM	,{par_type::ignored	, par_type::ignored	, par_type::ignored	}, par_type::ignored	},
	{}, // 2
	{OP_FUNCTION	,{par_type::ignored	, par_type::ignored	, par_type::type	}, par_type::function	},
	{OP_ENDFUNCTION	,{par_type::ignored	, par_type::ignored	, par_type::ignored	}, par_type::ignored	},
	{OP_LOCAL		,{par_type::ignored	, par_type::ignored	, par_type::type	}, par_type::variable	},
	{OP_GLOBAL		,{par_type::ignored	, par_type::ignored	, par_type::type	}, par_type::variable	},
	{OP_CONSTANT	,{par_type::unknown	, par_type::unknown	, par_type::unknown	}, par_type::unknown	},
	{OP_POPFUNCARG	,{par_type::ignored	, par_type::integer	, par_type::type	}, par_type::variable	},
	{}, // 9
	{}, // A
	{OP_CLEANSTACK	,{par_type::ignored	, par_type::ignored	, par_type::integer	}, par_type::ignored	},
	{OP_LITERAL		,{par_type::ignored	, par_type::type	, par_type::reg		}, par_type::integer	},
	{OP_SETRET		,{par_type::ignored	, par_type::reg		, par_type::ignored	}, par_type::ignored	},
	{OP_GETVAR		,{par_type::ignored	, par_type::type	, par_type::reg		}, par_type::variable	},
	{OP_CODE		,{par_type::unknown	, par_type::unknown	, par_type::unknown	}, par_type::unknown	},
	{OP_GETARRAY	,{par_type::unknown	, par_type::unknown	, par_type::unknown	}, par_type::unknown	},
	{OP_SETVAR		,{par_type::unknown	, par_type::unknown	, par_type::reg		}, par_type::variable	},
	{OP_SETARRAY	,{par_type::unknown	, par_type::unknown	, par_type::unknown	}, par_type::unknown	},
	{OP_PUSH		,{par_type::unknown	, par_type::unknown	, par_type::reg		}, par_type::unknown	},
	{OP_SETRIGHT	,{par_type::unknown	, par_type::unknown	, par_type::unknown	}, par_type::unknown	},
	{OP_NATIVE		,{par_type::ignored	, par_type::ignored	, par_type::ignored }, par_type::function	},
	{OP_JASSCALL	,{par_type::ignored	, par_type::ignored	, par_type::ignored }, par_type::function	},
	{OP_I2R			,{par_type::unknown	, par_type::unknown	, par_type::unknown	}, par_type::unknown	},
	{OP_AND			,{par_type::unknown	, par_type::unknown	, par_type::unknown	}, par_type::unknown	},
	{OP_OR			,{par_type::unknown	, par_type::unknown	, par_type::unknown	}, par_type::unknown	},
	{OP_EQUAL		,{par_type::unknown	, par_type::unknown	, par_type::unknown	}, par_type::unknown	},
	{OP_NOTEQUAL	,{par_type::unknown	, par_type::unknown	, par_type::unknown	}, par_type::unknown	},
	{OP_LESSEREQUAL ,{par_type::unknown	, par_type::unknown	, par_type::unknown	}, par_type::unknown	},
	{OP_GREATEREQUAL,{par_type::unknown	, par_type::unknown	, par_type::unknown	}, par_type::unknown	},
	{OP_LESSER		,{par_type::unknown	, par_type::unknown	, par_type::unknown	}, par_type::unknown	},
	{OP_GREATER		,{par_type::unknown	, par_type::unknown	, par_type::unknown	}, par_type::unknown	},
	{OP_ADD			,{par_type::unknown	, par_type::unknown	, par_type::unknown	}, par_type::unknown	},
	{OP_SUB			,{par_type::unknown	, par_type::unknown	, par_type::unknown	}, par_type::unknown	},
	{OP_MUL			,{par_type::unknown	, par_type::unknown	, par_type::unknown	}, par_type::unknown	},
	{OP_DIV			,{par_type::unknown	, par_type::unknown	, par_type::unknown	}, par_type::unknown	},
	{OP_MODULO		,{par_type::unknown	, par_type::unknown	, par_type::unknown	}, par_type::unknown	},
	{OP_NEGATE		,{par_type::unknown	, par_type::unknown	, par_type::unknown	}, par_type::unknown	},
	{OP_NOT			,{par_type::unknown	, par_type::unknown	, par_type::unknown	}, par_type::unknown	},
	{OP_RETURN		,{par_type::ignored	, par_type::ignored	, par_type::ignored	}, par_type::ignored	},
	{OP_LABEL		,{par_type::ignored	, par_type::ignored	, par_type::ignored }, par_type::jmp_dest	},
	{OP_JUMPIFTRUE	,{par_type::unknown	, par_type::unknown	, par_type::unknown	}, par_type::unknown	},
	{OP_JUMPIFFALSE ,{par_type::unknown	, par_type::unknown	, par_type::unknown	}, par_type::unknown	},
	{OP_JUMP		,{par_type::unknown	, par_type::unknown	, par_type::unknown	}, par_type::unknown	}
};	 