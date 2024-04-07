#include "lexer.h"

token getToken(std::string& code, uint64_t idx) {
  switch (code[idx]) {
    case '+': return tk_plus;
    case '-': return tk_minus;
    case '>': return tk_more;
    case '<': return tk_less;
    case '[': return tk_lbracket;
    case ']': return tk_rbracket;
    case '.': return tk_dot;
    case ',': return tk_comma;
    default: return tk_skip;
  }
}
