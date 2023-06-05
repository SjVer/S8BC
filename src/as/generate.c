#include <memory.h>
#include "as/generate.h"
#include "as/common.h"

byte data[ROM_SIZE];

#define Rel(addr) (data + addr - ROM_START + 1)

static void gen_raw_data_rom(word a, raw_data_node* d) {
	if (d->is_array) // string
		memcpy(Rel(a), d->as.array, d->length);
    else if (d->length == 1) // byte
		*Rel(a) = d->as.byte;
    else if (d->length == 2) { 
		Assert(!d->is_ident, "unresolved label");
		*Rel(a) = d->as.word & 0xff;
		*Rel(a + 1) = d->as.word >> 8;
	}
}

static void gen_instruction_rom(word a, instr_node* i) {

}

static void gen_node_rom(node* n) {
	switch (n->type) {
		case NODE_RAW_DATA:
			gen_raw_data_rom(n->address, &n->as.raw_data);
			break;
		case NODE_LABEL:
			break;
		case NODE_INSTRUCTION:
			gen_instruction_rom(n->address, &n->as.instr);
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

	printf("%02x %02x\n", data[ROM_SIZE - 2], data[ROM_SIZE - 1]);
	return data;
}