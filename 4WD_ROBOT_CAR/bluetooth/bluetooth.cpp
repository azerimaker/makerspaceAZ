#include "bluetooth.h"

Serial BTmodule(p13,p14); // TX, RX
Serial pc(USBTX,USBRX);

void BT_init(void)
{
    BTmodule.baud(9600);   
    pc.baud(9600);
    pc.printf("Serial com init Bluetooth-PC...\n\r");
}

char BT_read(void)
{
    char c = BTmodule.getc();
    return c;  
}