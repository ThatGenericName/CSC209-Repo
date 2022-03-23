#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <getopt.h>

#include <errno.h>

// The maximum length of an HTTP message line
#define MAX_LINE 256
// The maximum length of an HTTP response message
#define MAX_LENGTH 16*1024
// The size of a chunk of HTTP response to read from the pipe
#define CHUNK_SIZE 1024


void printError(char *str); // "str" added after the "*" because for some reason it doesnt exist
void printServerError(); // argument char *str removed
void printResponse(char *str);

int debug = 0;

int execute(char* resource, char* query, FILE* fp, char** result);

int main(int argc, char** argv) {
    FILE *fp = stdin; // default is to read from stdin
    // Parse command line options.
    // Whether verbose (debug) is used and the input file

    int opt;
    while((opt = getopt(argc, argv, "v")) != -1) {
        switch(opt) {
            case 'v':
                debug = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-v] [filename]\n", argv[0]);
                exit(1);
        }
    }
    if(optind < argc) {
        if((fp = fopen(argv[optind], "r")) == NULL) {
            perror("fopen");
            exit(1);
        }
    }

    // TODO Complete program
    // So if i'm understanding this correctly, I parse the HTTP request, and then
    // use that to call exec on the right program.
    // I then use a pipe the read the output of those programs (as it prints to stdout)
    // This also runs on a while loop
    char line[MAX_LINE];
    int lineCounter = 0;
    int requestCounter = 0;
    while (fgets(line, MAX_LINE, fp) != NULL){
        if (strncmp("GET ", line, 4) == 0){
            // this is a request line;
            char* resStartPt = strchr(line, '/');
            char* queryStartPt = strchr(line, '?');
            char* queryEndPt;
            int hasQuery = 1;
            if (queryStartPt == NULL){
                queryStartPt = line;
                hasQuery = 0;
            }
            queryEndPt = strchr(resStartPt, ' ');
            int resLen;
            int queryLen;
            if (hasQuery){
                resLen = (queryStartPt - resStartPt);
                queryLen = (queryEndPt - queryStartPt) - 1;
                queryStartPt = queryStartPt + 1;
            }
            else{
                queryLen = 0;
                resLen = (queryEndPt - resStartPt);
            }
            char* resource = malloc(sizeof(char) * resLen + 1);
            if (resource == NULL){
                perror("malloc");
                printServerError();
                continue;
            }
            strncpy(resource, resStartPt, resLen);
            resource[resLen] = '\0';
        
            char* query;
            query = malloc(sizeof(char) * queryLen + 1);
            if (query == NULL){
                perror("malloc");
                printServerError();
                continue;
            }
            strncpy(query, queryStartPt, queryLen);
            query[queryLen] = '\0';

            // onto the running part
            if (debug) printf("DEBUG: Request #%d: %s\n", requestCounter, line);
            if (debug) printf("DEBUG: resource: %s, query %s\n", resource, query);
            char* result;
            int executeRes = execute(resource, query, fp, &result);
            if (executeRes){
                if (executeRes == 2){
                    printError(resource);
                }
                else{
                    printServerError();
                }
            }
            else{
                printResponse(result);
                free(result);
            }
            fflush(stdout);
            free(query);
            free(resource);
            requestCounter++;
        }
        lineCounter++;
    }
    if(debug) printf("DEBUG: lines read: %d\n", lineCounter);

    if(fp != stdin) {
        if(fclose(fp) == EOF) {
            perror("fclose");
            exit(1);
        }
    }
}

int execute(char* resource, char* query, FILE* fp, char** result){
    int setEnvRes = setenv("QUERY_STRING", query, 1);
    if (setEnvRes != 0){
        perror("setenv");
        return 1;
    }

    int printRedirect[2];
    if (pipe(printRedirect) == -1){
        perror("pipe");
        return 1;
    }

    int forkRes = fork();
    if (forkRes < 0){
        perror("fork");
        return 1;
    }
    else if (forkRes == 0){
        // child process
        // codes must be checked with wait and exit codes
        fclose(fp); // child process does not need the input stream;
        if (dup2(printRedirect[1], fileno(stdout)) == -1){
            perror("dup2");
            close(printRedirect[1]);
            exit(1); // 1 means server error
        }
        close(printRedirect[0]); // child does not need read end of the fd;
        if (execl(resource + 1, resource + 1, NULL) == -1){
            perror("execl");
            close(printRedirect[1]);
            if (errno == ENOENT){
                fclose(stdout); // desperation to solve issue
                exit(2); // code 2 means resource doesnt exist
            }
            exit(1); 
        }
        close(printRedirect[1]);
        exit(0);
    }
    else{
        //parent process;
        int returnCode = 0;
        close(printRedirect[1]);
        char chunk[CHUNK_SIZE + 1];
        char fullData[MAX_LENGTH + 1];
        char* fullDataPtr = &fullData[0]; // this is jank
        int bytesToRead = CHUNK_SIZE;
        int i = 0;
        memset(chunk, '\0', CHUNK_SIZE);
        int readRes = read(printRedirect[0], chunk, bytesToRead);
        int maxChunkNum = MAX_LENGTH / CHUNK_SIZE;

        while (readRes > 0 && i < maxChunkNum){
            strncpy(fullDataPtr, chunk, CHUNK_SIZE);
            fullDataPtr += CHUNK_SIZE;
            memset(chunk, '\0', CHUNK_SIZE);
            readRes = read(printRedirect[0], chunk, bytesToRead);
            i++;
        }
        int stat;
        wait(&stat);
        int exitedNorm = WIFEXITED(stat);
        int d2 = WIFSIGNALED(stat);
        if (debug) printf("DEBUG: Exited: %d, Signalled: %d\n", exitedNorm, d2);
        if (debug) fflush(stdout);
        if (exitedNorm){
            // exited normally
            int exitCode = WEXITSTATUS(stat);
            if (exitCode == 0){
                // exited without error.
                fullData[MAX_LENGTH] = '\0'; // in case for some reason the data doesnt null terminate.
                *result = malloc(sizeof(char) * MAX_LENGTH + 1);
                strncpy(*result, fullData, MAX_LENGTH);
            }
            else{
                *result = NULL;
                if (exitCode == 2){
                    // resource not found
                    returnCode = 2;
                }
                else{
                    returnCode = 1;
                }
                close(printRedirect[0]);
            }
        }
        else if (d2){
            *result = NULL;
            returnCode = 1;
            close(printRedirect[0]);
        }
        memset(fullData, '\0', MAX_LENGTH); // stack memory shinanigans.
        return returnCode;
    }
}

/* Print an http error page  
 * Arguments:
 *    - str is the path to the resource. It does not include the question mark
 * or the query string.
 */
void printError(char *str) {
    printf("HTTP/1.1 404 Not Found\r\n\r\n");

    printf("<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n");
    printf("<html><head>\n");
    printf("<title>404 Not Found</title>\n");
    printf("</head><body>\n");
    printf("<h1>Not Found</h1>\n");
    printf("The requested resource %s was not found on this server.\n", str);
    printf("<hr>\n</body></html>\n");
}


/* Prints an HTTP 500 error page 
 */
void printServerError() {
    printf("HTTP/1.1 500 Internal Server Error\r\n\r\n");

    printf("<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n");
    printf("<html><head>\n");
    printf("<title>500 Internal Server Error</title>\n");
    printf("</head><body>\n");
    printf("<h1>Internal Server Error</h1>\n");
    printf("The server encountered an internal error or\n");
    printf("misconfiguration and was unable to complete your request.<p>\n");
    printf("</body></html>\n");
}


/* Prints a successful response message
 * Arguments:
 *    - str is the output of the CGI program
 */
void printResponse(char *str) {
    printf("HTTP/1.1 200 OK\r\n\r\n");
    printf("%s", str);
}
