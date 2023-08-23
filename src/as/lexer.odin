package as

import "core:log"
import "core:unicode"
import "core:strings"
import "core:strconv"

l_source : string
l_start, l_offset : int
l_line : int
l_char : rune

init_lexer :: proc(src : string) {
    l_source = src
    l_start = 0
    l_offset = 0
    l_line = 1
    l_char = rune(l_source[0])
}

l_is_at_end :: proc() -> bool {
    return l_offset >= len(l_source)
}

l_advance :: proc() -> rune {
    l_offset += 1

    if !l_is_at_end() {
        l_char = rune(l_source[l_offset])
        return l_char
    } else {
        return '\x00'
    }
    
}

l_is_start_of_lit :: proc(ch : rune) -> bool {
    return ch == '$' || ch == '%' || unicode.is_number(ch)
}

l_curr_string :: proc() -> string {
    return strings.cut(l_source, l_start, l_offset - l_start)
}

l_make_token :: proc(type : Token_Type, as : Token_As = nil) -> Token {
    return Token {
        type = type,
        line = l_line,
        length = l_offset - l_start,
        as = as,
    }
}

l_skip_spaces :: proc() {
    for !l_is_at_end() {
        switch l_char {
            case ' ', '\r', '\t':
                l_advance()
            case '\n':
                l_line += 1
                l_advance()
            
            case ';':
                // comments
                for l_char != '\n' && !l_is_at_end() {
                    l_advance()
                }

            case:
                return
        }
    }
}

l_instr_or_ident :: proc() -> Token {
    for unicode.is_alpha(l_char) || unicode.is_number(l_char) {
        l_advance()
    }
    ident := l_curr_string()
    
    switch ident {
        case "x": return l_make_token(.Register_X)
        case "y": return l_make_token(.Register_Y)

        case "nop": return l_make_token(.Instruction, .NOP)
        
        case "lda": return l_make_token(.Instruction, .LDA)
        case "ldx": return l_make_token(.Instruction, .LDX)
        case "ldy": return l_make_token(.Instruction, .LDY)
        case "sta": return l_make_token(.Instruction, .STA)
        case "stx": return l_make_token(.Instruction, .STX)
        case "sty": return l_make_token(.Instruction, .STY)
        case "sti": return l_make_token(.Instruction, .STI)

        case "tax": return l_make_token(.Instruction, .TAX)
        case "tay": return l_make_token(.Instruction, .TAY)
        case "txa": return l_make_token(.Instruction, .TXA)
        case "tya": return l_make_token(.Instruction, .TYA)
        case "sax": return l_make_token(.Instruction, .SAX)
        case "say": return l_make_token(.Instruction, .SAY)
        case "sxy": return l_make_token(.Instruction, .SXY)

        case "tsx": return l_make_token(.Instruction, .TSX)
        case "txs": return l_make_token(.Instruction, .TXS)
        case "psh": return l_make_token(.Instruction, .PSH)
        case "pll": return l_make_token(.Instruction, .PLL)
        case "pop": return l_make_token(.Instruction, .POP)

        case "and": return l_make_token(.Instruction, .AND)
        case "ior": return l_make_token(.Instruction, .IOR)
        case "xor": return l_make_token(.Instruction, .XOR)
        case "shl": return l_make_token(.Instruction, .SHL)
        case "shr": return l_make_token(.Instruction, .SHR)
        case "not": return l_make_token(.Instruction, .NOT)

        case "add": return l_make_token(.Instruction, .ADD)
        case "sub": return l_make_token(.Instruction, .SUB)
        case "mul": return l_make_token(.Instruction, .MUL)
        case "div": return l_make_token(.Instruction, .DIV)
        case "inc": return l_make_token(.Instruction, .INC)
        case "dec": return l_make_token(.Instruction, .DEC)
        case "cmp": return l_make_token(.Instruction, .CMP)

        case "jmp": return l_make_token(.Instruction, .JMP)
        case "jzs": return l_make_token(.Instruction, .JZS)
        case "jzn": return l_make_token(.Instruction, .JZN)
        case "jcs": return l_make_token(.Instruction, .JCS)
        case "jcn": return l_make_token(.Instruction, .JCN)
        case "jes": return l_make_token(.Instruction, .JES)
        case "jen": return l_make_token(.Instruction, .JEN)
        case "jls": return l_make_token(.Instruction, .JLS)
        case "jln": return l_make_token(.Instruction, .JLN)
        case "jgs": return l_make_token(.Instruction, .JGS)
        case "jgn": return l_make_token(.Instruction, .JGN)
        case "jsr": return l_make_token(.Instruction, .JSR)
        case "rts": return l_make_token(.Instruction, .RTS)
        case "rti": return l_make_token(.Instruction, .RTI)
        case "hlt": return l_make_token(.Instruction, .HLT)
    }
    return l_make_token(.Abs_Identifier, ident)
}

l_is_hex_digit :: proc(ch : rune) -> bool {
    switch ch {
        case 'a'..='f', 'A'..='F', '0'..='9':
            return true
    }
    return false
}

l_number :: proc(do_offset := false) -> uint {
    base : int
    descr : string

    if l_char == '%' {
        // binary
        l_advance()
        for l_char == '1' || l_char == '0' { l_advance() }

        base = 2
        descr = "binary"
    }
    else if l_char == '$' {
        // hexadecimal
        l_advance()
        for l_is_hex_digit(l_char) { l_advance() }

        base = 16
        descr= "hexadecimal"
    }
    else {
        // decimal
        for unicode.is_digit(l_char) { l_advance() }

        base = 10
        descr = "decimal"
    }

    l_offset := int(do_offset) + int(base != 10)
    str := strings.cut(l_curr_string(), l_offset)
    
    n, ok := strconv.parse_uint(str, base)
    if !ok { abort("invalid %v l_number at l_line %d", descr, l_line) }
    return n
}

l_any_value :: proc() -> Token {
    first_char := l_char
    if l_char == '#' || l_char == '@' { l_advance() }

    if unicode.is_letter(l_char) {
        // identifier
        for unicode.is_alpha(l_char) || unicode.is_number(l_char) {
            l_advance()
        }

        if first_char == '#' {
            return l_make_token(.Imm_Identifier, l_curr_string())
        } else if first_char == '@' {
            abort("expected an l_offset after '@' at l_line %d", l_line)
        } else {
            return l_make_token(.Abs_Identifier, l_curr_string())
        }
    }
    else {
        // literal
        n := l_number(first_char == '#' || first_char == '@')

        if first_char == '#' {
            if n > 0xff { 
                abort("immediate l_number '%d' too large at l_line %d", n, l_line)
            }
            return l_make_token(.Imm_Literal, Imm_Literal(n))
        }
        else if first_char == '@' {
            if n > 0xff { 
                abort("l_offset '%d' too large at l_line %d", n, l_line)
            }
            return l_make_token(.Stk_Literal, Imm_Literal(n))
        }
        else {
            if n > 0xffff { 
                abort("l_number '%d' too large at l_line %d", n, l_line)
            }
            return l_make_token(.Abs_Literal, Abs_Literal(n))
        }
    }
}

l_escaped_string :: proc() -> Token {
    l_advance()
    b, _ := strings.builder_make()

    for !l_is_at_end() && l_char != '"' {
        if l_char == '\n' { abort("unterminated string at l_line %d", l_line) }

        if l_char == '\\' {
            // handle escape sequences
            l_advance()
            switch l_char {
                case 'a': strings.write_rune(&b, '\a')
                case 'b': strings.write_rune(&b, '\b')
                case 'f': strings.write_rune(&b, '\f')
                case 'n': strings.write_rune(&b, '\n')
                case 'r': strings.write_rune(&b, '\r')
                case 't': strings.write_rune(&b, '\t')
                case 'v': strings.write_rune(&b, '\v')
                case '\\': strings.write_rune(&b, '\\')
                case '\'': strings.write_rune(&b, '\'')
                case '\"': strings.write_rune(&b, '\"')
                case '0': strings.write_rune(&b, '\x00')
                case : abort("invalid escape sequence '\\%v' at l_line %d", l_char, l_line)
            }
        } else {
            strings.write_rune(&b, l_char)
        }

        l_advance()
    }

    if l_char != '"' {
        abort("unterminated string at l_line %d", l_line)
    }

    return l_make_token(.String, strings.to_string(b))
}

l_directive :: proc() -> Token {
    for unicode.is_alpha(l_char) || unicode.is_number(l_char) {
        l_advance()
    }
    ident := strings.cut(l_curr_string(), 1)
    
    dir : Directive
    switch ident {
        case "byte": dir = .Byte
        case "word": dir = .Word
        case "string": dir = .String
        case : abort("invalid l_directive '%s' at l_line %d", ident, l_line)
    }

    return l_make_token(.Directive, dir)
}

scan_token :: proc() -> Token {
    l_skip_spaces()

    l_start = l_offset

    if l_is_at_end() {
        return l_make_token(.Eof)
    }
    
    if unicode.is_letter(l_char) {
        return l_instr_or_ident()
    } else if l_char == '#' || l_char == '@' || l_is_start_of_lit(l_char) {
        return l_any_value()
    } else if l_char == '"' { 
        return l_escaped_string()
    } else if l_char == '.' {
        l_advance()
        return l_directive()
    } else if l_char == ':' { 
        l_advance()
        return l_make_token(.Colon)
    } else if l_char == '=' { 
        l_advance()
        return l_make_token(.Equal)
    } else {
        abort("unexpected character '%c' at l_line %d", l_char, l_line)
    }
}