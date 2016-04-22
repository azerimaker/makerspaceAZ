
/*
 * www.Makerspace-Az.org
 * Arduino Sketch: 4X4 Robot masin, Radio Receiver Test (Rx)
 * Kommunikasiya: nRF24L01 2.4GHz
 * modifikasia olunub: 12 Aprel 2016
 * [azerimaker]
*/
/*-----( Kitabxana elavesi)-----*/
#include <SPI.h>
#include <RF24.h>
/*-----(  Sabitler ve Pin Nomreleri )-----*/
#define CE_PIN   8
#define CSN_PIN 10

/*-----( Radio Obyektinin yaradilmasi )-----*/
RF24 radio(CE_PIN, CSN_PIN);

/*-----( Deyishenler )-----*/
byte address[6] = {"1Node"}; //  "Pipe" adi

struct dataStruct {
  int Xposition;          // Joystick qiymetleri
  int Yposition;
  bool switchOn;         
} myData;                 


 /****** SETUP: Bir defe icra olunacaq ******/
void setup()  
{
  Serial.begin(115200);  
  
  radio.begin();          // nRF24L01 Radio transiveri ishe sal
  radio.setChannel(108);  //  WiFi siqnallarindan yuksek bir kanal deyeri sech
  radio.setDataRate(RF24_250KBPS); // Data gonderme sureti
  radio.setPALevel(RF24_PA_HIGH); // Oturme gucu: RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX

  radio.openReadingPipe(1, address); // Qebul Pipe (kanali) ishe sal
  
  radio.startListening(); // Radio siqnallari dinlemeye bashla
//  radio.printDetails(); //kommenti sileniz coxlu Debug informasiyasi print olunacaq

}/****** SETUP sonu ******/


 /****** LOOP: Daimi tekrarlanir ******/
void loop()  
{
  if ( radio.available())
  {

    while (radio.available())   // Qebul Pipe-da melumat olarsa
    {
      radio.read( &myData, sizeof(myData) ); // melumati oxu
    }

    
    Serial.print("Data qebul olundu: ");  // Qebul olunmus melumati goster
    Serial.print(myData.Xposition);
    Serial.print(", Y= ");
    Serial.print(myData.Yposition);
    if ( myData.switchOn == 1)
    {
      Serial.println(", Achar ON");
    }
    else
    {
      Serial.println(", Achar OFF");
    }

  } //  radio.available shertinin sonu


}/****** LOOP sonu ******/

