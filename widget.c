#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <string.h>
#include <stdint.h>

#include "days.c"


int date_and_time[3];           // this arry stors min, hours, day of a week


void draw_day_bitmap(uint8_t* fbmem, int x, int y,
    struct fb_var_screeninfo vinfo,
    struct fb_fix_screeninfo finfo,
    uint32_t color,
    uint8_t day_bitmap[3][8],
    int scale) {
    for (int ch = 0; ch < 3; ch++) {
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                if ((day_bitmap[ch][row] >> (7 - col)) & 1) {
                    for (int dy = 0; dy < scale; dy++) {
                        for (int dx = 0; dx < scale; dx++) {
                            int px = x + ch * (8 * scale + scale) + col * scale + dx;
                            int py = y + row * scale + dy;
                            long location = py * finfo.line_length + px * (vinfo.bits_per_pixel / 8);
                            *((uint32_t*)(fbmem + location)) = color;
                        }
                    }
                }
            }
        }
    }
}


void get_current_time(){

    time_t t = time(NULL);
    struct tm date = *localtime(&t);        // get current time

    date_and_time[0] = date.tm_min;         // into variable min save value from tm_min
    date_and_time[1] = date.tm_hour;        // into variable hour save value from tm_hout
    int day = date.tm_mday;                 // etc
    int mon = date.tm_mon + 1;
    int year = date.tm_year - 100;          // variable day of a week stors 0 number
    int day_of_week = 0;                    // the day of a week is calculated bellow
    int sum = 0;                            // sum is a tmp variable which helps calculate day of a week

    sum += day + 6;                         // to sum add day and const 6. 6 is a code for year 2000 - 2099
    switch(mon){                            // it means in 3000 this function will calculate wrong result
        case 1:                             // add to sum a code of a month
            sum += 0;
        break;
        case 2:
            sum += 3;
        break;
        case 3:
            sum += 3;
        break;
        case 4:
            sum += 6;
        break;
        case 5:
            sum += 1;
        break;
        case 6:
            sum += 4;
        break;
        case 7:
            sum += 6;
        break;
        case 8:
            sum += 2;
        break;
        case 9:
            sum += 5;
        break;
        case 10:
            sum += 0;
        break;
        case 11:
            sum += 3;
        break;
        case 12:
            sum += 5;
        break;
    }

    sum += year + year / 4;
    day_of_week = sum % 7;
    date_and_time[2] = day_of_week;

}



int create_connection(){

    int fb = open("/dev/fb0", O_RDWR);  // use open syscall to open fb0 device in path /dev
    if(fb < 0){                         // open it for read and write
        perror("ERROR");                // if the fb is less then 0 print an ERROR message
        return errno;
    }

    get_current_time();                 // this function gets current time and date from OS and calculate day of a week

    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;

    ioctl(fb, FBIOGET_VSCREENINFO, &vinfo);
    ioctl(fb, FBIOGET_FSCREENINFO, &finfo);

    long screensize = vinfo.yres * finfo.line_length;

    uint8_t* fbmem = mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);
    if ((intptr_t)fbmem == -1) {
        perror("mmap failed");
        return errno;
    }

    uint32_t green = 0x0000FF00;            // green color
    // function that set pixel color *((uint32_t*)(number)) = green;
	int x = 1100, y = 50;
    switch (date_and_time[2]) {
        case 1:
            draw_day_bitmap(fbmem, x, y, vinfo, finfo, green, days_bitmap[0], 3);
        break;
        case 2:
            draw_day_bitmap(fbmem, x, y, vinfo, finfo, green, days_bitmap[1], 3);
        break;
        case 3:
            draw_day_bitmap(fbmem, x, y, vinfo, finfo, green, days_bitmap[2], 3);
        break;
        case 4:
            draw_day_bitmap(fbmem, x, y, vinfo, finfo, green, days_bitmap[3], 3);
        break;
        case 5:
            draw_day_bitmap(fbmem, x, y, vinfo, finfo, green, days_bitmap[4], 3);
        break;
        case 6:
            draw_day_bitmap(fbmem, x, y, vinfo, finfo, green, days_bitmap[5], 3);
        break;
        case 7:
            draw_day_bitmap(fbmem, x, y, vinfo, finfo, green, days_bitmap[6], 3);
        break;
    }


    munmap(fbmem, screensize);
    close(fb);          // close device 

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
