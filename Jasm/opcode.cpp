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
	"type",
	op_unknown, // 10
	"cleanstack",
	"literal",
	"mov",
	"getvar",
	"code",
	"getvar[]",
	"setvar",
	"setvar[]",
	"push",
	"pop",
	"callnative",
	"calljass",
	"i2r",
	"and",
	"or",
	"eq",
	"ne",
	"le",
	"ge",
	"lt",
	"gt",
	"add",
	"sub",
	"mul",
	"div",
	"mod",
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