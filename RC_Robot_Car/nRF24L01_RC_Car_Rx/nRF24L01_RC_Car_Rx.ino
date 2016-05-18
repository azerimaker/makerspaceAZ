/*
 * www.Makerspace-Az.org
 * Arduino Sketch: 4X4 Robot masin, Radio Receiver Robot Control (Rx)
 * Kommunikasiya: nRF24L01 2.4GHz
 * son modifikasia tarixi: 16 May 2016
 * [azerimaker]
*/

/*-----( Kitabxana elavesi)-----*/
#include <SPI.h>
#include <RF24.h>
/*-----(  Sabitler ve Pin Nomreleri )-----*/
#define MAX_SPEED 255 // analogWrite maks qiymeti
#define MAX_FAILSAFE_COUNT 6

#define CE_PIN   8  // nRF24L01 modulu uchun
#define CSN_PIN 10

#define motor1A 9     // INA  - L298n Motor Surucu pinleri
#define motor1B 6     // INB
#define motor2A 5     // INC
#define motor2B 3     // IND

#define X_min 480
#define X_max 520
#define Y_min 480
#define Y_max 520
/*-----( Radio Obyektinin yaradilmasi )-----*/
RF24 radio(CE_PIN, CSN_PIN);


/*-----( Funksiya Prototipleri )-----*/
inline void calcDirection(int X_pos, int Y_pos);
inline void calcSpeed(int X_pos, int Y_pos);
void driveRobot(char DIRECTION, unsigned char SPEED);
void setSpeedRight(unsigned char SPEED, boolean DIR);
void setSpeedLeft(unsigned char SPEED, boolean DIR);
void motorTormozla(void);

/*-----( Deyishenler )-----*/
byte address[6] = {"1Node"}; //  nRF ucun "Pipe" adi
int failsafe_count = 0;

struct dataStruct {
  int Xposition;          // Joystick qiymetleri
  int Yposition;
  bool switchOn;         
} myData;                 

char Joystick_Dir;     // Joystick kordinatlarini hereket istiqametine ve 
unsigned char Joystick_Speed = 0;   // surete cevirmek
int X_map, Y_map;
unsigned int X_sq, Y_sq;     // suret vektorunun hesablamaq uchun

boolean FORWARD = true; // ireli
boolean BACKWARD = false; // geri

/****** SETUP: Bir defe icra olunacaq ******/                       
void setup() 
{
  pinMode(motor1A, OUTPUT);
  pinMode(motor1B, OUTPUT);
  pinMode(motor2A, OUTPUT);
  pinMode(motor2B, OUTPUT);
  
  motorTormozla(); // tekerleri tormozla, ehtiyat ucun

  Serial.begin(115200);  
  
  radio.begin();          // nRF24L01 Radio transiveri ishe sal
  radio.setChannel(108);  //  WiFi siqnallarindan yuksek bir kanal deyeri sech
  radio.setDataRate(RF24_250KBPS); // Data gonderme sureti
  radio.setPALevel(RF24_PA_HIGH); // Oturme gucu: RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX

  radio.openReadingPipe(1, address); // Qebul Pipe (kanali) ishe sal
  
  radio.startListening(); // Radio siqnallari dinlemeye bashla
  radio.printDetails(); //kommenti sileniz coxlu Debug informasiyasi print olunacaq
} /****** SETUP sonu ******/


 /****** LOOP: Daimi tekrarlanir ******/
void loop()
{

   if ( radio.available()){

    while (radio.available())   // Qebul Pipe-da melumat olarsa
    {
      radio.read( &myData, sizeof(myData) ); // melumati oxu
    }

  //  Serial.print("Data qebul olundu: ");  // Qebul olunmus melumati goster
    Serial.print(myData.Xposition);     // 0-1023 aralighinda,  sabit olduqda 495
    Serial.print(", ");
    Serial.print(myData.Yposition);     // 0-1023 aralighinda, sabit olduqda 501
    if ( myData.switchOn == 1){
      Serial.println(", Achar ON");
    }else
    {
      Serial.println(", Achar OFF");
    }

    // Yeni qebul olunmush melumati robotun hereket sxemine uyghunlashdir
    calcDirection(myData.Xposition, myData.Yposition); // S,F,B,L,R,I,G,H,J
    calcSpeed(myData.Xposition, myData.Yposition);  // 0-255
  
    Serial.print(" ");
    Serial.print(Joystick_Dir);
    Serial.print(" ");
    Serial.println(Joystick_Speed);
   
    driveRobot(Joystick_Dir, Joystick_Speed); 

  }else{
    failsafe_count++;  // radioelaqe bir muddet iterse motoru saxla
    Serial.println(failsafe_count);
    
    if(failsafe_count==MAX_FAILSAFE_COUNT){
        failsafe_count = 0;
        motorTormozla();
      }	
  }//  radio.available shertinin sonu
  delay(100);
}

 /*
 * Bu funksiya X,Y qiymetlerine esasen qlobal 
 * Istiqamet deyishenini yenileyir 
 */ 
inline void calcDirection(int X_pos, int Y_pos){
  
  if(X_pos >= X_min && X_pos <= X_max && Y_pos > Y_max)
  {
    Joystick_Dir = 'F';
  }
  else if(X_pos >= X_min && X_pos <= X_max && Y_pos < Y_min)
  {
    Joystick_Dir = 'B';
  }
  else if(Y_pos >= Y_min && Y_pos <= Y_max && X_pos < X_min)
  {
    Joystick_Dir = 'L';
  }
  else if(Y_pos >= Y_min && Y_pos <= Y_max && X_pos > X_max)
  {
    Joystick_Dir = 'R';
  }
  else if(X_pos > X_max && Y_pos > Y_max)
  {
    Joystick_Dir = 'I';
  }
  else if(X_pos < X_min && Y_pos > Y_max)
  {
    Joystick_Dir = 'G';
  }
  else if(X_pos < X_min && Y_pos < Y_min)
  {
    Joystick_Dir = 'H';
  }
  else if(X_pos > X_max && Y_pos < Y_min)
  {
    Joystick_Dir = 'J';
  }
  else{
      Joystick_Dir = 'S'; // deadzone
  }
}

/*
 * Bu funksiya X,Y qiymetlerine esasen qlobal 
 * Suret deyishenini yenileyir 
 */ 
inline void calcSpeed(int X_pos, int Y_pos){

  X_map = map(X_pos, 0, 1023, -100, 100); // intervali kicilt
  Y_map = map(Y_pos, 0, 1023, -100, 100);
  
  X_map = abs(X_map); // musbet edede cevir
  Y_map = abs(Y_map);

  X_sq = sq(X_map); // kvadrata yukselt
  Y_sq = sq(Y_map);

  Joystick_Speed = (unsigned char)sqrt(X_sq + Y_sq); // Joystick suret vektorunu hesabla, Pifaqor teoremi :)
  if(Joystick_Speed>100){
    Joystick_Speed = 100;
  }
}

/*
 * Bu funksiya suret ve istiqamet deyishenine esasen motor suretini
 * teyin edir 
 */ 
void driveRobot(char DIRECTION, unsigned char SPEED)
{ 
  
  byte SET_SPEED = map(SPEED, 0,100, 0, MAX_SPEED);// SPEED 0-100, intervali deyish 0-255
  byte SPEED_MIX = SET_SPEED/2;

  
  switch(DIRECTION){
      case 'F': // Forward
          setSpeedRight(SET_SPEED, FORWARD);
          setSpeedLeft( SET_SPEED, FORWARD);
          break;
      case 'B': // Backward
          setSpeedRight(SET_SPEED, BACKWARD);
          setSpeedLeft( SET_SPEED, BACKWARD);
          break;
      case 'R': // Right
          setSpeedRight(SPEED_MIX, FORWARD);
          setSpeedLeft( SPEED_MIX, BACKWARD);
          break;
      case 'L': // Left
          setSpeedRight(SPEED_MIX, BACKWARD);
          setSpeedLeft( SPEED_MIX, FORWARD);
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
            motorTormozla(); // istenilen bashqa simvol olduqda tekerleri tormozla
            break;
    }
}

/*
 * sag ve sol tekerlerin suretini ve istiqametini 
 * teyin eden funksiyalar
 */ 
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

void setSpeedLeft(byte SPEED, boolean DIR)
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

void motorTormozla(void)
{
    digitalWrite(motor1A, LOW);
    digitalWrite(motor1B, LOW);
    digitalWrite(motor2A, LOW);
    digitalWrite(motor2B, LOW);
}
