/*
 * www.Makerspace-Az.org
 * Arduino Sketch: 4X4 Robot mashin kontroller
 * Kommunikasiya: Bluetooth (BT)
 * Android App: Bluetooth RC
 * last modified: 24 December 2015
 * by (azerimaker)
*/

#include <SoftwareSerial.h>

#define Rx 2          // BT modulun TX pinine qoshulur     
#define Tx 3          // Gerginlik bolucu rezistorlarla BT modulun RX pinine qoshulur

#define motor1A 11    // INA
#define motor1B 10    // INB
#define motor2A 9     // INC
#define motor2B 6     // IND

// deyishenlar ve sabitler
char BT_RECEIVED;   // BT-dan gelen simvol
char PC_RECEIVED;   // PC-den gelen simvol
 
unsigned char SPEED_MIX = 0;
unsigned char SET_SPEED = 100; // bashlangic suret 100 

boolean FORWARD = false; // ireli
boolean BACKWARD = true; // geri
  
// Hardware serial PC ile kommunikasiya ucun lazim oldughu ucun SoftwareSerial ishledirik
SoftwareSerial BTserial(Rx, Tx);
             
// bu funksiya cemi bir defe program ishe dushende icra olunur                                   
void setup() 
{

  pinMode(Rx, INPUT);  // pinleri girish/cixish kimi teyin et
  pinMode(Tx, OUTPUT);
  pinMode(motor1A, OUTPUT);
  pinMode(motor1B, OUTPUT);
  pinMode(motor2A, OUTPUT);
  pinMode(motor2B, OUTPUT);
  
  brakeMotor(); // tekerleri tormozla, ehtiyat ucun
  
  Serial.begin(9600);
  BTserial.begin(9600); // SoftwareSerial baud qiymeti HC-06 BT modulu ucun 9600.
  
  delay(50);

  Serial.println("AT emrleri daxil edin!");

  BTserial.print("AT"); // status oyrenme emrini BT modula gonder
  //BTserial.print("AT\r\n"); // HC-05 modullari ucun bu emrden istifade edin
  while(BTserial.available())
  { 
      Serial.write(BTserial.read()); // sorgunun cavabini PC-e gonder
  }

  delay(50);
} 

// Bu funksiya sonsuz olaraq tekrarlanir. 
void loop()
{
  if(Serial.available())
    {
        PC_RECEIVED = (char)Serial.read();  // PC-ni oxu
        BTserial.write(PC_RECEIVED);        // BT moduluna gonder
    }

  if(BTserial.available())
    {
       while(BTserial.available())
       {
          BT_RECEIVED = (char)BTserial.read();   // BT-u oxu
          Serial.write(BT_RECEIVED);             // PC-e gonder
          driveRobot(BT_RECEIVED);   // Simvolu hemcinin Robotu idare funksiyasina gonder
       }
    }

    delay(10);
}


void driveRobot(char COMMAND)
{  
  // cuzi donmeler aparmaq ucun esas suret deyishenini
    // texminen 2 defe azalt ve uygun tekerin yeni suretini teyin et
  SPEED_MIX = map(SET_SPEED, 0, 255, 0, 127); 
  // COMMAND deyishenine uyghun addim at
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
        case 'q': 
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
        case 'D': 
              Serial.println("BT elaqesi kesildi");
              break;
        default: 
              brakeMotor(); // istenilen bashqa simvol olduqda tekerleri tormozla
              break;
    }
}

// sag tekerlerin suretini ve istiqametini teyin eden funksiya
void setSpeedRight(unsigned char SPEED, boolean DIR)
{
  if(DIR == true){
      digitalWrite(motor2A, LOW);
      analogWrite (motor2B, SPEED);
  }
  else{
      analogWrite(motor2A, SPEED);
      digitalWrite(motor2B, LOW);
  }
  
}

// sol tekerlerin suretini ve istiqametini teyin eden funksiya
void setSpeedLeft(unsigned char SPEED, boolean DIR)
{
  if(DIR == true){
      digitalWrite(motor1A, LOW);
      analogWrite (motor1B, SPEED);
  }
  else{
      analogWrite(motor1A, SPEED);
      digitalWrite(motor1B, LOW);   
  }
  
}

void brakeMotor(void)
{
    digitalWrite(motor1A, LOW);
    digitalWrite(motor1B, LOW);
    digitalWrite(motor2A, LOW);
    digitalWrite(motor2B, LOW);
}
