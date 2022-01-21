#include <stdio.h>

int main(int argc, char **argv) {

    char phone[11];
    scanf("%s", phone);


    /*
        I found the feof function on a stackoverflow question that I since been unable to find, however the question was related
        to how to tell when standard input closes.
        Here's a link on what feof does though; in essense, it checks if a specific datastream is currently at the the end-of-file, 
        and returns 1 or 0.

        stdin is standard input stream.

        https://www.tutorialspoint.com/c_standard_library/c_function_feof.htm
    */
    while(!feof(stdin)){
        int op;
        int succ = scanf("%d", &op);

        if (succ != 0){
            if (op == -1){
            printf("%s\n", phone);
            }
            else if (op >= 0  && op <= 9){
                printf("%c\n", phone[op]);
            }
            else{
                printf("ERROR");
            }
        }
        else{
            /* found on StackOverflow: https://stackoverflow.com/questions/11805414/why-does-scanf-not-wait-for-user-input-after-it-fails-one-time
        
            A behaviour with scanf seems to be that if you give it an invalid input (ie, scanf looking for int and you give it a string), it will "clog up" the input
            buffer. This matters more for the phone_loop as it sits in a loop and it seems that it will cause the loop to keep executing the failed command.

            Doing this in theory clears out the buffer by putting everything in the buffer into a temp array of chars/string, which only lasts for the if scope.
            */
            char junk[256];
            scanf("%s", junk);
            printf("ERROR");
        }
    }

    return 0;
}
