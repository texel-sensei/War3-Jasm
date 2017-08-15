#include "opcode.h"
#include <string>

const char* op_unknown = "";

const char* op_names[46] =
{
	op_unknown,
	"endprogram",
	op_unknown,
	"func",
	"endf",
	"local",
	"global",
	"const",
	"poparg",
	"extend",
	"type", // 10
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