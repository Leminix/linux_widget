#include <stdlib.h>
#include <stdio.h>


void create_connection(){
    printf("hello world\n");
}


int main(void){

    #ifdef __linux__            //check OS. This will call create_connection function only when the user OS is linux
        create_connection();    // in other case print a message which says to user this code can not run on your OS
    #else
        printf("Sorry but this code can not be execute on your OS\n");
    #endif

    return 0;
}