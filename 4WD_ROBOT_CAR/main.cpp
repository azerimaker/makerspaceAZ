#include "mbed.h"
#include "bluetooth.h"
#include "motor.h"
//#include "distance.h"



int main(){
    
    
    BT_init();
    initMotor();
    
    char BT_recv_char;
    
    while(1)
    {
        if(BTmodule.readable())
        {
            BT_recv_char = BT_read();
            setMotor(BT_recv_char);
            pc.printf("%c \n\r",BT_recv_char);
        }
    
    }
}
