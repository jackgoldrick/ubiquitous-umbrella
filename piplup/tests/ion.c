#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
/* NOTES:
    - The ? operator in C is part of the conditional (ternary) operator, 
    which is used as a shorthand for an if-else statement. 
    It takes three operands and is used as follows:

        condition ? expression_if_true : expression_if_false;

    condition is evaluated first. If it is true (non-zero), 
    expression_if_true is evaluated and its result is returned.
    If condition is false (zero), expression_if_false is evaluated
    and its result is returned.

    - The __VA_ARGS__ macro is a variable argument macro that represents the variable number of arguments passed to a function.
    It is used in the context of a variadic macro (a macro that takes a variable number of arguments).
    The __VA_ARGS__ macro is replaced by the arguments passed to the variadic macro when it is expanded.
    For example, consider the following variadic macro that takes a format string and a variable number of arguments and prints them using printf():

        #define LOG(format, ...) printf(format, __VA_ARGS__)   

    The __VA_ARGS__ macro is used to represent the variable number of arguments passed to the LOG macro.
    When the LOG macro is expanded, the __VA_ARGS__ macro is replaced by the arguments passed to the macro.
    For example, LOG("Hello, %s!\n", "world") expands to printf("Hello, %s!\n", "world").

    - difference between n and (n) in C:
        n is a variable name, while (n) is an expression that evaluates to the value of the variable n.
        For example, if n is defined as int n = 5;, then n is the variable name, and (n) is the expression that evaluates to 5.

    - The offsetof() macro in C is used to determine the offset of a member within a struct or union.
*/

#define MAX(x,y) ((x) >= (y) ? (x) : (y)) // return the maximum of two values



/*  

        Stretchy Buffers

 */

typedef struct BufHdr {
    size_t len;
    size_t cap;
    char buf[0];

} BufHdr;
// offsetof returns the offset of the member from the beginning of the struct in bytes basically like handling empty space? 
// double underscore is a convention for private functions (backend functions)
#define buf__hdr(b) ((BufHdr *)((char *)(b) - offsetof(BufHdr, buf))) 

// helper fit functions to resize the buffer

#define buf__fits(b,n) (buf_len(b) + (n) <= buf_cap(b)) // check if the buffer fits


// #define buf__fit(b,n) ((n) > buf_cap(b) ? (b) = buf__grow((b), (n), sizeof(*(b))) : 0) // if n is greater than the capacity of the buffer, then grow the buffer
// if the buffer fits, then do nothing, otherwise grow the buffer, assigne the new buffer to b
#define buf__fit(b,n) (buf__fits(b, n) ? 0 : (b) = buf__grow((b), buf_len(b) + (n), sizeof(*(b)))) 

/* 
** single underscore is a public function convention (frontend functions)
** two cases need to be handled:
**      1. if the buffer is NULL, then we need to allocate a new buffer
**      2. if the buffer is not NULL, then we need to reallocate the buffer
*/ 

#define buf_len(b) ((b) ? buf__hdr(b)->len : 0) // if b is not null, then return the length of the buffer, otherwise return 0

// the same thing has to be done for the capacity of the buffer

#define buf_cap(b) ((b) ? buf__hdr(b)->cap : 0) // if b is not null, then return the capacity of the buffer, otherwise return 0

//#define buf_push(b, ...) (buf__fit(b, 1), (b)[buf__hdr(b)->len++] = (__VA_ARGS__)) // push an element onto the buffer
#define buf_push(b, ... ) (buf__fit(b, 1), (b)[buf_len(b)++] = (__VA_ARGS__)) // push an element onto the buffer: "simplest version"


/* Implementation of the buf__grow function:
    - if the buffer is NULL, then allocate a new buffer with the specified capacity
    - if the buffer is not NULL, then reallocate the buffer with the specified capacity
    - copy the elements from the old buffer to the new buffer
    - update the length and capacity of the buffer
    - return the new buffer

*/

void *buf__grow(const void *buf, size_t new_len, size_t elem_size) {
    assert(buf_cap(buf) <= (SIZE_MAX - 1) / 2); // check for overflow
    size_t new_cap = MAX(16, MAX(1 + 2 * buf_cap(buf), new_len)); // calculate the new capacity
    assert(new_len <= new_cap); // check for overflow
    assert(new_cap <= (SIZE_MAX - offsetof(BufHdr, buf)) / elem_size); // check for overflow
    size_t new_size = offsetof(BufHdr, buf) + new_cap * elem_size; // calculate the new size
    BufHdr *new_hdr;
    if (buf) {
        new_hdr = realloc(buf__hdr(buf), new_size); // reallocate the buffer
    } else {
        new_hdr = malloc(new_size); // allocate a new buffer
        new_hdr->len = 0;
    }
    new_hdr->cap = new_cap; // update the capacity
    return new_hdr->buf; // return the new buffer
}


int main(int argc, char **argv) {
    int *buf = NULL;
    //buf_push(buf, 10);
    for (int i = 0; i < buf_len(buf); i++) {
        printf("%d\n", buf[i]);
    }
    // buf_free(buf);
    return 0;
}