package as

import "../spec"
import "core:log"
import "core:fmt"

p_prev_tok, p_curr_tok : Token

init_parser :: proc() {}

p_advance :: proc() -> Token {
    p_prev_tok = p_curr_tok
    p_curr_tok = scan_token()
    return p_curr_tok
}

p_check :: proc(type: Token_Type) -> bool {
    return p_curr_tok.type == type
}

p_match :: proc(type: Token_Type) -> bool {
    if p_check(type) { 
        p_advance()
        return true
    }
    return false
}

p_consume :: proc(type: Token_Type, what: string) {
    if !p_match(type) {
        abort("expected %v at line %d", what, p_curr_tok.line)
    }
}

p_directive :: proc() -> Node {
    switch p_prev_tok.as.(Directive) {
        case .Byte:
            if p_match(.Imm_Identifier) {
                return Node {
                    type = .Raw_Data,
                    as = Raw_Data_Node {
                        length = 1,
                        as = p_prev_tok.as.(string)
                    }
                }
            } else {
                p_consume(.Imm_Literal, "an immediate value")
                return Node {
                    type = .Raw_Data,
                    as = Raw_Data_Node {
                        length = 1,
                        as = p_prev_tok.as.(Imm_Literal)
                    }
                }
            }
        case .Word:
            if p_match(.Abs_Identifier) {
                return Node {
                    type = .Raw_Data,
                    as = Raw_Data_Node {
                        length = 2,
                        as = p_prev_tok.as.(string)
                    }
                }
            } else {
                p_consume(.Abs_Literal, "a value")
                return Node {
                    type = .Raw_Data,
                    as = Raw_Data_Node {
                        length = 2,
                        as = p_prev_tok.as.(Abs_Literal)
                    }
                }
            }
        case .String:
            p_consume(.String, "a string")
            return Node {
                type = .Raw_Data,
                as = Raw_Data_Node {
                    length = len(p_prev_tok.as.(string)),
                    as = p_prev_tok.as.(string)
                }
            }
    }
    
    abort("invalid directive")
}

p_alias :: proc() -> Node {
    name := p_prev_tok.as.(string)

    assert(p_match(.Equal))
    p_consume(.Abs_Literal, "an address")

    return Node {
        type = .Alias,
        as = Alias_Node {
            name = name,
            address = p_prev_tok.as.(Abs_Literal)
        }
    }
}

p_label :: proc() -> Node {
    label_tok := p_prev_tok
    p_consume(.Colon, "a label or alias")

    if label_tok.type == .Abs_Identifier {
        return Node {
            type = .Label,
            as = Label_Node(label_tok.as.(string))
        }
    } else {
        assert(label_tok.type == .Abs_Literal)
        return Node {
            type = .Label,
            as = Label_Node(label_tok.as.(Abs_Literal))
        }
    }
}

p_op :: proc(instr: ^Instr_Node, modes: bit_set[spec.OperandMode]) {
    if .Imm in modes && p_match(.Imm_Literal) {
        instr.op_mode = .Imm
        instr.operand = p_prev_tok.as.(Imm_Literal)
    }
    else if .Imm in modes && p_match(.Imm_Identifier) {
        instr.op_mode = .Imm
        instr.operand = p_prev_tok.as.(string)
    }
    else if .OpX in modes && p_match(.Register_X) {
        instr.op_mode = .OpX
        instr.operand = nil
    }
    else if .OpY in modes && p_match(.Register_Y) {
        instr.op_mode = .OpY
        instr.operand = nil
    }
    else if .Stk in modes && p_match(.Stk_Literal) {
        instr.op_mode = .Stk
        instr.operand = p_prev_tok.as.(Imm_Literal)
    }
    else if .Abs in modes && p_match(.Abs_Literal) {
        instr.op_mode = .Abs
        instr.operand = p_prev_tok.as.(Abs_Literal)
    }
    else if .Abs in modes && p_match(.Abs_Identifier) {
        instr.op_mode = .Abs
        instr.operand = p_prev_tok.as.(string)
    }
    else if .Imp in modes || .None in modes {
        instr.op_mode = .Imp in modes ? .Imp : .None
        instr.operand = nil
    }
    else {
        abort("expected an operand at line %d", p_curr_tok.line)
    }
}

p_instruction :: proc() -> Node {
    instr := Instr_Node {
        instr = p_prev_tok.as.(Instruction),
    }

    // parse operand(?)
    switch p_prev_tok.as.(Instruction) {
        case .NOP: p_op(&instr, {.None})
        
        // load/store operations
        case .LDA: p_op(&instr, {.Imm, .OpX, .OpY, .Stk, .Abs})
        case .LDX: p_op(&instr, {.Imm, .OpY, .Stk, .Abs})
        case .LDY: p_op(&instr, {.Imm, .OpX, .Stk, .Abs})
        case .LDI: p_op(&instr, {.Abs})
        case .STA: p_op(&instr, {.OpX, .OpY, .Stk, .Abs})
        case .STX:
        case .STY:
        case .STI: p_op(&instr, {.Abs})

        // register operations
        case .TAX:
        case .TAY:
        case .TXA:
        case .TYA:
        case .SAX:
        case .SAY:
        case .SXY: p_op(&instr, {.None})

        // stack operations
        case .TSX:
        case .TXS: p_op(&instr, {.None})
        case .PSH: p_op(&instr, {.Imm, .Imp, .OpX, .OpY})
        case .PLL:
        case .POP: p_op(&instr, {.Imp, .OpX, .OpY})

        // bitwise operations
        case .AND:
        case .IOR:
        case .XOR:
        case .SHL:
        case .SHR: p_op(&instr, {.Imm, .OpX, .Abs})
        case .NOT: p_op(&instr, {.Imp, .OpX, .OpY, .Abs})

        // numerical operations
        case .ADD:
        case .SUB:
        case .MUL:
        case .DIV: p_op(&instr, {.Imm, .OpX, .Abs})
        case .INC:
        case .DEC: p_op(&instr, {.Imp, .OpX, .OpY, .Abs})
        case .CMP: p_op(&instr, {.Imm, .OpX, .OpY, .Abs})

        // control flow operations
        case .JMP:
        case .JZS:
        case .JZN:
        case .JCS:
        case .JCN:
        case .JES:
        case .JEN:
        case .JLS:
        case .JLN:
        case .JGS:
        case .JGN:
        case .JSR: p_op(&instr, {.Abs})
        case .RTS:
        case .RTI:
        case .HLT: p_op(&instr, {.None})
    }

    return Node {
        type = .Instruction,
        as = instr
    }
}

p_node :: proc() -> Node {
    if p_match(.Directive) {
        return p_directive()
    }
    else if p_match(.Abs_Identifier) || p_match(.Abs_Literal) {
        if p_prev_tok.type == .Abs_Identifier && p_check(.Equal) {
            return p_alias()
        } else {
            return p_label()
        }
    }
    else if p_match(.Instruction){
        return p_instruction()
    }

    abort("unexpected token at line %d", p_curr_tok.line)
}

parse :: proc() -> [dynamic]Node {
    nodes := make_dynamic_array([dynamic]Node)

    p_advance()
    for p_curr_tok.type != .Eof {
        node := p_node()
        // fmt.println(node.as)
        append(&nodes, node)
    }

    return nodes
}