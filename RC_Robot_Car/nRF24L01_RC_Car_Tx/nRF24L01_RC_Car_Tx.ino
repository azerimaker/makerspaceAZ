/*
 * www.Makerspace-Az.org
 * Arduino Sketch: 4X4 Robot masin, Radio Transmitter (Tx)
 * Kommunikasiya: nRF24L01 2.4GHz
 * son modifikasia tarixi: 16 May 2016
 * [azerimaker]
*/

/*-----( Kitabxana elavesi)-----*/
#include <SPI.h>
#include <RF24.h>
/*-----(  Sabitler ve Pin Nomreleri )-----*/
#define CE_PIN   9
#define CSN_PIN 10

#define JOYSTICK_X   A1  // Joystick VRx Analoq pin A0-a baglanir
#define JOYSTICK_Y   A0  // Joystick VRy Analoq pin A1-e baglanir
#define JOYSTICK_SW  4  //  Joystick merkeze basildiqda achar

/*-----( Radio Obyektinin yaradilmasi )-----*/
RF24 radio(CE_PIN, CSN_PIN); 

/*-----( Deyishenler )-----*/
byte address[6] = {"1Node"}; //  "Pipe" adi, nrf24l01 uchun

// Proqramla Analoq qiymetleri filterleme uchun deyishenler
int X_qiymeti = 0;
int Y_qiymeti = 0;        
float X_exp_emsal = 0.8;      //exponensial emsal qiymeti, 1-olduqda filtirleme deaktiv olur
float Y_exp_emsal = 0.8;      // bu emsali isteyinizw gore deyishe bilersiniz (0-1] aralighinda
int X_exp_cem = 0;            //exponensial cemin bashlangic qiymeti
int Y_exp_cem = 0;     

// Joystick qiymetleri olan deyishenler strukturu
struct dataStruct {
  int Xposition;           
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

  // Filterleme ucun bashlangic hesablama
  X_exp_cem =  analogRead(JOYSTICK_X); 
  Y_exp_cem =  analogRead(JOYSTICK_Y); 
  
} /****** SETUP sonu ******/


 /****** LOOP: Daimi tekrarlanir ******/
void loop()  
{
  X_qiymeti = analogRead(JOYSTICK_X);  // Jostick qiymetlerini oxu
  Y_qiymeti = analogRead(JOYSTICK_Y);
  
  myData.switchOn  = !digitalRead(JOYSTICK_SW);  // Achar basildiqda 0V oldughu ucun, inversiya etmeliyik
  
/* 
 * Bu emeliyyat zamani X_exp_emsal qiymeti ne qeder kicik olarsa, 
 * yeni oxunan X_qiymeti yekun X_exp_cem qiymetine bir o qeder az 
 * tesir edecek, neticede sert kecidler, yumshaq olacaq
 * 
 */
  X_exp_cem = (X_exp_emsal * X_qiymeti) + ((1-X_exp_emsal)*X_exp_cem); // Exponensial filterleme emeliyyati
  Y_exp_cem = (Y_exp_emsal * Y_qiymeti) + ((1-Y_exp_emsal)*Y_exp_cem); 
        
  myData.Xposition = X_exp_cem; // Filter olunmush qiymetleri umumi struktura yaz
  myData.Yposition = Y_exp_cem;

  // Bu Serial Print-leri komment ede bilersiniz. 
  Serial.print("Indi gonderilir, X= ");
  Serial.print(myData.Xposition);
  Serial.print(", Y= ");
  Serial.print(myData.Yposition);
  if ( myData.switchOn == 1)
  {
      Serial.println(", Achar ON");
  }else
  {
      Serial.println(", Achar OFF");
  }

  if(!radio.write( &myData, sizeof(myData) )) {  // Melumati gonder ve xeta olarsa print
    //Serial.println("Gonderilme ugursuz oldu");
  }
  
  delay(50); // Delay olmasi stabilliyi artirir. 
  

} /****** LOOP sonu ******/


