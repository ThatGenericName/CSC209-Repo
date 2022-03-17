/* The purpose of this program is to practice writing signal handling
 * functions and observing the behaviour of signals.
 */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

/* Message to print in the signal handling function. */
#define MESSAGE "%ld reads were done in %ld seconds.\n"

/* Global variables to store number of read operations and seconds elapsed. 
 */
long num_reads, seconds;


void handler(int code){
    // printf("caught SIGPROF, %d\n", code);
    printf(MESSAGE, num_reads, seconds);
    exit(0);
}

/* The first command-line argument is the number of seconds to set a timer to run.
 * The second argument is the name of a binary file containing 100 ints.
 * Assume both of these arguments are correct.
 */

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: time_reads s filename\n");
        exit(0);
    }
    seconds = strtol(argv[1], NULL, 10);

    FILE *fp;
    if ((fp = fopen(argv[2], "r")) == NULL) {
      perror("fopen");
      exit(1);
    }

    struct sigaction newact;
    newact.sa_handler = handler;
    newact.sa_flags = 0;
    sigemptyset(&newact.sa_mask);
    sigaction(SIGPROF, &newact, NULL);

    /* In an infinite loop, read an int from a random location in the file,
     * and print it to stderr.
     */

    //seeding rng
    unsigned int seed = clock();
    srandom(seed);

    int fd = fileno(fp);

    int maxSize = lseek(fd, 0, SEEK_END);
    int readData;
    int readLoc;

    num_reads = 0;

    //setting timer
    struct timeval timeValInt;
    timeValInt.tv_usec = 0;
    timeValInt.tv_sec = 0;

    struct timeval timeValDur;
    timeValDur.tv_sec = seconds;
    timeValDur.tv_usec = 0;

    struct itimerval timerStruct;
    timerStruct.it_interval = timeValInt;
    timerStruct.it_value = timeValDur;

    int res = setitimer(ITIMER_PROF, &timerStruct, NULL);

    if (res == -1){
        perror("setitimer");
        exit(1);
    }

    for (;;) {
        readLoc = random() % maxSize;
        lseek(fd, readLoc, SEEK_SET);
        read(fd, &readData, sizeof(int));
        num_reads++;
    }
    return 1; // something is wrong if we ever get here!
}
