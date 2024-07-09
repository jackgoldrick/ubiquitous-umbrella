#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Token Structure
struct token {
    int token;
    int intValue;

};

enum {
    T_PLUS,
    T_MINUS,
    T_STAR,
    T_SLASH,
    T_INTLIT
};

