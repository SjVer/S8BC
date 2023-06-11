#pragma once
#define AS_DEBUG_H

#include "as/parser.h"

const char* string_of_ins(instruction ins);
void log_node(node* n);
void log_nodes(node* n);