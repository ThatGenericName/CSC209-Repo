#include <stdio.h>

int main(int argc, char **argv) {

    char phone[11];
    scanf("%s", phone);

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
                printf("ERROR\n");
            }
        }
        else{
            char junk[256];
            scanf("%s", junk);
            printf("ERROR\n");
        }
    }

    return 0;
}
