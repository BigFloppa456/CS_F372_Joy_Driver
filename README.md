# Driver For Using An Xbox Controller As An Alternative Mouse

**Prerequisites:**  joystick
```
sudo apt-get install joystick
```

Use jstest to find the joystick device file in /dev/input. Can be done by
```
jstest /dev/input/jsX
```
where X is the number of input joysticks. Find the appropriate one, and make changes in reader.c accordingly

**To setup the LKM:**
In the directory of the files, provide superuser privileges.
```
sudo su
```
and enter your password. Then:
```
make
```
```
gcc -o reader reader.c
```
```
gcc -o caller caller.c
```

Create a dummy character device file /dev/dummy for our Kernel-related operations. 
```
sudo mknod /dev/dummy c 95 0
```

**To install the LKM:**
In the same directory as the files
```
sudo insmod joy_driver.ko
```

This will install the LKM onto the kernel. To verify if the LKM has initialized properly, use: ```dmesg | tail -n 2```
This should give an output similar to:
```
[5958.146652] Initialize Driver!
[5958.146669] joy_driver registered Device number Major: 95, M
inor: 0
```

**To run the application files:**
Call in the following order to avoid errors
```
./reader
```
```
./caller
```

**To unload LKM:**
```
sudo rmmod joy_driver.ko
```

**To clean files made during .ko generation:**
```
make clean
```
___________________________________________________________________
## Documentation

### Things to note
Controller inputs will be read as js_event defined in the linux/joystick.h library.
js_event is defined as:
```
struct js_event{
    _u32 time;  // event timestamp
    _s16 value;
    _u8 type;   // type of event
    _u8 number; // axis/button number
};
```
Event types are defined as follows:
```
#define JS_EVENT_BUTTON 0x01 // button pressed or released
#define JS_EVENT_AXIS 0x02 // analog stick
#define JS_EVENT_INIT 0x80 // initial state of device
```

struct mousek_device uses a USB Request block to get USB data and takes controller input to push out input data and the variables x and y are used to keep track of the position of the controller input device 
code block
```
struct mousek_device {
    signed char data[4];     /* use a 4-byte protocol */
    struct urb urb;          /* USB Request block, to get USB data*/
    struct input_dev *idev;   / pointer for input device, to push out input  data */
    int x, y;                /* keep track of the position of this device */
};
```

#### joy_driver.h 
Contains definitions for a _IOR macro and a struct joyinp used to transfer data from caller.c to the LKM defined in joy_driver.c
```
struct joyinp{
  int number;  // Button or Axis number
	int type;    // Type of event
	int value;   // Value of data attributed to the button/axis
};
```

```
#define READVALUE _IOR('m', 'n', struct joyinp *)
```


#### reader.c
Uses the functionality from linux/joystick.h to read data from 
joystick input, convert it to a string and write it into a file,
from where caller.c will be able to read it.

#### caller.c
Opens a /dev/dummy file for performing LKM calls. It reads the 
data stored in joyInputs.txt, converts it to appropriate type, 
puts it in a struct and passes it to the kernel module defined in joy_driver.c

#### joy_driver.h
Linux Loadable Kernel Module LKM to take data recieved from caller.c
	and use it to operate the mouse pointer.

 ###### joy_open: 
  Called when caller.c opens an instance of /dev/dummy using 'open()' from ioctl.h. Prints a message to the kernel to indicate it has been called.
  ###### joy_close: 
  Called when caller.c closes an instance of a device file using 'close()' from ioctl.h. Prints a message to the kernel to indicate it has been called.
  ###### joy_ioctl: 
  Called when caller.c calls 'ioctl()' after reading the values and passing it into the appropriate struct joyinp format. The macro defined has a magic number attributed that will allow this method to perform appropriate actions using the switch case ladder.
###### joystick_init:
Initialization function for the LKM. When loaded onto the kernel using ```insmod``` print the following to the kernel (on success):
```
[5958.146652] Initialize Driver!
[5958.146669] joy_driver registered Device number Major: 95, M
inor: 0
```
Ensure that that the Major and Minor numbers being registered for the device file in this init method match with those of the /dev/dummy file that is being created.
###### joystick_end:
Exit function for the LKM. When unloaded from the kernel using ```rmmod``` will print the following to the kernel: 
```
 [ 1722.550794] exit module
```
______________________________________
# REFERENCES
### Johannes 4GNU_Linux
References taken from the following directories on the author's github:
1. 01_simple_LKM
2. 02_dev_nr
3. 13_ioctl

https://github.com/Johannes4Linux/Linux_Driver_Tutorial.git   

Youtube Tutorials Referred: https://youtube.com/playlist?list=PLCGpd0Do5-I3b5TtyqeF1UdyD4C-S-dMa&si=v_P7eN6y7r_YzAzn   

### Group Tetris - Simon Fraser University
Group Tetris. (n.d.). How to use the linux/joystick.h library. How To Use The linux/joystick.h Library. https://opencoursehub.cs.sfu.ca/bfraser/grav-cms/cmpt433/links/files/2022-student-howtos/LinuxJoystick.hLibrary.pdf  

### Linux Kernel Archives - Ragnar Hojland Espinosa
Espinosa, R. H. (1998, August 7). Joystick API Documentation . The Linux Kernel Archives. https://www.kernel.org/doc/Documentation/input/joystick-api.txt
____________________________________________________
## Contribution
1. Divith Mahajan - 2020B3A31513G - Basic LKM structure Code, caller.c file, Documentation regarding the LKM init, exit, open, close methods, File operations idea to transfer data from reader to caller to LKM (40%).
2. Akshat Saboo - 2020B3A80709G  - Attempted integration with X11 for mouse control and documentation regarding setup, initalize, running and unloading (10%).
3. Divyam Srivastava - 2020B2A82103G - Documentation of Structs and _IOR macro used throughout the project, Reader file to read controller input (15%).
4. Bhamare Yash Prashant - 2020B2A80760G - Reader file to read controller input (5%)
5. Harshith Harithsa R - 2020B2A42007G - Implementation of Left Mouse Button (LMB) mapped to LB button of controller in LKM (30%)

