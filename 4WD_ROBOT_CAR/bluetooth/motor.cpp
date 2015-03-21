#include "motor.h"
#include "bluetooth.h"


PwmOut MOTOR_1A(p21);
PwmOut MOTOR_1B(p22);
PwmOut MOTOR_2A(p23);
PwmOut MOTOR_2B(p24);

BusOut myleds(LED1, LED2, LED3, LED4);

void initMotor()
{
    MOTOR_1A.period(0.020f); // set PWM freq for all motors, default 20 msec
    MOTOR_1B.period(0.020f); 
    MOTOR_2A.period(0.020f);  
    MOTOR_2B.period(0.020f); 
    Blink(2);
}

float SetSpeed = 0.3f; // Initial speed

void setMotor(char BT_rcv_char)
{
    float SpeedMix = 0.2f; // Subtracted peed for side movement
    
    /*** Speed settings ***/
    switch(BT_rcv_char)
    {
        case '0': SetSpeed = 0.0f;
                break;
        case '1': SetSpeed = 0.1f;
                break;
        case '2': SetSpeed = 0.2f;
                break;
        case '3': SetSpeed = 0.3f;
                break;
        case '4': SetSpeed = 0.4f;
                break;
        case '5': SetSpeed = 0.5f;
                break;
        case '6': SetSpeed = 0.6f;
                break;
        case '7': SetSpeed = 0.7f;
                break;
        case '8': SetSpeed = 0.8f;
                break;
        case '9': SetSpeed = 0.9f;
                break;
        case 'q': SetSpeed = 1.0f;
                break;
    }
    
    SpeedLEDs(SetSpeed); // Send current Speed value for visual feedback
    
    /*** Heading settings ***/
    if(BT_rcv_char == 'F')
    {
        setRight(true,SetSpeed);
        setLeft(true,SetSpeed);
    }
    else if(BT_rcv_char == 'B')
    {
        setRight(false,SetSpeed);
        setLeft(false,SetSpeed);
    }
    else if(BT_rcv_char == 'R')
    {
        setRight(false,SetSpeed);
        setLeft(true,SetSpeed);
    }
    else if(BT_rcv_char == 'L')
    {
        setRight(true,SetSpeed);
        setLeft(false,SetSpeed);
    }
    else if(BT_rcv_char == 'I')
    {
        setRight(true,SetSpeed-SpeedMix);   // Right + Forward
        setLeft(true,SetSpeed);
    }
    else if(BT_rcv_char == 'G')
    {
        setRight(true,SetSpeed);        // Left + Forward
        setLeft(true,SetSpeed-SpeedMix);
    }
    else if(BT_rcv_char == 'J')
    {
        setRight(false,SetSpeed-SpeedMix);        // Right + Reverse
        setLeft(false,SetSpeed);
    }
    else if(BT_rcv_char == 'H')
    {
        setRight(false,SetSpeed);        // Left + Reverse 
        setLeft(false,SetSpeed-SpeedMix);
    }
    else
    {
        freeRunMotor();
    }
}


void setLeft(bool DIR, float SPEED)
{
    // Make sure SPEED value is Limited to 0.0-1.0
    if(SPEED <0.0f)
    { 
        SPEED = 0.0f; 
    }
    else if(SPEED > 1.0f)
    { 
        SPEED = 1.0f; 
    }
    
    if(DIR == true) // Set duty cycle using SPEED value btw 0 and 100%
    {
        MOTOR_1A.write(0.0f);  // 0% duty cycle
        MOTOR_1B.write(SPEED);
    }else
    {

        MOTOR_1A.write(SPEED); 
        MOTOR_1B.write(0.0f);
    }
}

void setRight(bool DIR, float SPEED)
{
    // Make sure SPEED value is Limited to 0.0-1.0
    if(SPEED <0.0f)
    { 
        SPEED = 0.0f; 
    }
    else if(SPEED > 1.0f)
    { 
        SPEED = 1.0f; 
    }
    
    if(DIR == true) // Set duty cycle using SPEED value btw 0 and 100%
    {
        MOTOR_2A.write(SPEED); 
        MOTOR_2B.write(0.0f);
    }else
    {
        
        MOTOR_2A.write(0.0f);  // 0% duty cycle
        MOTOR_2B.write(SPEED);
    }
}


void brakekMotor()
{
    MOTOR_1A.write(0.0f);  
    MOTOR_1B.write(0.0f);
    MOTOR_2A.write(0.0f);  
    MOTOR_2B.write(0.0f);  
}

void freeRunMotor()
{
    MOTOR_1A.write(0.0f);  
    MOTOR_1B.write(0.0f);
    MOTOR_2A.write(0.0f);  
    MOTOR_2B.write(0.0f); 
}

void Blink(int times)
{
    for(int i = 0; i < times; i++)
    {
        myleds = 0xF;
        wait(0.2);
        myleds = 0x0;
        wait(0.2);
    }
}

// Later improve it with BusOut
void SpeedLEDs(float SPEED) 
{    
    if(SPEED >0.0f && SPEED <0.3f) 
    {
        myleds = 1;
    }
    else if(SPEED >=0.3f && SPEED <0.5f) 
    {
        myleds = 3;
    }
    else if(SPEED >=0.5f && SPEED <=0.7f) 
    {
        myleds = 7;
    }
    else if(SPEED >0.7f && SPEED <=1.0f) 
    {
        myleds = 15;
    }else
    {
        myleds = 0;
    }
}