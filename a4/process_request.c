#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "ws_helpers.h"
#include "wrapsock.h"

char *getPath(char *str);

/* Process the HTTP request in cs
 *    If the path is an allowed program, create a child process,
 *    set up a pipe, redirect standard output to the pipe, and execute 
 *    the program
 * Return the pipe descriptor from which the parent process will read the 
 * CGI output
 * Return -1 if there is an error
 */
int processRequest(struct clientstate *cs) {

    // Check if the program requested is in the allowed set
    if(!validResource(cs->path)) {
        printNotFound(cs->sock);
        return(-1);
    }
    
    // Part of A3 code - use your own implementation or
    // wait until this full file is provided.
    
    return -1; /* should never get here */
}



/* Extract and return the resource string from the first line of a GET request
 * stored in str 
 */
char * getPath(char *str) {
    
    // Part of A3 - use your own implementation, or wait until this file is
    // provided in full

    return NULL;
}


/* Return the query string portion of a get request
 * Assume the only question mark in the request marks
 * the beginning of a query string.
 */
char *getQuery(char *resource) {
    
    // Part of A3 - use your own implementation, or wait until this file is
    // provided in full

    return NULL;
}
