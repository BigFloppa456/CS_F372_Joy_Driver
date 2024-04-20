#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include "joy_driver.h"

/*
	Linux Loadable Kernel Module LKM to take data recieved from caller.c
	and use it to operate the mouse pointer.
*/

MODULE_LICENSE("GPL");
MODULE_AUTHOR("divith");
MODULE_DESCRIPTION("Driver to read controller inputs");

static int joy_open(struct inode *device_file, struct file *instance) {
	printk("open called!\n");
	return 0;
}

static int joy_close(struct inode *device_file, struct file *instance) {
	printk("close called!\n");
	return 0;
}

struct joyinp recieved ;

static long int joy_ioctl(struct file *file, unsigned cmd, unsigned long arg){
    printk("cmd: %d", cmd);
	switch(cmd){
		case READVALUE:
            if(copy_from_user(&recieved,(struct joyinp *) arg, sizeof(recieved))) 
				printk("Error copying data to user!\n");
			else
				// Mouse movement functionality here:
				// use recieved.number, recieved.type, recieved.value
			break;
        default:
            // Ignore init events.
            break;
	}
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = joy_open,
    .unlocked_ioctl = joy_ioctl,
	.release = joy_close
	
};

#define MYMAJOR 95
int joystick_init(void)
{
    int ret;
	printk("Initialize Driver!\n");
	/* register device number */
	ret = register_chrdev(MYMAJOR, "joy_driver", &fops);
	if(ret == 0) {
		printk("joy_driver - registered Device number Major: %d, Minor: %d\n", MYMAJOR, 0);
	}
	else if(ret > 0) {
		printk("joy_driver - registered Device number Major: %d, Minor: %d\n", ret>>20, ret&0xfffff);
	}
	else {
		printk("Could not register device number!\n");
		return -1;
	}
	return 0;
}

void joystick_end(void)
{
    unregister_chrdev(MYMAJOR, "joy_driver");
    printk("exit module\n");
}

module_init(joystick_init);
module_exit(joystick_end);