#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/usb/input.h>
#include <linux/hid.h>

/* usb stuff */
#include <linux/input.h>
#include <linux/usb.h>

/* miscdevice stuff */
#include <linux/poll.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include "joy_driver.h"

/*
	Linux Loadable Kernel Module LKM to take data recieved from caller.c
	and use it to operate the mouse pointer.
*/

#define DEVICE_NAME "mousek"
#define MAX_SCREEN 100
#define MIN_SCREEN 0

struct mousek_device {
    signed char data[4];     /* use a 4-byte protocol */
    struct urb urb;          /* USB Request block, to get USB data*/
    struct input_dev *idev;   /* input device, to push out input  data */
    int x, y;                /* keep track of the position of this device */
};

static struct mousek_device *mouse;

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Group-9");
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
			else{
				// Mouse movement functionality here:
				// use recieved.number, recieved.type, recieved.value
				printk("%d\n", recieved.value);
				struct input_dev *dev = mouse->idev;
				input_report_key(dev, BTN_LEFT, 1);
				//left click up
				input_report_key(dev, BTN_LEFT, 0);
				input_sync(dev);}

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
	struct input_dev *input_dev;
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
	mouse = kmalloc(sizeof(struct mousek_device), GFP_KERNEL);
    if (!mouse) return -ENOMEM; /* failure */
    memset(mouse, 0, sizeof(*mouse));

	input_dev = input_allocate_device();
	if (!input_dev) {
		printk(KERN_ERR "mousek.c: Not enough memory\n");
		ret = -ENOMEM;
		//goto err_free_irq;
	}
	//updating struct
	mouse->idev = input_dev;
    
	/* tell the features of this input device: fake only keys */
    //mouse->idev.evbit[0] = BIT(EV_KEY);
    /* and tell which keys: only the arrows */
    //set_bit(103, mouse->idev.keybit); /* Up    */
    //set_bit(105, mouse->idev.keybit); /* Left  */
    //set_bit(106, mouse->idev.keybit); /* Right */
    //set_bit(108, mouse->idev.keybit); /* Down  */
	
	input_dev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_REL);
	//input_dev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS);
    
	//set_bit(103, input_dev->keybit); /* Up    */

	input_dev->keybit[BIT_WORD(BTN_MOUSE)] = BIT_MASK(BTN_LEFT) | BIT_MASK(BTN_RIGHT) | BIT_MASK(BTN_MIDDLE);
	input_dev->relbit[0] = BIT_MASK(REL_X) | BIT_MASK(REL_Y) | BIT_MASK(REL_WHEEL);
	//input_dev->absbit[0] = BIT_MASK(ABS_X) | BIT_MASK(ABS_Y);
	
	//input_set_abs_params(input_dev, ABS_X, MIN_SCREEN, MAX_SCREEN, 0, 0);
	//input_set_abs_params(input_dev, ABS_Y, MIN_SCREEN, MAX_SCREEN, 0, 0);

	
	input_dev->name = DEVICE_NAME;	
	input_set_drvdata(input_dev, mouse);
	
	ret = input_register_device(input_dev);
	if (ret) {
		printk(KERN_ERR "mousek: Failed to register device\n");
		goto err_free_dev;
	}
	return 0;
err_free_dev:
	input_free_device(mouse->idev);
	kfree(mouse);
	return ret;
}

void joystick_end(void)
{
    unregister_chrdev(MYMAJOR, "joy_driver");
	input_unregister_device(mouse->idev);
	kfree(mouse);
    printk("exit module\n");
}

module_init(joystick_init);
module_exit(joystick_end);