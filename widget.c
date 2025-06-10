#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>



int create_connection(){
    
    int fb = open("/dev/fb0", O_RDWR);  // use open syscall to open fb0 device in path /dev
    if(fb < 0){                         // open it for read and write
        perror("ERROR");                // if the fb is less then 0 print an ERROR message
        return errno;
    }


    // write a code which will get time and date from OS
    // write a code which will call function to convert data to binary code


    if(close(fb) < 0){          // close device and check returned value
        perror("ERROR");        // if a value is less then 0 print an ERROR message
        return errno;           // but if the value is 0 return 0 to main function
    }

    return 0;

}



int main(void){

    #ifdef __linux__            //check OS. This will call create_connection function only when the user OS is linux
        create_connection();    // in other case print a message which says to user this code can not run on your OS
    #else
        printf("Sorry but this code can not be execute on your OS\n");
    #endif

    return 0;
}