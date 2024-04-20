#ifndef JOY_DRIVER_H
#define JOY_DRIVER_H

// #include <linux/joystick.h>

/*  Controller inputs will be read as js_event */
    // this is defined in linux/joystick.h
// struct js_event{
//     _u32 time;  // event timestamp
//     _s16 value;
//     _u8 type;   // type of event
//     _u8 number; // axis/button number
// };

//     // this is defined in linux/joystick.h
// #define JS_EVENT_BUTTON 0x01 // button pressed or released
// #define JS_EVENT_AXIS 0x02 // analog stick
// #define JS_EVENT_INIT 0x80 // initial state of device

struct joyinp{
    int number;
	int type;
	int value;
};

#define READVALUE _IOR('m', 'n', struct joyinp *)

#endif
