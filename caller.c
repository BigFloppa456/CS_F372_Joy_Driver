#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include "joy_driver.h"
#include <errno.h>
#include <string.h>

/* caller.c Opens a /dev/dummy file for performing LKM calls. It reads the 
    data stored in joyInputs.txt, converts it to appropriate type, 
    puts it in a struct and passes it to the kernel module*/
    
int main() {
    
    int dev = open("/dev/dummy", O_WRONLY);

    if (dev < 0)
        printf("open failed. Return code: %ld, meaning: %s\n", dev, strerror(errno));
    
    struct joyinp out ={0};

    struct joyinp joy = {0};
	while(true){
        FILE *fp = fopen("joyInputs.txt", "r");
        if(fp == NULL){
            perror("Error opening files \n");
        }
        
        char buffer[100];
        int i = 0;
        while (fscanf(fp, "%s", buffer) == 1) {
            if(i == 0){
                joy.number = atoi(buffer);
                i++;
            }
            else if(i == 1){
                joy.type = atoi(buffer);
                i++;
            }
            else if(i == 2){
                joy.value = atoi(buffer);
                i = 0;
            }
        }
        long ret = ioctl(dev, READVALUE, &joy);
        
        if (ret < 0){
        // printf("ioctl failed. Return code: %ld, meaning: %s\n", ret, strerror(errno));
        }
            
            
        
        fclose(fp);
        }
    close(dev);
	return 0;
    }

