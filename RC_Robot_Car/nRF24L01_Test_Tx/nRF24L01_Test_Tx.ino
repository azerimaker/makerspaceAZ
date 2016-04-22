/*
 * www.Makerspace-Az.org
 * Arduino Sketch: 4X4 Robot masin, Radio Transmitter (Tx)
 * Kommunikasiya: nRF24L01 2.4GHz
 * modifikasia olunub: 12 Aprel 2016
 * [azerimaker]
*/

/*-----( Kitabxana elavesi)-----*/
#include <SPI.h>
#include <RF24.h>
/*-----(  Sabitler ve Pin Nomreleri )-----*/
#define CE_PIN   9
#define CSN_PIN 10

#define JOYSTICK_X   A0  // Joystick VRx Analoq pin A0-a baglanir
#define JOYSTICK_Y   A1  // Joystick VRy Analoq pin A1-e baglanir
#define JOYSTICK_SW  4  //  Joystick merkeze basildiqda achar

/*-----( Radio Obyektinin yaradilmasi )-----*/
RF24 radio(CE_PIN, CSN_PIN); 

/*-----( Deyishenler )-----*/
byte address[6] = {"1Node"}; //  "Pipe" adi

struct dataStruct {
  int Xposition;           // Joystick qiymetleri olan deyishenler strukturu
  int Yposition;
  bool switchOn;          
} myData;                


 /****** SETUP: Bir defe icra olunacaq ******/
void setup()  
{
  Serial.begin(115200);
  pinMode(JOYSTICK_SW, INPUT_PULLUP);  // Kenar rezistor ishletmeye ehtiyac yoxdu
                                      //  daxili Pull-up rezistorunu ishe saliriq                                     
  radio.begin();          // nRF24L01 Radio transiveri ishe sal
  radio.setChannel(108);  //  WiFi siqnallarindan yuksek bir kanal deyeri sech
  radio.setDataRate(RF24_250KBPS); // Data gonderme sureti
  radio.setPALevel(RF24_PA_HIGH); // Oturme gucu: RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
 
  radio.openWritingPipe(address); // Gonderme Pipe (kanali) ishe sal
//  radio.printDetails(); //kommenti sileniz coxlu Debug informasiyasi print olunacaq

} /****** SETUP sonu ******/

 /****** LOOP: Daimi tekrarlanir ******/
void loop()  
{
  myData.Xposition = analogRead(JOYSTICK_X);  // Jostick qiymetlerini oxu
  myData.Yposition = analogRead(JOYSTICK_Y);
  myData.switchOn  = !digitalRead(JOYSTICK_SW);  // Achar basildiqda 0V oldughu ucun, inversiya etmeliyik

  Serial.print("Indi gonderilir: ");

  if(!radio.write( &myData, sizeof(myData) )) {            // Melumati gonder ve xeta olarsa print
    Serial.println("Gonderilme ugursuz oldu");
  }
  
  delay(100); // Delay olmasi stabilliyi artirir. 
  

} /****** LOOP sonu ******/


