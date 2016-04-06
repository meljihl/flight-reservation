#include "rest_helper.h"
#include <stdio.h>

int main(void) {


    long status;
    char buffer[1000], buffer2[100];
    rest_call("localhost:8888/countries", "GET", "", &status, buffer);
    int index;


    int i, j = 0;
    for (i = 0; i <= strlen(buffer); i++) {

        if (buffer[i] == ';' || i ==strlen(buffer)) {

            memset(buffer2, '\0', sizeof(buffer2));
            strncpy(buffer2, buffer+j,  i - j);
            printf("%s\n", buffer2);
            j = ++i;


        }
    }


    return 0;
}