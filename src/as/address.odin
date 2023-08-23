package as

import "../spec"
import "core:log"

a_labels : map[string]word

a_prev_addr, a_curr_addr : word

a_set_curr_addr :: proc(addr: word) {
    // only check prev_address so that the incrementing
    // after writing to ROM_END doesn't cause an error

    if (a_prev_addr < spec.ROM_START || a_prev_addr > spec.ROM_END) {
        abort("address exceeded ROM bounds ($%04x)", a_prev_addr)
    }

    a_prev_addr = a_curr_addr
    a_curr_addr = addr
}

a_incr_curr_addr :: proc(offset : uint = 1) {
    a_set_curr_addr(a_curr_addr + word(offset))
}

a_register_labels :: proc(node: Node) {
    switch node.type {
        case .Raw_Data:
            a_incr_curr_addr(node.as.(Raw_Data_Node).length)

        case .Alias:
            // add the alias as a label
            name := node.as.(Alias_Node).name
            address := node.as.(Alias_Node).address
            a_labels[name] = address
            log.debugf("added alias '%s' for $%04x", name, address)
            
        case .Label:
            switch label in node.as.(Label_Node) {
                case string:
                    // add the label
                    a_labels[label] = a_curr_addr
                    log.debugf("added label '%s' for $%04x", label, a_curr_addr)
                
                case word:
                    a_set_curr_addr(label)
            }
        
        case .Instruction:
            a_incr_curr_addr()
            instr := node.as.(Instr_Node)
            switch instr.op_mode {
                case .None, .Imp, .OpX, .OpY: break
                case .Imm, .Stk: a_incr_curr_addr()
                case .Abs: a_incr_curr_addr(2)
            }
    }
}

a_solve_address :: proc(node: ^Node) {
    switch node.type {
        case .Raw_Data:
            if ident, ok := node.as.(Raw_Data_Node).as.(string); ok {
                // replace the label
                if node.as.(Raw_Data_Node).length == 1 {
                    r := &node.as.(Raw_Data_Node)
                    r.as = byte(a_labels[ident] & 0xff)
                } else if node.as.(Raw_Data_Node).length == 2 {
                    r := &node.as.(Raw_Data_Node)
                    r.as = a_labels[ident]
                }
            }
            node.address = a_curr_addr
            a_incr_curr_addr(node.as.(Raw_Data_Node).length)

        case .Alias:
            // nothing to do
            break
            
        case .Label:
            // just set the current address
            switch label in node.as.(Label_Node) {
                case string: a_set_curr_addr(a_labels[label])
                case word: a_set_curr_addr(label)
            }
            node.address = a_curr_addr
        
        case .Instruction:
            node.address = a_curr_addr
            a_incr_curr_addr()
            
            instr := &node.as.(Instr_Node)
            switch instr.op_mode {
                case .None, .Imp, .OpX, .OpY:
                    break

                case .Imm, .Stk: 
                    a_incr_curr_addr()
                    if ident, ok := instr.operand.(string); ok {
                        instr.operand = byte(a_labels[ident] & 0xff)
                    }

                case .Abs:
                    a_incr_curr_addr(2)
                    if ident, ok := instr.operand.(string); ok {
                        instr.operand = a_labels[ident]
                    }
            }
    }
}

solve_addresses :: proc(nodes: [dynamic]Node) {
    a_labels = make_map(type_of(a_labels))
    a_prev_addr = spec.ROM_START
    a_curr_addr = spec.ROM_START

    // register all labels and aliases
    for node in nodes {
        a_register_labels(node)
    }

    a_prev_addr = spec.ROM_START
    a_curr_addr = spec.ROM_START

    // set the addresses of the instructions
    for &node in nodes {
        a_solve_address(&node)
    }
}