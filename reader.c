

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>
#include <stdint.h>
#include "joy_driver.h"
#include <errno.h>

/*
    reader.c uses the functionality from linux/joystick.h to read data from 
    joystick input, convert it to a string and write it into a file,
    from where caller.c will be able to read it.
*/

int main() {
	char* deviceRoute = "/dev/input/js2";
    int js = open(deviceRoute, O_RDONLY);
	if (js == -1)
	{
		perror("Couldn't open joystick\n");
        return -1;
	}

    // Infinite loop to read inputs and write them to joyInputs.txt
    while(true){
        usleep(10000);

        struct js_event event;

        int bytesRead = read(js, &event, sizeof(event));
        if (bytesRead <= 0) return -1;

        if(event.type == JS_EVENT_INIT){
            printf("Initialized. Event type: %d\n", event.type);
        }
        if (event.type == JS_EVENT_BUTTON ) {
			printf("Button %d's value is: %d\n", event.number, event.value);
            FILE *fp = fopen("joyInputs.txt", "w+");
            if(fp == NULL){
                perror("Error opening files \n");
            }

            char str1[10], str2[10],str3[10]; 
            sprintf(str1,"%d", event.number);
            sprintf(str2,"%d",event.value);
            sprintf(str3,"%d",event.type);
            fprintf(fp, "%s %s %s", str1, str2, str3);
            fclose(fp);
        }
		if (event.type == JS_EVENT_AXIS ) {
			printf("Axis %d's value is: %d\n", event.number, event.value);
            FILE *fp = fopen("joyInputs.txt", "w+");
            if(fp == NULL){
                perror("Error opening files \n");
            }

            char str1[10], str2[10],str3[10]; 
            sprintf(str1,"%d", event.number);
            sprintf(str2,"%d",event.value);
            sprintf(str3,"%d",event.type);
            fprintf(fp, "%s %s %s", str1, str2, str3);
            fclose(fp);
		}
        
    }
    close(js);
    printf("---  |\nend  |\n---  |\n");

	return 0;
    }

