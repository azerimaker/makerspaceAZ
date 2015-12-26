/* Sade elaqe proqrami, Smartfondan gelen simvollari PC-e gonder*/
#include <SoftwareSerial.h>

#define Rx 2          // BT modulun TX pinine qoshulur     
#define Tx 3          // Gerginlik bolucu rezistorlarla BT modulun RX pinine qoshulur

// deyishenlar ve sabitler
char BT_RECEIVED;   // BT-dan gelen simvol
char PC_RECEIVED;   // PC-den gelen simvol

// Hardware serial PC ile kommunikasiya ucun lazim oldughu ucun SoftwareSerial ishledirik
SoftwareSerial BTserial(Rx, Tx);
             
// bu funksiya cemi bir defe program ishe dushende icra olunur                                   
void setup() 
{ 
  Serial.begin(9600);
  BTserial.begin(9600); // SoftwareSerial baud qiymeti HC-06 BT modulu ucun 9600.
  
  Serial.println("AT emrleri daxil edin!");
  delay(50);
} 

// Bu funksiya sonsuz olaraq tekrarlanir. 
void loop()
{
  if(Serial.available())
  {
      PC_RECEIVED = (char)Serial.read();  // PC-ni oxu
      BTserial.write(PC_RECEIVED);        // Eger PC-den nese gonderilibse, somvolu BT modula gonder
  }

  if(BTserial.available())
  {
     while(BTserial.available())
     {
        BT_RECEIVED = (char)BTserial.read();   // BT-u oxu
        Serial.write(BT_RECEIVED);             // PC-e gonder
     }
  }

  delay(10);  // Serial.write ele de suretli olmadighindan biraz gozlemek stabilliyi artirir. 
}

