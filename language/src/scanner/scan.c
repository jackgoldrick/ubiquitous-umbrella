/* The scan.c file holds the functions of our lexical scanner. We are going to read in one character at a time from our input file. 
** However, there will be times when we need to "put back" a character if we have read too far ahead in the input stream.
** We also want to track what line we are currently on so that we can print the line number in our debug messages. 
** All of this is done by the next() function: 
*/

// get the next character from the input file
static int next(void) {
    int c;
    
    if (Putback)

















}