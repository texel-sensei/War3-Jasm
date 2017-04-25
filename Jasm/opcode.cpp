#include "opcode.h"
#include <string>

using namespace std;

namespace {
	string op_unknown = "unknown";
}

string op_names[46] =
{
	op_unknown,
	op_unknown,
	"jump",
	"func_start",
	"func_end",
	"local",
	"global",
	"constant",
	"pop",
	op_unknown,
	op_unknown, // 10
	"clean stack",
	"literal",
	"retval",
	"getvar",
	"code",
	"getvar[]",
	"setvar",
	"setvar[]",
	"push",
	"setreg",
	"callnative",
	"callfunc",
	"typecast",
	op_unknown,
	op_unknown,
	"==",
	op_unknown,
	"<=",
	"<",
	">=",
	">",
	"add",
	"sub",
	"mul",
	"div",
	op_unknown,
	"negate",
	"bnot",
	"return",
	"jump dest",
	"jump if true",
	"jump if false",
	op_unknown,
	op_unknown,
	op_unknown
};

string type_names[9] = {
	"unknown","unknown","null","code","integer","real","string","handle","boolean"
};