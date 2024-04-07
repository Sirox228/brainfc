#include "lexer.h"
#include <cstdlib>
#include <iostream>

token getToken(std::string& code, uint64_t idx) {
  switch (code[idx]) {
    case '+': return tk_plus;
    case '-': return tk_minus;
    case '>': return tk_more;
    case '<': return tk_less;
    case '[':
      {
        bool foundClose = false;
        int nc = 0;
        if (idx+1 == code.length()) {
          goto fail;
        }
        for (int i = idx+1; i < code.length(); i++) {
          if (code[i] == '[') {
            nc++;
          }
          if (code[i] == ']') {
            if (!nc) {
              foundClose = true;
              break;
            } else {
              nc--;
            }
          }
        }
        if (!foundClose) {
fail:
          std::cout << "failed to find matching ] for [ at position " << idx+1 << std::endl;
          exit(1);
        }
        return tk_lbracket;
      }
    case ']': 
      {
        bool foundOpen = false;
        int no = 0;
        if (idx-1 < 0) {
          goto fail2;
        }
        for (int i = idx-1; i > 0; i--) {
          if (code[i] == ']') { 
            no++;
          }
          if (code[i] == '[') {
            if (!no) {
              foundOpen = true;
              break;
            } else {
              no--;
            }
          }
        }
        if (!foundOpen) {
fail2:
          std::cout << "failed to find matching [ for ] at position " << idx+1 << std::endl;
          exit(1);
        }
        return tk_rbracket;
      }
    case '.': return tk_dot;
    case ',': return tk_comma;
    default: return tk_skip;
  }
}
