/*
 * www.Makerspace-Az.org
 * Sketch: 4X4 Robot car controller with Smartphone over Bluetooth
 * 
 * modified 12 December 2015
 * by Orkhan (azerimaker)
*/

#include <SoftwareSerial.h>

#define Rx 3          // Connect to Tx pin on BT module        
#define Tx 4          // Connect through Resitive divider to Rx pin of BT module 
#define motor1A 11    // INA
#define motor1B 10    // INB
#define motor2A 9     // INC
#define motor2B 6     // IND

// variables and constants
char BT_RECEIVED;  
volatile unsigned char SPEED_MIX = 0;	// reduced speed for turning
volatile unsigned char SET_SPEED = 100;	// Initial speed
boolean FORWARD = false;				// Motor rotation directions
boolean BACKWARD = true;
  
// function prototypes
char BT_read(void);						
void driveRobot(char COMMAND);							
void setSpeedRight(unsigned char SPEED, boolean DIR);
void setSpeedLeft(unsigned char SPEED, boolean DIR);
void brakeMotor(void);		

// define software serial port
SoftwareSerial BTserial(Rx, Tx);
             
                                   
void setup() 
{
  pinMode(Rx, INPUT);
  pinMode(Tx, OUTPUT);
  pinMode(motor1A, OUTPUT);
  pinMode(motor1B, OUTPUT);
  pinMode(motor2A, OUTPUT);
  pinMode(motor2B, OUTPUT);
  
  brakeMotor(); // set all pins to 0
  
  Serial.begin(9600);
  delay(50);
  Serial.println("Type AT commands!");
  Serial.println("Waiting for BT connection");

  BTserial.begin(9600); // SoftwareSerial baud rate for JY-MCU v1.03 defaults to 9600.
  delay(50);
} 


void loop(){

   if(BTserial.available()>0){

      	BT_RECEIVED = BT_read(); // read BT data byte
        
      	Serial.println(BT_RECEIVED);
        
      	driveRobot(BT_RECEIVED);	// send BT data to run Motors
    
       // Read user input if available.
	   if (Serial.available()){
	       delay(10);
	       BTserial.write(Serial.read());
	   }
    }
    delay(10);
}

// function to read BT data
char BT_read(void)
{
    char c = (char)BTserial.read();
    return c;
}

// Function to control motor speeds and directions
void driveRobot(char COMMAND)
{
  
  SPEED_MIX = map(SET_SPEED, 0, 255, 0, 127); // set different rotation speed for turning
  
  switch(COMMAND){
    case '0':  
        SET_SPEED = 0;
            break;
    case '1': 
        SET_SPEED = 25;
            break;
    case '2': 
        SET_SPEED = 50;
            break;
    case '3': 
        SET_SPEED = 75;
            break;
    case '4': 
        SET_SPEED = 100;
            break;
    case '5': 
        SET_SPEED = 125;
            break;
    case '6': 
        SET_SPEED = 150;
            break;
    case '7': 
        SET_SPEED = 175;
            break;
    case '8': 
        SET_SPEED = 200;
            break;
    case '9': 
        SET_SPEED = 225;
            break;
    case 'q': // Maks speed
        SET_SPEED = 255;
            break;
    case 'F': // Forward
        setSpeedRight(SET_SPEED, FORWARD);
        setSpeedLeft( SET_SPEED, FORWARD);
        break;
    case 'B': // Backward
        setSpeedRight(SET_SPEED, BACKWARD);
        setSpeedLeft( SET_SPEED, BACKWARD);
        break;
    case 'R': // Right
        setSpeedRight(SET_SPEED, BACKWARD);
        setSpeedLeft( SET_SPEED, FORWARD);
        break;
    case 'L': // Left
        setSpeedRight(SET_SPEED, FORWARD);
        setSpeedLeft( SET_SPEED, BACKWARD);
        break;
    case 'I': // Right + Forward
        setSpeedRight(SPEED_MIX, FORWARD);
        setSpeedLeft( SET_SPEED, FORWARD);
        break;
    case 'G': // Left + Forward
        setSpeedRight(SET_SPEED, FORWARD);
        setSpeedLeft( SPEED_MIX, FORWARD);
        break;
    case 'J': // Right + Backward
        setSpeedRight(SPEED_MIX, BACKWARD);
        setSpeedLeft( SET_SPEED, BACKWARD);
        break;  
    case 'H': // Left + Backward 
        setSpeedRight(SET_SPEED, BACKWARD);
        setSpeedLeft( SPEED_MIX, BACKWARD);
        break;  
    default: 
        brakeMotor();
        break;
    }
}

void setSpeedRight(unsigned char SPEED, boolean DIR)
{
  if(DIR == true){
      digitalWrite(motor2A, LOW);
      analogWrite( motor2B, SPEED); // PWM
      
  }else{
      analogWrite( motor2A, SPEED); // PWM
      digitalWrite(motor2B, LOW);
  }
}

void setSpeedLeft(unsigned char SPEED, boolean DIR)
{
  if(DIR == true){
      digitalWrite(motor1A, LOW);
      analogWrite( motor1B, SPEED);
  }else{
      analogWrite( motor1A, SPEED);
      digitalWrite(motor1B, LOW);
      
  }
}

// function to stop the car
void brakeMotor(void){
    digitalWrite(motor1A, LOW);
    digitalWrite(motor1B, LOW);
    digitalWrite(motor2A, LOW);
    digitalWrite(motor2B, LOW);
}



