# Driver For Using An Xbox Controller As An Alternative Mouse

**Prerequisites:**  jstest
```
sudo apt-get install jstest
```

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
