#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "seqbot_helpers.h"

void generate_all_molec_recur(char *sequence, int left, int size);
void complement(char* sequence, int seqLength);
void reverse(char* sequence, int sequenceLength);
int errorCheck(char* sequence, int sequenceLength, int mode);

/* Return the melting temperature of sequence, or -1 if the sequence is invalid.
 * The melting temperature formula is given in the handout.
 * An invalid sequence is a sequence of length 0, or a sequence that contains
 * characters other than 'A', 'C', 'G', 'T'.
 */
int calculate_melting_temperature(char *sequence, int sequence_length)
{
    int nA = 0;
    int nT = 0;

    int nC = 0;
    int nG = 0;
    // TODO: complete this function

    for (int i = 0; i < sequence_length; i++)
    {
        switch(sequence[i]){
            case 'A':
                nA += 1;
                break;
            case 'T':
                nT += 1;
                break;
            case 'C':
                nC += 1;
                break;
            case 'G': 
                nG += 1;
                break;
        }
    }

    return ((nA + nT) * 2) + ((nC + nG) * 4);
}

/* Prints the instructions to make a molecule from sequence.
 * If an invalid character is found in sequence print
 * "INVALID SEQUENCE" and return immediately
 */
void print_instructions(char *sequence, int sequence_length)
{
    char currentCode = sequence[0];
    int currentCodeLength = 0;
    printf("START\n");
    for (int i = 0; i < sequence_length; i++)
    {
        if (sequence[i] != currentCode){
            printf("WRITE %c %d\n", currentCode, currentCodeLength);
            currentCode = sequence[i];
            currentCodeLength = 1;
        }
        else{
            currentCodeLength += 1;
        }
    }
    printf("WRITE %c %d\n", currentCode, currentCodeLength);
    int meltTemp = calculate_melting_temperature(sequence, sequence_length);
    printf("SET_TEMPERATURE %d\n", meltTemp);
    printf("END\n");
}

/* Print to standard output all of the sequences of length k.
 * The format of the output is "<length> <sequence> 0" to 
 * correspond to the input format required by generate_molecules_from_file()
 * 
 * Reminder: you are not allowed to use string functions in these files.
 */
void generate_all_molecules(int k)
{
    char *sequence = malloc(sizeof(char) * k);
    generate_all_molec_recur(sequence, k, k);
    free(sequence);
}

void generate_all_molec_recur(char *sequence, int left, int size){
    if (left == 0){
        printf("%d ", size);
        for (int i = 0; i < size; i++)
        {
            printf("%c", sequence[i]);
        }
        printf(" 0\n");
    }
    else{
        char choices[4] = {'A', 'C', 'G', 'T'};
        for (int i = 0; i < 4; i++)
        {
            sequence[size - left] = choices[i];
            generate_all_molec_recur(sequence, left - 1, size);
        }
    }
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
    // printf("reading file %s\n", filename);
    FILE *file = fopen(filename, "r");

    if (file == NULL){
        // make this do something later
        printf("file read fail\n");
    }

    int seqLength;

    while (fscanf(file, "%d ", &seqLength) == 1){
        char* sequence = malloc(sizeof(char) * seqLength + 1);
        if (fgets(sequence, seqLength + 1, file) != NULL){
            int mode = 0;

            char next = getc(file);
            if (next != ' '){
                while (!(next == '\n' || next == 0)){
                    if (next == 0){
                        break;
                    }
                    next = getc(file);
                }
            }
            else{
                ungetc(next, file);
                if (fscanf(file, "%d", &mode) == 1){
                    if (errorCheck(sequence, seqLength, mode) == 0){
                        switch(mode){
                            case 3:
                                complement(sequence, seqLength);
                            case 2:
                                reverse(sequence, seqLength);
                                break;
                            case 1:
                                complement(sequence, seqLength);
                                break;
                        }
                    print_instructions(sequence, seqLength);
                    continue;
                    }
                }
            }
        }

        printf("INVALID_SEQUENCE\n");
    }
}

void complement(char* sequence, int seqLength){
    for (int i = 0; i < seqLength; i++)
    {
        switch(sequence[i]){
            case 'A':
                sequence[i] = 'G';
                break;
            case 'T':
                sequence[i] = 'C';
                break;
            case 'C':
                sequence[i] = 'T';
                break;
            case 'G': 
                sequence[i] = 'A';
                break;
        }
    }    
}

void reverse(char* sequence, int sequenceLength){
    char* newSequence = malloc(sizeof(char) * sequenceLength);

    int counter = 0;
    for (int i = sequenceLength - 1; i >= 0; i--)
    {
        newSequence[counter] = sequence[i];
    }
    for (int i = 0; i < sequenceLength; i++)
    {
        sequence[i] = newSequence[i];
    }
}

int errorCheck(char* sequence, int sequenceLength, int mode){
    if (sequenceLength < 0){
        return 1;
    }
    if (mode > 3 || mode < 0){
        return 1;
    }
    for (int i = 0; i < sequenceLength; i++)
    {
        switch (sequence[i]){
            case 'A':
            case 'T':
            case 'C':
            case 'G': 
                break;
            default:
                return 1;
        }
    }
    return 0;
}
