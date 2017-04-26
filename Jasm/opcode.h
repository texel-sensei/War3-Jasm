#pragma once
#include <cstdint>
#include <string>
#include <iostream>
#include <sstream>
#include "types.h"

/*
 * 0x1 is used internally for saving the cur op
 */
enum TYPES
{
	J_NOTHING=0x0,
	J_NULL=0x2,
	J_CODE,
	J_INTEGER,
	J_REAL,
	J_STRING,
	J_HANDLE,
	J_BOOLEAN,
	J_INTARRAY=0x9,
	J_REALARRAY,
	J_STRARRAY,
	J_HARRAY,
	J_BOOLARRAY
};

/*
 * Declare types?
 * 0x9
 * 0xA
 */
/*
 * Unknown
 * 0x17
 */

enum OPCODES : unsigned char
{
	OP_ENDPROGRAM = 0x1, //Signal to the parser that code has ended. Ignored by the VM
	OP_JUMP_DEPRECATED = 0x2, //Might have been used in past, now has the same behaviour as OP_JUMP

	OP_FUNCTION = 0x3, //Function declaration. Arg1 = return type, Data = function name. Used only
	//at parsing stage.

	OP_ENDFUNCTION = 0x4, //Denotes end of function. Used only at parsing stage, ignored by VM

	OP_LOCAL = 0x5, //Create local variable. Arg1 = variable type, Data = Variable name

	OP_GLOBAL = 0x6,
	OP_CONSTANT = 0x7, //These two are actually the same! There's no difference between
	//globals and constants from VM's point of view. Parameters are
	//the same as above.

	OP_POPFUNCARG = 0x8, //Create a local var and assign a value to it directly from the caller's
	//stack. Arg1 = Type, Arg2 = #FuncArg, Data = Variable Name

	OP_TYPE = 0x9,
	OP_EXTENDS = 0xA, //Used only by parser, ignored by VM

	OP_CLEANSTACK = 0xB, //Pops <Arg1> values from the stack. Used after calling Jass functions,
	//not needed when calling natives.

	OP_LITERAL = 0xC, //Set value of register to a literal. Arg1 = DestReg, Arg2 = DestType,
	//Data = Literal. If type is J_STRING, a string handle is immediately
	//created, and that's what the register actually gets.

	OP_MOV = 0xD, //Moves 1 register to another. Curiously it's only used when returning
	//a value (by moving to R0). Arg1 = DestReg, Arg2 = SrcReg

	OP_GETVAR = 0xE, //Read variable. Arg1 = DestReg, Arg2 = DestType, Data = Variable name.
	//DestType doesn't need to match the variable type.

	OP_CODE = 0xF, //Get function address. Arg1 = DestReg, Data = Function name. Value is
	//stored with type J_CODE.

	OP_GETARRAY = 0x10, //Read from array. Arg1 = DestReg, Arg2 = IndexReg, Arg3 = DestType,
	//Data = Array name. DestType doesn't need to match array type.

	OP_SETVAR = 0x11, //Write variable. Arg1 = SrcReg, Data = Variable name. Type of SrcReg MUST
	//MATCH the type of variable, unless it's J_NULL.

	OP_SETARRAY = 0x12, //Write to array. Arg1 = IndexReg, Arg2 = SrcReg, Data = Array name. Type
	//of SrcReg must match the array type.

	OP_PUSH = 0x13, //Pushes register <Arg1> onto the stack

	OP_POP = 0x14, //Pops the value from the top of the stack to register <Arg1>. God knows
	//why Blizzard uses PUSH/POP on math operations.

	OP_NATIVE = 0x15, //Call a native. Data = Native name. No arguments,

	OP_JASSCALL = 0x16, //Call a JASS function. Same as above.

	OP_I2R = 0x17, //Read register <Arg1>, convert it to a float, and store the result in the
	//same register, with type J_REAL. This is faster than native I2R

	//Boolean operations, Arg1 = DestReg, Arg2 and 3 are source operands.

	//These are not bitwise operations, but just boolean, they return either 0 or 1.
	OP_AND = 0x18,
	OP_OR = 0x19,

	//Comparison operations, return either 0 or 1
	OP_EQUAL = 0x1A,
	OP_NOTEQUAL = 0x1B, // check
	OP_LESSEREQUAL = 0x1C,
	OP_GREATEREQUAL = 0x1D,
	OP_LESSER = 0x1E,
	OP_GREATER = 0x1F,

	//End boolean operations

	OP_ADD = 0x20,
	OP_SUB,
	OP_MUL,
	OP_DIV, //Math operations, Arg1 = DestReg, Arg2 and 3 are source operands.

	OP_MODULO = 0x24, //Same as above, but this one is deprecated and never emitted by compiler (WHY?)

	OP_NEGATE = 0x25, //Negate the value of register <Arg1> and store in the same register. Notice that
	//writing negative literals in the script will always produce this operation, when
	//it could just emit a literal already negated.

	OP_NOT = 0x26, //Boolean NOT. Returns 1 if register <Arg1> is 0, and 0 in all other cases. Result
	//is stored in the same register.

	OP_RETURN = 0x27, //No arguments

	OP_LABEL = 0x28, //Defines a jump label. Data = Label Id. Used at parsing stage, ignored by VM

	OP_JUMPIFTRUE = 0x29,
	OP_JUMPIFFALSE = 0x2A, //Jump to label <Data> depending on contents of register <Arg1>

	OP_JUMP = 0x2B //Unconditional jump. Data = Label Id
};

inline std::ostream& operator<<(std::ostream& os, OPCODES t) = delete;

extern std::string type_names[9];
extern const char* op_names[46];

inline std::ostream& operator<<(std::ostream& os, Type t)
{
	auto v = t.value();
	auto array = false;
	if (v < 0x09)
	{
		return os << type_names[v];
	}

	switch (static_cast<TYPES>(v))
	{
	case J_INTARRAY: os << "int[]";
		break;
	case J_REALARRAY: os << "real[]";
		break;
	case J_STRARRAY: os << "string[]";
		break;
	case J_HARRAY: os << "hdl[]";
		break;
	case J_BOOLARRAY: os << "bool[]";
		break;
	default: ;
	}
	return os;
}

struct opcode
{
	unsigned char r3;
	unsigned char r2;
	unsigned char r1;
	OPCODES optype;
	int32_t arg;
};

static_assert(sizeof(opcode) == 8, "Invalid size for struct opcode");

inline opcode build_opcode(int32_t A, int32_t B)
{
	auto op = opcode();
	*reinterpret_cast<int32_t*>(&op) = A;
	op.arg = B;
	return op;
}
