#ifndef JOY_DRIVER_H
#define JOY_DRIVER_H

struct joyinp{
    int number;
	int type;
	int value;
};

#define READVALUE _IOR('m', 'n', struct joyinp *)

#endif
