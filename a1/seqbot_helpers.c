#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "seqbot_helpers.h"

/* Return the melting temperature of sequence, or -1 if the sequence is invalid.
 * The melting temperature formula is given in the handout.
 * An invalid sequence is a sequence of length 0, or a sequence that contains
 * characters other than 'A', 'C', 'G', 'T'.
 */
int calculate_melting_temperature(char *sequence, int sequence_length)
{

    // TODO: complete this function
    return -1;
}

/* Prints the instructions to make a molecule from sequence.
 * If an invalid character is found in sequence print
 * "INVALID SEQUENCE" and return immediately
 */
void print_instructions(char *sequence, int sequence_length)
{
    // TODO: complete this function
}


/* Print to standard output all of the sequences of length k.
 * The format of the output is "<length> <sequence> 0" to 
 * correspond to the input format required by generate_molecules_from_file()
 * 
 * Reminder: you are not allowed to use string functions in these files.
 */
void generate_all_molecules(int k)
{
    // TODO: complete this function
}



/* Print the instructions for each of the sequences found in filename according
 * to the mode provided.
 * filename contains one sequence per line, and the format of each line is
 * "<length> <sequence> <mode>" where 
 *     - <length> is the number of characters in the sequence 
 *     - <sequence> is the array of DNA characters
 *     - <mode> is either 0, 1, 2, or 3 indicating how the <sequence> should 
 *              be modified before printing the instrutions. The modes have the 
 *              following meanings:
 *         - 0  - print instructions for sequence (unmodified)
 *         - 1  - print instructions for the the complement of sequence
 *         - 2  - print instructions for the reverse of sequence
 *         - 3  - print instructions for sequence where it is complemented 
 *                and reversed.
 * 
 * Error checking: If any of the following error conditions occur, the function
 * immediately prints "INVALID SEQUENCE" to standard output and exits the 
 * program with a exit code of 1.
 *  - length does not match the number of characters in sequence
 *  - length is not a positive number
 *  - sequence contains at least one invalid character
 *  - mode is not a number between 0 and 3 inclusive
 * 
 * You do not need to verify that length or mode are actually integer numbers,
 * only that they are in the correct range. It is recommended that you use a 
 * fscanf to read the numbers and fgetc to read the sequence characters.
 */
void generate_molecules_from_file(char* filename)
{
    // TODO: complete this function
}
