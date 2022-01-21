#include <stdio.h>
#include <stdlib.h>

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
    else if (succ == 2){
        printf("ERROR\n");
    }

    return 0;
}
