#include <memory.h>
#include "as/generate.h"
#include "as/common.h"
#include "instructions.h"

byte data[ROM_SIZE];

#define Rel(addr) (data + addr - ROM_START)

static void gen_raw_data(word a, raw_data_node* d) {
	if (d->is_array) {
		// string
		memcpy(Rel(a), d->as.array, d->length);
	}
    else if (d->length == 1) {
		// byte
		*Rel(a) = d->as.byte;
	}
    else if (d->length == 2) { 
		Assert(!d->is_ident, "unresolved label");
		*Rel(a) = d->as.word & 0xff;
		*Rel(a + 1) = d->as.word >> 8;
	}
}

static void gen_instr_with_operands(word a, instr_node* i,
	opcode imm, opcode imp_or_none, opcode opx, opcode opy, opcode abs) {
	// assume the parser only generates correct nodes

	switch (i->arg_type) {
		case 0:
			*Rel(a) = imp_or_none;
			break;

		case TOK_IMM_LITERAL:
			*Rel(a) = imm;
			*Rel(a + 1) = i->as.imm_literal;
			break;

		case TOK_REGISTER_X:
			*Rel(a) = opx;
			break;

		case TOK_REGISTER_Y:
			*Rel(a) = opy;
			break;

		case TOK_ABS_LITERAL:
			*Rel(a) = abs;
			*Rel(a + 1) = i->as.literal & 0xff;
			*Rel(a + 2) = i->as.literal >> 8;
			break;

		default:
			printf("$%04x %d\n", a, i->arg_type);
			Log_err("invalid parser output");
			Abort(STATUS_INTERNAL_ERROR);
	}
}

static void gen_instruction(word a, instr_node* i) {
	if (i->arg_type)
		Assert(i->arg_type != TOK_IMM_IDENTIFIER
			&& i->arg_type != TOK_ABS_IDENTIFIER, "unresolved label");

#define Op(...)	gen_instr_with_operands(a, i, __VA_ARGS__); break

	switch (i->instr) {
		// load/store operations
		case INS_LDA: Op(OP_LDA_IMM, 0, OP_LDA_OPX, OP_LDA_OPY, OP_LDA_ABS);
		case INS_LDX: Op(OP_LDX_IMM, 0, 0, OP_LDX_OPY, OP_LDX_ABS);
		case INS_LDY: Op(OP_LDY_IMM, 0, OP_LDY_OPX, 0, OP_LDY_ABS);
		case INS_STA: Op(0, 0, OP_LDA_OPX, OP_LDA_OPY, OP_LDA_ABS);
		case INS_STX: Op(0, 0, 0, 0, OP_STX_ABS);
		case INS_STY: Op(0, 0, 0, 0, OP_STY_ABS);

		// register operations
		case INS_TAX: Op(0, OP_TAX, 0, 0, 0);
		case INS_TAY: Op(0, OP_TAY, 0, 0, 0);
		case INS_TXA: Op(0, OP_TXA, 0, 0, 0);
		case INS_TYA: Op(0, OP_TYA, 0, 0, 0);
		case INS_SAX: Op(0, OP_SAX, 0, 0, 0);
		case INS_SAY: Op(0, OP_SAY, 0, 0, 0);
		case INS_SXY: Op(0, OP_SXY, 0, 0, 0);

		// stack operations
		case INS_TSX: Op(0, OP_TSX, 0, 0, 0);
		case INS_TXS: Op(0, OP_TXS, 0, 0, 0);
		case INS_PSH: Op(OP_PSH_IMM, OP_PSH_IMP, OP_PSH_OPX, OP_PSH_OPY, 0);
		case INS_PLL: Op(0, OP_PLL, 0, 0, 0);
		case INS_POP: Op(0, OP_POP, 0, 0, 0);

		// bitwise operations
		case INS_AND: Op(OP_AND_IMM, 0, OP_AND_OPX, 0, OP_AND_ABS);
		case INS_IOR: Op(OP_IOR_IMM, 0, OP_IOR_OPX, 0, OP_IOR_ABS);
		case INS_XOR: Op(OP_XOR_IMM, 0, OP_XOR_OPX, 0, OP_XOR_ABS);
		case INS_SHL: Op(OP_SHL_IMM, 0, OP_SHL_OPX, 0, OP_SHL_ABS);
		case INS_SHR: Op(OP_SHR_IMM, 0, OP_SHR_OPX, 0, OP_SHR_ABS);

		// numerical operations
		case INS_ADD: Op(OP_ADD_IMM, 0, OP_ADD_OPX, 0, OP_ADD_ABS);
		case INS_SUB: Op(OP_SUB_IMM, 0, OP_SUB_OPX, 0, OP_SUB_ABS);
		case INS_MUL: Op(OP_MUL_IMM, 0, OP_MUL_OPX, 0, OP_MUL_ABS);
		case INS_DIV: Op(OP_DIV_IMM, 0, OP_DIV_OPX, 0, OP_DIV_ABS);
		case INS_INC: Op(OP_LDA_IMM, 0, OP_LDA_OPX, OP_LDA_OPY, OP_LDA_ABS);
		case INS_DEC: Op(OP_LDA_IMM, 0, OP_LDA_OPX, OP_LDA_OPY, OP_LDA_ABS);

		// control flow operations
		case INS_JMP: Op(0, OP_JMP, 0, 0, 0);
		case INS_JIZ: Op(0, OP_JIZ, 0, 0, 0);
		case INS_JNZ: Op(0, OP_JNZ, 0, 0, 0);
		case INS_JIC: Op(0, OP_JIC, 0, 0, 0);
		case INS_JNC: Op(0, OP_JNC, 0, 0, 0);
		case INS_CLL: Op(0, OP_CLL, 0, 0, 0);
		case INS_RET: Op(0, OP_RET, 0, 0, 0);
		case INS_HLT: Op(0, OP_HLT, 0, 0, 0);
	}

#undef Op
}

static void gen_node_rom(node* n) {
	switch (n->type) {
		case NODE_RAW_DATA:
			gen_raw_data(n->address, &n->as.raw_data);
			break;
		case NODE_LABEL:
			break;
		case NODE_INSTRUCTION:
			gen_instruction(n->address, &n->as.instr);
			break;
	}
}

byte* generate_rom(node* nodes) {
	memset(data, 0, ROM_SIZE);

	node* curr = nodes;
	while (curr) {
		gen_node_rom(curr);
		curr = curr->next;
	}

	return data;
}