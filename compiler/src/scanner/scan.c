#include "defs.h"
#include "data.h"
#include "decl.h"

#define TEXTLEN 100 // Define the TEXTLEN constant with an appropriate value

/* The scan.c file holds the functions of our lexical scanner. We are going to read in one character at a time from our input file. 
** However, there will be times when we need to "put back" a character if we have read too far ahead in the input stream.
** We also want to track what line we are currently on so that we can print the line number in our debug messages. 
** All of this is done by the next() function: 
*/


// get the next character from the input file
static int next(void) {
    int c;
    if (Putback) { // use the character put back if there is one
        c = Putback; 
        Putback = 0; // reset Putback to 0
        return c;
    }

    c = fgetc(Infile); // get the next character from the input file
        
    if ('\n' == c) // increment the line number if we encounter a newline character
        Line++;

    return c;

}

//  put back an unwanted character
static void putback(int c) {
    Putback = c;
}

// skip past input that we don't need to deal with
// i.e., whitespace, newlines, and comments
// return the next character that we do need
static int skip(void) {

    int c;

    c = next();
    while (c < 33) { // skip whitespace
        c = next();
    }

    if ('/' == c) { // skip comments
        if ('/' == (c = next())) {
            while ('\n' != c) {
                c = next();
            }
        } else if ('*' == c) {
            while (1) {
                c = next();
                if ('*' == c) {
                    if ('/' == (c = next())) {
                        break;
                    }
                }
            }
        } else {
            putback(c);
            c = '/';
        }
    } 

    return (c);

}

// scan and return the token found in the input
// Return 1 if token valid, 0 if not
int scan (struct token *t) {

    int c;

    // skip whitespace, newlines, and comments
    c = skip();
    // Determine the based on 
    // the character we have found

    switch (c) {

        case EOF:
            return (0);

        case '+':
            t->token = T_PLUS;
            break;

        case '-':
            t->token = T_MINUS;
            break;

        case '*':
            t->token = T_STAR;
            break;
        
        case '/':
            t->token =  T_SLASH;
            break;

        default: 

            // If digit, scan literal
            // helps with hexadeciomal and octal literals apparently  
            if (isdigit(c)) {
                t->intValue = scanInt(c);
                t->token = T_INTLIT;
                break;
            }

            fprintf(stderr, "Unrecognised character %c on line %d\n", c, Line);
            exit(1);

    }

    return (1);
    
}

scanInt(int c) {

    int k, val = 0;


    while ((k = chrpos("0123456789", c)) >= 0) {
        val = val * 10 + k;
        c = next();
    }


    putback(c);
    return val;
    
}


// Return the Posistion of character c
// in string s, or -1 if c is not found
int chrpos(char *s, int c) {
    char *p;

    p = strchr(s, c);
    return (p ? p - s : -1);
}


/* // scan an identifier from the input file
static int scanident(int c, struct token *t) {
    int i = 0;

    for (; isalnum(c); c = next()) {
        if (i < TEXTLEN - 1) {
            t->lexeme[i++] = c;
        }
    }

    putback(c);
    t->lexeme[i] = 0;

    return (T_INTLIT);
}
 */