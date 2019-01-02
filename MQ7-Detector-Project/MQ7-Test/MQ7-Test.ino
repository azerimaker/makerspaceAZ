// sabitler
#define MQ7_SENSOR_CTRL 6
#define MQ7_SENSOR_IN A0
#define BUZZER 9
#define P_BUTTON 2
#define LED_PIN 10

// funksiya deklarasiyasi
void buzzer_alarm(int BEEP_COUNT);

void setup() 
{
  Serial.begin(9600);
  // Giriş-Çıxış pinləri - IO pins
  // Analoq giriş pinlərini PinMode etməyə ehtiyac yoxdur
  pinMode(MQ7_SENSOR_CTRL, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(P_BUTTON,INPUT);

  delay(500);
  buzzer_alarm(5); // start siqnalı
}

void loop() 
{
  analogWrite(MQ7_SENSOR_CTRL, 255); // Qızdırıcı elementi tam qoş
  analogWrite(LED_PIN, 255); // LED tam qoş - vizual göstəriş üçün
 
  delay(5900); // 60san gözlə (isinme periodu)

  // Qızdırıcı elementi 1.4V-a endir
  analogWrite(MQ7_SENSOR_CTRL, 72);  // 255x1.4/5 = ~72
  analogWrite(LED_PIN, 10); // LED zəif qoş - vizual göstəriş üçün
  
  
  delay(9000); // 90san gözlə 

  int MQ7_sensor_val = analogRead(MQ7_SENSOR_IN); // sensor dəyərini oxu

  Serial.println(MQ7_sensor_val); // Terminala çap et

  delay(1000); // 1san gözlə 

  if(MQ7_sensor_val>=300 && MQ7_sensor_val<400)
  {
    buzzer_alarm(1); // 1 beep - riskli zona
  }else if(MQ7_sensor_val>=400 && MQ7_sensor_val<500)
  {
    buzzer_alarm(2); // 2 beep
  }else if(MQ7_sensor_val>=500 && MQ7_sensor_val<700)
  {
    buzzer_alarm(3); // 3 beep
  }else if(MQ7_sensor_val>=700)
  {
    buzzer_alarm(4); // 4 beep
  }
    
}

void buzzer_alarm(int BEEP_COUNT)
{
    for(int i=0; i<BEEP_COUNT; i++)
    {
      digitalWrite(BUZZER,HIGH);
      delay(100);
      digitalWrite(BUZZER,LOW);
      delay(100); 
    }
}
