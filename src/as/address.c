#include <string.h>
#include "as/address.h"
#include "as/common.h"

struct label {
    char* ident;
    word address;
    struct label* next;
}* labels;

word prev_address;
word curr_address;

static void set_curr_addr(int address) {
    // only check prev_address so that the incrementing
    // after writing to ROM_END doesn't cause an error

    if (prev_address < ROM_START || prev_address > ROM_END) {
        Log_err("address exceeded ROM bounds ($%04x)", prev_address);
        Abort(STATUS_CODEGEN_ERROR);
    }

    prev_address = curr_address;
    curr_address = address;
}

static void incr_curr_addr(int offset) {
    set_curr_addr(curr_address + offset);
}

static void add_labels_in_node(node* n) {
    if (n->type == NODE_RAW_DATA) {
        incr_curr_addr(n->as.raw_data.length);
    }
    else if (n->type == NODE_ALIAS) {
        // add the alias 
        struct label* l = malloc(sizeof(struct label));
        l->ident = n->as.alias.ident;
        l->address = n->as.alias.address;
        l->next = labels;
        labels = l;

        if (cli_args.verbose)
            Log("added alias \"%s\" for $%04x",
                l->ident, l->address);
    }
    else if (n->type == NODE_LABEL) {
        if (n->as.label.is_ident) {
            // add the label
            struct label* l = malloc(sizeof(struct label));
            l->ident = n->as.label.as.ident;
            l->address = curr_address;
            l->next = labels;
            labels = l;

            if (cli_args.verbose)
                Log("added label \"%s\" at $%04x",
                    l->ident, l->address);
        } else {
            // set the current address
            set_curr_addr(n->as.label.as.literal);
        }
    }
    else if (n->type == NODE_INSTRUCTION) {
        instr_node i = n->as.instr;
        incr_curr_addr(1);
        if (i.arg_type == TOK_IMM_IDENTIFIER
            || i.arg_type == TOK_IMM_LITERAL)
            incr_curr_addr(1);
        else if (i.arg_type == TOK_ABS_IDENTIFIER
            || i.arg_type == TOK_ABS_LITERAL)
            incr_curr_addr(2);
    }
}

static word find_label(char* ident) {
    struct label* l = labels;
    while (l && strcmp(l->ident, ident)) l = l->next;

    if (!l) {
        Log_err("use of undefined label: \"%s\"", ident);
        Abort(STATUS_CODEGEN_ERROR);
    }
    return l->address;
}

static void solve_node_address(node* n) {
    if (n->type == NODE_RAW_DATA) {
        raw_data_node* d = &n->as.raw_data;
        if (d->is_ident) {
            word addr = find_label(d->as.ident);
            free(d->as.ident);
            d->is_ident = false;
            d->as.word = addr;
        }

        n->address = curr_address;
        incr_curr_addr(d->length);
    }
    else if (n->type == NODE_LABEL) {
        label_node* l = &n->as.label;
        if (l->is_ident) {
            set_curr_addr(find_label(l->as.ident));
            // don't free the ident (for debug)
        }
        else set_curr_addr(l->as.literal);

        n->address = curr_address;
    }
    else if (n->type == NODE_INSTRUCTION) {
        n->address = curr_address;

        instr_node* i = &n->as.instr;
        incr_curr_addr(1);

        if (i->arg_type == TOK_IMM_LITERAL) incr_curr_addr(1);
        else if (i->arg_type == TOK_IMM_IDENTIFIER) {
            incr_curr_addr(1);

            word addr = find_label(i->as.ident);
            free(i->as.ident);
            i->arg_type = TOK_IMM_LITERAL;
            // i->as.imm_literal = (byte)(addr & 0xff);
            i->as.imm_literal = (byte)(addr & 0xff);
        }
        else if (i->arg_type == TOK_ABS_LITERAL) incr_curr_addr(2);
        else if (i->arg_type == TOK_ABS_IDENTIFIER) {
            incr_curr_addr(2);

            word addr = find_label(i->as.ident);
            free(i->as.ident);
            i->arg_type = TOK_ABS_LITERAL;
            i->as.abs_literal = addr;
        }
    }
}

void solve_addresses(node* nodes) {
    // add labels
    labels = NULL;
    prev_address = curr_address = ROM_START;
    node* curr = nodes;
    while (curr) {
        add_labels_in_node(curr);
        curr = curr->next;
    }

    // generate addresses
    prev_address = curr_address = ROM_START;
    curr = nodes;
    while (curr) {
        solve_node_address(curr);
        curr = curr->next;
    }
}