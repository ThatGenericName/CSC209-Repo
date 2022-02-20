#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wordlist.h"


/* Read the words from a filename and return a linked list of the words.
 *   - The newline character at the end of the line must be removed from
 *     the word stored in the node.
 *   - You an assume you are working with Linux line endings ("\n").  You are
 *     welcome to also handle Window line endings ("\r\n"), but you are not
 *     required to, and we will test your code on files with Linux line endings.
 *   - The time complexity of adding one word to this list must be O(1)
 *     which means the linked list will have the words in reverse order
 *     compared to the order of the words in the file.
 *   - Do proper error checking of fopen, fclose, fgets
 */
struct node *read_list(char *filename) {

    FILE *wordFile = fopen(filename, "r");

    if (wordFile == NULL){
        return NULL;
    }

    char stringTemp[6];
    char* temp = fgets(stringTemp, 6, wordFile);
    if (temp == NULL){
        return NULL;
    }

    struct node *head = malloc(sizeof(struct node));

    strncpy(head -> word, stringTemp, 6);
    // assuming that removing the newline char, we are replacing it with the null terminator
    head -> word[5] = '\0';
    
    char _c = fgetc(wordFile);

    if (_c != '\n'){
        ungetc(_c, wordFile);
    }

    while (fgets(stringTemp, 6, wordFile) != NULL){
        struct node *newNode = malloc(sizeof(struct node));
        newNode -> next = head;
        strncpy(newNode -> word, stringTemp, 6);
        newNode -> word[5] = '\0'; // assuming that removing the newline char, we are replacing it with the null terminator
        head = newNode;
        _c = fgetc(wordFile);
        if (_c != '\n'){
            ungetc(_c, wordFile);
        }
    }

    int closeCheck = fclose(wordFile);

    // not sure why we need to errorcheck fclose, we will be returning either way.
    if (closeCheck == EOF){
        return head;
    }
    return head;
}

/* Print the words in the linked-list list one per line
 */
void print_dictionary(struct node *list) {
    struct node* current = list;
    while (current != NULL){
        printf("%s\n", current -> word);
        current = current -> next;
    }
}
/* Free all of the dynamically allocated memory in the dictionary list 
 */ 
void free_dictionary(struct node *list) {
    struct node* current = list;
    struct node* next;
    while (current != NULL){
        next = current -> next;
        free(current);
        current = next;
    }
}

