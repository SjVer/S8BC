#include <string.h>
#include "as/address.h"
#include "as/common.h"

struct label {
    char* ident;
    word address;
    struct label* next;
}* labels;

word curr_address;

static void set_curr_addr(int address) {
    if (address < ROM_START || address > ROM_END) {
        Log_err("addresses exceeded ROM bounds");
        Abort(STATUS_CODEGEN_ERROR);
    }
    curr_address = address;
}

static void incr_curr_addr(int offset) {
    set_curr_addr(curr_address + offset);
}

static void add_labels_in_node(node* n) {
    if (n->type == NODE_RAW_DATA) {
        incr_curr_addr(n->as.raw_data.length);
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
                    n->as.label.as.ident, curr_address);
        } else {
            // set the current address
            set_curr_addr(n->as.label.as.literal);
        }
    }
    else if (n->type == NODE_INSTRUCTION) {
        instr_node i = n->as.instr;
        incr_curr_addr(1 + i.has_arg + !i.arg_is_imm);
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
        incr_curr_addr(1 + i->has_arg + !i->arg_is_imm);
        if (i->has_arg) {
            if (i->arg_is_ident) {
                word addr = find_label(i->arg_as.ident);
                free(i->arg_as.ident);
                i->arg_is_ident = false;
                i->arg_as.literal = addr;
            }
        }
    }
}

void solve_addresses(node* nodes) {
    // add labels
    labels = NULL;
    set_curr_addr(ROM_START);
    node* curr = nodes;
    while (curr) {
        add_labels_in_node(curr);
        curr = curr->next;
    }

    // generate addresses
    set_curr_addr(ROM_START);
    curr = nodes;
    while (curr) {
        solve_node_address(curr);
        curr = curr->next;
    }
}