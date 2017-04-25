#pragma once
#include <cstdint>
#include <string>
#include <iostream>

/*
 * 0x1 is used internally for saving the cur op
 */
enum TYPES {
	J_NOTHING=0x0,J_NULL=0x2,J_CODE,J_INTEGER,J_REAL,J_STRING,J_HANDLE,J_BOOLEAN,
	J_INTARRAY=0x9,J_REALARRAY,J_STRARRAY,J_HARRAY,J_BOOLARRAY
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

enum OPCODES {
	OP_ENDPROGRAM=0x1,
	OP_FUNCTION=0x3, // _ _ rettype funcname
	OP_ENDFUNCTION=0x4,
	OP_LOCAL=0x5, // _ _ type name
	OP_GLOBAL=0x6,
	OP_CONSTANT=0x7,
	OP_POPFUNCARG=0x8, // _ srcargi type destvar
	OP_CLEANSTACK=0xB, // _ _ nargs _
	OP_LITERAL=0xC, // _ type destreg srcvalue
	OP_SETRET=0xD, // _ srcreg _ _
	OP_GETVAR=0xE, // _ type destreg srcvar
	OP_CODE=0xF,
	OP_GETARRAY=0x10,
	OP_SETVAR=0x11, // _ _ srcreg destvar
	OP_SETARRAY=0x12,
	OP_PUSH=0x13, // _ _ srcreg _
	OP_SETRIGHT=0x14,
	OP_NATIVE=0x15, // _ _ _ fn
	OP_JASSCALL=0x16, // _ _ _ fn
	OP_I2R=0x17,
	OP_AND = 0x18,
	OP_OR = 0x19,
	OP_EQUAL=0x1A,
	OP_NOTEQUAL=0x1B, // check
	OP_LESSEREQUAL=0x1C,
	OP_GREATEREQUAL=0x1D,
	OP_LESSER=0x1E,
	OP_GREATER=0x1F,
	OP_ADD=0x20,
	OP_SUB,
	OP_MUL,
	OP_DIV,
	OP_MODULO = 0x24,
	OP_NEGATE=0x25,
	OP_NOT = 0x26,
	OP_RETURN=0x27, // _ _ _ _
	OP_LABEL=0x28,
	OP_JUMPIFTRUE=0x29,
	OP_JUMPIFFALSE=0x2A,
	OP_JUMP=0x2B,

	/*
// statically typed mods
	SOP_ADD_INT=0x80,SOP_ADD_REAL,SOP_ADD_STRING,
	SOP_SUB_INT=0x84,SOP_SUB_REAL,
	SOP_MUL_INT=0x88,SOP_MUL_REAL,
	SOP_DIV_INT=0x8C,SOP_DIV_REAL,
	SOP_NEGATE_INT=0x90,SOP_NEGATE_REAL,

	SOP_GE_INT=0x94,SOP_GE_REAL,
	SOP_LE_INT=0x98,SOP_LE_REAL,
	SOP_G_INT=0x9C,SOP_G_REAL,
	SOP_L_INT=0xA0,SOP_L_REAL
	*/
};

extern std::string type_names[9];
extern std::string op_names[46];

struct opcode {
	union {
		unsigned char r1;
		unsigned char nfarg;
	};
	union {
		unsigned char r2;
		unsigned char usereturn;
	};
	union {
		unsigned char r3;
		unsigned char rettype;
	};
	unsigned char optype;
	//union {
		int32_t arg;
		//opcode *dest;
	//};
};
static_assert(sizeof(opcode) == 8, "Invalid size for struct opcode");

inline std::ostream& operator<<(std::ostream& os, OPCODES code) {
	return os << op_names[code];
}