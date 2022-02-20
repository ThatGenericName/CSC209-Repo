#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "constraints.h"

/* Create and initialize a constraints struct. 
 * Sets the fields to 0 or empty string.
 * Return a pointer to the newly created constraints struct.
 */
struct constraints *init_constraints() {
    struct constraints *newConstraints;
    newConstraints = malloc(sizeof(struct constraints));

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        newConstraints -> cannot_be[i] = 0;
    }
    for (int i = 0; i < WORDLEN; i++){
        // might replace this with a loop that just sets everything to the null terminator, I'm not too sure the specs for this part of the assignment.
        // though copying an empty string to another string in theory does just that.
        strncpy(newConstraints -> must_be[i], "", SIZE);
    }

    //TODO: check back later if "must_be" constraint should be setup
    
    return newConstraints;
}

/* Update the "must_be" field at "index" to be a string 
 * containing "letter"
 * The tile at this index is green, therefore the letter at "index"
 * must be "letter"
 */
void set_green(char letter, int index, struct constraints *con) {
    assert(islower(letter));
    assert(index >= 0 && index < SIZE);

    con -> must_be[index][0] = letter;
    for (int i = 1; i < SIZE; i++)
    {
        con -> must_be[index][i] = '\0';
    }
}

/* Update "con" by adding the possible letters to the string at the must_be 
 * field for "index".
 * - index is the index of the yellow tile in the current row to be updated
 * - cur_tiles is the tiles of this row
 * - next_tiles is the tiles of the row that is one closer to the solution row
 * - word is the word in the next row (assume word is all lower case letters)
 * Assume cur_tiles and next_tiles contain valid characters ('-', 'y', 'g')
 * 
 * Add to the must_be list for this index the letters that are green in the
 * next_tiles, but not green in the cur_tiles or green or yellow in the 
 * next_tiles at index.
 * Also add letters in yellow tiles in next_tiles.
 */
void set_yellow(int index, char *cur_tiles, char *next_tiles, 
                char *word, struct constraints *con) {

    assert(index >=0 && index < SIZE);
    assert(strlen(cur_tiles) == WORDLEN);
    assert(strlen(next_tiles) == WORDLEN);
    assert(strlen(word) == WORDLEN);

    char possCharSet[ALPHABET_SIZE];
    char impossCharSet[ALPHABET_SIZE];

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        possCharSet[i] = 0;
        impossCharSet[i] = 0;
    }

    for (int i = 0; i < WORDLEN; i++)
    {
        if (i == index){
            if (next_tiles[index] == 'g'){
                if (cur_tiles[index] == 'y'){
                    impossCharSet[word[index] - 'a'] = 1;
                }
            }
        }
        else{
            if (next_tiles[i] == 'g'){
                if (cur_tiles[i] != 'g'){
                    possCharSet[word[i] - 'a'] = 1;
                }
                if (cur_tiles[i] == 'g'){
                    impossCharSet[word[i] - 'a'] = 1;
                }
            }
            else if (next_tiles[i] == 'y'){
                possCharSet[word[i] - 'a'] = 1;
            }
        }

    }

    char mustBeStr[SIZE];
    int mustBeStrInc = 0;

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (possCharSet[i] && !impossCharSet[i]){
            mustBeStr[mustBeStrInc++] = i + 'a';
        }
    }
    // filling the rest of mustBeStr with newlines in case there was garbage
    // data in it beforehand.
    for (; mustBeStrInc < SIZE; mustBeStrInc++){
        mustBeStr[mustBeStrInc] = '\0';
    }

    strncpy(con -> must_be[index], mustBeStr, SIZE);
    
}

/* Add the letters from cur_word to the cannot_be field.
 * See the comments in constraints.h for how cannot_be is structured.
 */
void add_to_cannot_be(char *cur_word, struct constraints *con) {
    assert(strlen(cur_word) <= WORDLEN);

    for (int i = 0; i < WORDLEN; i++)
    {
        con -> cannot_be[cur_word[i] - 'a'] = 1;
    }
}

void print_constraints(struct constraints *c) {
    printf("cannot_be: ");

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (c -> cannot_be[i]){
            printf("%c ", i + 'a');
        }
    }
    
    
    printf("\nmust_be\n");

    for (int i = 0; i < WORDLEN; i++)
    {
        printf("[%d]", i);
        for (int n = 0; n < SIZE; n++)
        {
            if (c -> must_be[i][n] == '\0'){
                break;
            }
            printf(" %c", c -> must_be[i][n]);
        }
        printf("\n");
        
    }
    

    printf("\n");
}