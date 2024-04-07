#ifndef LEXER_H
#define LEXER_H

#include <stdint.h>
#include <string>

enum token {
  tk_more = 0,
  tk_less = 1,
  tk_plus = 2,
  tk_minus = 3,
  tk_dot = 4,
  tk_comma = 5,
  tk_lbracket = 6,
  tk_rbracket = 7,
  tk_skip = -1
};

token getToken(std::string& code, uint64_t idx);

#endif
