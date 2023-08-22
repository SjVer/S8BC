package as

import "core:log"

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

p_directive :: proc() -> Node {
    ...
}

p_node :: proc() -> Node {
    if p_match(.Directive) { return p_directive() }

    abort("unexpected token at line %d", p_curr_tok.line)
}

parse :: proc() -> [dynamic]Node {
    nodes := make_dynamic_array([dynamic]Node)

    for p_advance().type != .Eof {
        log.debug(p_curr_tok.type)

        node := p_node()
        log.debug(node)
        append(&nodes, node)
    }

    return nodes
}