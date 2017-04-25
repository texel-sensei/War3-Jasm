#include "opcode.h"
#include <string>

const char* op_unknown = nullptr;

const char* op_names[46] =
{
	nullptr,
	"endprogram",
	op_unknown,
	"func",
	"endf",
	"local",
	"global",
	"const",
	"poparg",
	"cleanstack",
	op_unknown, // 10
	"cleanstack",
	"literal",
	"retval",
	"getvar",
	"code",
	"getvar[]",
	"setvar",
	"setvar[]",
	"push",
	"setright",
	"callnative",
	"calljass",
	"i2r",
	op_unknown,
	op_unknown,
	"eq",
	"ne",
	"le",
	"lt",
	"ge",
	"gt",
	"add",
	"sub",
	"mul",
	"div",
	op_unknown,
	"neg",
	"not",
	"ret",
	"label",
	"jmpt",
	"jmpf",
	"jmp",
	op_unknown,
	op_unknown
};

std::string type_names[9] = {
	"void","unknown","null","code","int","real","string","handle","bool"
};