#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wordle.h"
#include "constraints.h"

/* Read the wordle grid and solution from fp. 
 * Return a pointer to a wordle struct.
 * See sample files for the format. Assume the input file has the correct
 * format.  In other words, the word on each is the correct length, the 
 * words are lower-case letters, and the line ending is either '\n' (Linux,
 * Mac, WSL) or '\r\n' (Windows)
 */
struct wordle *create_wordle(FILE *fp) {
    struct wordle *w = malloc(sizeof(struct wordle));
    char line[MAXLINE]; 
    w->num_rows = 0;

    int i = 0;

    while(fgets(line, MAXLINE, fp ) != NULL) {
        // remove the newline character(s) 
        char *ptr;
        if(((ptr = strchr(line, '\r')) != NULL) ||
           ((ptr = strchr(line, '\n')) != NULL)) {
            *ptr = '\0';
        }
        
        strncpy(w->grid[w->num_rows], line, SIZE);
        w->grid[w->num_rows][SIZE - 1] = '\0';
        w->num_rows++;
        i++;
    }
    return w;
}


/* Create a solver_node and return it.
 * If con is not NULL, copy con into dynamically allocated space in the struct
 * If con is NULL set the new solver_node con field to NULL.
 * Tip: struct assignment makes copying con a one-line statements
 */
struct solver_node *create_solver_node(struct constraints *con, char *word) {

    struct solver_node *solverNode = malloc(sizeof(struct solver_node));

    if (con == NULL){
        solverNode -> con = NULL;
    }
    else{
        struct constraints constraintCpy = *con;
        solverNode -> con = &constraintCpy;
    }

    // needs to be done or bad times;
    solverNode -> next_sibling = NULL;
    solverNode -> child_list = NULL;

    strncpy(solverNode -> word, word, SIZE);

    return solverNode;
}

/* Checks if str contains c;
 * returns 1 if c is contained within str.
 * assumes str is a proper string (is null terminated)
 */
int contains(char *str, char c){
    int i = 0;
    while (str[i] != '\0'){
        if (str[i] == c){
            return 1;
        }
        i++;
    }
    return 0;
}


/* Return 1 if "word" matches the constraints in "con" for the wordle "w".
 * Return 0 if it does not match
 */
int match_constraints(char *word, struct constraints *con, 
struct wordle *w, int row) {

    if (verbose && !strncmp("annex", word, WORDLEN)){
        printf("test\n");
    }
    int seenLetters[26];
    for (int i = 0; i < 26; i++)
    {
        seenLetters[i] = 0;
    }

    char* wordleGuessResult = w -> grid[row];
    for (int i = 0; i < WORDLEN && word[i] != '\0'; i++){
        if (con -> must_be[i][0] != '\0'){
            if (!contains(con -> must_be[i], word[i])){
                return 0;
            }
        }
        else{
            if (con -> cannot_be[word[i] - 'a']){
                return 0;
            }
        }
        // checks if the guess result is 'g' or 'y', check if the character in the guess
        // has been seen before.
        char g = wordleGuessResult[i];
        switch (g) {
        case 'y':
            // if character is yellow, check that the solution word does not have
            // that character at that spot.
            if (w -> grid[0][i] == word[i]){
                return 0;
            }
        case 'g':
            if (seenLetters[word[i] - 'a']++){
                return 0;
            }
            break;
        default:
            // if character is grey, check that solution word does not include that
            // character
            if (contains(w -> grid[0], word[i]) || seenLetters[word[i] - 'a']){
                return 0;
            }
        }
    }
    return 1;
}

/* remove "letter" from "word"
 * "word" remains the same if "letter" is not in "word"
 */
void remove_char(char *word, char letter) {
    char *ptr = strchr(word, letter);
    if(ptr != NULL) {
        *ptr = word[strlen(word) - 1];
        word[strlen(word) - 1] = '\0';
    }
}

/* Build a tree starting at "row" in the wordle "w". 
 * Use the "parent" constraints to set up the constraints for this node
 * of the tree
 * For each word in "dict", 
 *    - if a word matches the constraints, then 
 *        - create a copy of the constraints for the child node and update
 *          the constraints with the new information.
 *        - add the word to the child_list of the current solver node
 *        - call solve_subtree on newly created subtree
 */

void solve_subtree(int row, struct wordle *w,  struct node *dict, 
                   struct solver_node *parent) {
    if(verbose) {
        printf("Running solve_subtree: %d, %s\n", row, parent->word);
    }

    if (row == w->num_rows){
        //recursion base case, no more rows to process.
        return;
    }
    else{
        // expected process:
        /*  1: Create Constraints for the current word (defined in solver_node)
         *  2: Using those constraints, use match constraints to find words in dict that 
         *     matches those constraints.
         *  3: Use the word to create a new solver node. 
         *  4: Solve the new solver node (recursion);
        */  
        // 1
        // First create/update constraints by using the word in parent node.
        if (!strncmp("dunce", parent -> word, SIZE)){
            if (verbose) printf("reached dunce -> amino");
        }

        struct constraints *constraint = parent -> con;
        char* word = parent -> word;
        char* currentRowGuess = w -> grid[row];
        char cannotBeCStr[6];
        int cbcsIndex = 0;
        for (int i = 0; i < WORDLEN; i++){
            switch (currentRowGuess[i]){
                case 'g':
                    set_green(word[i], i, constraint);
                    break;
                case '-':
                    cannotBeCStr[cbcsIndex++] = word[i];
                    break;
                case 'y':
                    // if the row is 1, then the next tile from w will be the solution rather
                    // than the next tile, therefore if we are at row 1, we need
                    // to explicitly give it a tile that referrs to the solution.
                    if (row == 1){
                        set_yellow(i, currentRowGuess, "ggggg", word, constraint);
                    }
                    else{
                        set_yellow(i, currentRowGuess, w -> grid[row - 1], word, constraint);
                    }
                default:
                    break;
            }
        }   
        cannotBeCStr[cbcsIndex] = '\0';
        add_to_cannot_be(cannotBeCStr, constraint);

        // 2
        // Now that the constraint has been built, loop through the dictionary to find words 
        // that match

        struct node *currWordNode = dict;
        struct solver_node *currentChild = parent -> child_list;

        while (currWordNode != NULL){
            // debug stuff, If this is still here please ignore
            if (!strncmp("amino", currWordNode -> word, SIZE)){
                if (!strncmp("dunce", parent -> word, SIZE)){
                    if (verbose) printf("reached dunce -> amino");
                }
            }

            // if this while condition gives segmentation faults, it might be because the next
            // pointer has not been NULL Initialized (assigned to NULL if not being used);

            if (match_constraints(currWordNode -> word, constraint, w, row)){
                // 3
                // Word found, create a child node;

                struct solver_node *childNode = malloc(sizeof(struct solver_node));
                struct constraints *constraintCpy = malloc(sizeof(struct constraints));
                *constraintCpy = *constraint; 

                for (int n = 0; n < WORDLEN; n++)
                {
                    strncpy(constraintCpy -> must_be[n], "", SIZE);
                }

                // this can be a source of bugs,
                // in theory the compiler will automatically malloc
                // the space needed.

                childNode -> con = constraintCpy;
                strncpy(childNode -> word, currWordNode -> word, SIZE); 
                // Note: word in the provided solver_node is a hardcoded 6, this might cause 
                // a buffer overflow if SIZE > 6

                childNode -> next_sibling = NULL; //null initialize the pointer;
                childNode -> child_list = NULL;

                if (currentChild == NULL){
                    if (parent -> child_list == NULL){
                        parent -> child_list = childNode;
                    }
                    currentChild = childNode;
                }
                else{
                    currentChild -> next_sibling = childNode;
                    currentChild = currentChild -> next_sibling;
                }

                solve_subtree(row + 1, w, dict, childNode);
            }
            // advance currWordNode
            currWordNode = currWordNode -> next;
        }
    }
}

/* Print to standard output all paths that are num_rows in length.
 * - node is the current node for processing
 * - path is used to hold the words on the path while traversing the tree.
 * - level is the current length of the path so far.
 * - num_rows is the full length of the paths to print
 */

void print_paths(struct solver_node *node, char **path, 
                 int level, int num_rows) {
    // base - node is valid.
    // base case 1: level == num_rows, print the path;

    if (level == num_rows){
        path[level - 1] = node -> word;
        int i = 0;
        printf("%s ", path[i++]);
        for (; i < num_rows; i++)
        {
            printf("%s ", path[i]);
        }
        printf("\n");
    }
    else{
        // if the next child node is NULL, no point continuing. 
        if (node -> child_list != NULL){
            
            path[level - 1] = node -> word;
            
            struct solver_node *currNode = node -> child_list;
            // since the next child list is not null.
            while (currNode != NULL){
                print_paths(currNode, path, level + 1, num_rows);
                currNode = currNode -> next_sibling;
            }
        }
    }
}

/* Free all dynamically allocated memory pointed to from w.
 */ 
void free_wordle(struct wordle *w){
    // as far as I can tell, nothing in the wordle struct is dynamically allocated.
    free(w);
}

/* Free all dynamically allocated pointed to from node
 */
void free_tree(struct solver_node *node){
    // recursive implementation, base case: node == NULL;

    if (node == NULL){
        return;
    }
    else{
        free_constraints(node -> con);
        free_tree(node -> next_sibling);
        free_tree(node -> child_list);
        free(node);
    }
}
