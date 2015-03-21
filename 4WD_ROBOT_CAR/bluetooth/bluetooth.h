#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "mbed.h"

extern Serial BTmodule; // allow BTmodule to be manipulated by other files
extern Serial pc;

void BT_init(void);
char BT_read(void);

#endif