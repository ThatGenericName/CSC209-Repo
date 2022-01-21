#include <stdio.h>

int main(int argc, char **argv) {

    char phone[11];
    int op;
    int succ;
    succ = scanf("%s %d", phone, &op);

    if (op == -1){
        printf("%s\n", phone);
    }
    else if (op >= 0  && op <= 9){
        printf("%c\n", phone[op]);
    }
    else {
        /* found on StackOverflow: https://stackoverflow.com/questions/11805414/why-does-scanf-not-wait-for-user-input-after-it-fails-one-time
        
        A behaviour with scanf seems to be that if you give it an invalid input (ie, scanf looking for int and you give it a string), it will "clog up" the input
        buffer. This matters more for the phone_loop as it sits in a loop and it seems that it will cause the loop to keep executing the failed command.

        Doing this in theory clears out the buffer by putting everything in the buffer into a temp array of chars/string, which only lasts for the if scope.
        */
        if (succ != 2){
            char junk[256];
            scanf("%s", junk);
        }
        printf("ERROR\n");
    }

    return 0;
}
