#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

/* Write 100 integers (in binary) to a file with the name given by the command-line
 * argument.  You can write them from 0 to 99 or use random to write random numbers.
 * This program creates a data file for use by the time_reads program.
 */

int main(int argc, char **argv) {
    
    unsigned int seed = clock();
    srandom(seed);

    if (argc != 2) {
        fprintf(stderr, "Usage: write_test_file filename\n");
        exit(1);
    }

    FILE *fp;
    if ((fp = fopen(argv[1], "w")) == NULL) { //if we're doing this in binary, shouldn't we be using 'wb'?
        perror("fopen");
        exit(1);
    }

    // TODO: complete this program according its description above.
    
    for (int i = 0; i < 100; i++)
    {
        int randInt = random() % 100;
        write(fileno(fp), &randInt, sizeof(i));
    }

    fclose(fp);
    return 0;
}
