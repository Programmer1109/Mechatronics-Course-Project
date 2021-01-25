#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define thermistorPin A0
#define fanApwm 5
#define fanBpwm 6
#define relayPin 8
#define R1 1000.00
#define c1 1.009249522e-3
#define c2 2.378405444e-4
#define c3 2.019202697e-7

int pwm;
bool flag= LOW;

void fan(bool ,int );
void disp(int ,int );

void setup(){
  Serial.begin(9600);
  
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Temp Controller");
  lcd.setCursor(0,1);
  lcd.print("Temp ");
  lcd.setCursor(7,1);
  lcd.print(" PWM ");
  
  pinMode(thermistorPin,INPUT);
  pinMode(fanApwm,OUTPUT);
  pinMode(fanBpwm,OUTPUT);
  pinMode(relayPin,OUTPUT);
  
  Serial.println("Starting...");
  digitalWrite(relayPin,HIGH);
  Serial.print("Relay State= ");
  Serial.print(digitalRead(relayPin));
  Serial.println();
  if(!digitalRead(relayPin))
    Serial.println("unfortunate turn of events");
  else if(digitalRead(relayPin))
    Serial.println("Switched on Relay");
}


void loop() {
  double R2,T,Tc;
  int volt = analogRead(thermistorPin);
  R2 = R1*(10230/(double)volt-1);
  T= (1.0/(c1+c2*log(R2)+c3*pow(log(R2),3)));
  Serial.print("Temperature(in K)= ");
  Serial.print(T);
  Serial.println();
  Tc= T-273.15;
  Serial.print("Temperature(in C)= ");
  Serial.print(Tc);
  Serial.println();
  
  pwm = map((int)Tc,20,60,80,255);
  if(Tc>60){
    if(digitalRead(relayPin)){
      digitalWrite(relayPin,LOW);
    }
    
    flag=HIGH;
    pwm=255;
    fan(flag,pwm);
    disp((int)Tc,pwm);
    Serial.print("Relay State= ");
    Serial.print(digitalRead(relayPin));
    Serial.println();
    if(digitalRead(relayPin))
      Serial.println("unfortunate turn of events");
    else if(!digitalRead(relayPin))
      Serial.println("Switched oOff Relay");
    Serial.print("FanA State= ");
    Serial.print(digitalRead(fanApwm));
    Serial.println();
    Serial.print("FanB State= ");
    Serial.print(digitalRead(fanBpwm));
    Serial.println();
    if(digitalRead(fanApwm) && digitalRead(fanBpwm))
      Serial.println("Switched on both fans");
    else if(digitalRead(fanApwm) && !digitalRead(fanBpwm))
      Serial.println("Switched on both fanA, couldn't switch fanB.");
    else if(!digitalRead(fanApwm) && digitalRead(fanBpwm))
      Serial.println("Switched on both fanB, couldn't switch fanA.");
    else if(!digitalRead(fanApwm) && !digitalRead(fanBpwm))
      Serial.println("couldn't switch on either fanA or fanB");
      
  }
  
  else if(Tc<60 || Tc>30){
    if(digitalRead(relayPin)){
      digitalWrite(relayPin,LOW);
    }
    
    Serial.print("Relay State= ");
    Serial.print(digitalRead(relayPin));
    Serial.println();
    if(digitalRead(relayPin))
      Serial.println("unfortunate turn of events");
    else if(!digitalRead(relayPin))
      Serial.println("Switched oOff Relay");
    
    fan(flag,pwm);
    disp((int)Tc,pwm);
    Serial.print("FanA State= ");
    Serial.print(digitalRead(fanApwm));
    Serial.println();
    Serial.print("FanB State= ");
    Serial.print(digitalRead(fanBpwm));
    Serial.println();
    if(digitalRead(fanApwm) && digitalRead(fanBpwm))
      Serial.println("Switched on both fans");
    else if(digitalRead(fanApwm) && !digitalRead(fanBpwm))
      Serial.println("Switched on both fanA, couldn't switch fanB.");
    else if(!digitalRead(fanApwm) && digitalRead(fanBpwm))
      Serial.println("Switched on both fanB, couldn't switch fanA.");
    else if(!digitalRead(fanApwm) && !digitalRead(fanBpwm))
      Serial.println("couldn't switch on either fanA or fanB");
  }

  else if(Tc<=30 || Tc>10){
    if(!digitalRead(relayPin)){
      digitalWrite(relayPin,HIGH);
    }
    pwm=0;
    fan(flag,pwm);
    disp((int)Tc,pwm);
    
    Serial.print("FanA State= ");
    Serial.print(digitalRead(fanApwm));
    Serial.println();     
    Serial.print("FanB State= ");
    Serial.print(digitalRead(fanBpwm));
    Serial.println();
    if(digitalRead(fanApwm) && digitalRead(fanBpwm))
      Serial.println("Switched on both fans");
    else if(digitalRead(fanApwm) && !digitalRead(fanBpwm))
      Serial.println("Switched on both fanA, couldn't switch fanB.");
    else if(!digitalRead(fanApwm) && digitalRead(fanBpwm))
      Serial.println("Switched on both fanB, couldn't switch fanA.");
    else if(!digitalRead(fanApwm) && !digitalRead(fanBpwm))
      Serial.println("couldn't switch on either fanA or fanB");
  }
  else if(Tc<10){
    pwm=0;
    flag= LOW;
    if(!digitalRead(relayPin)){
      digitalWrite(relayPin,HIGH);
    }
    fan(flag,pwm);
    disp((int)Tc,pwm);
  }
  delay(300);
}

void fan(bool var,int vel){
  if(!var){
    analogWrite(fanApwm,50);
    analogWrite(fanBpwm,50);
  }
  else if (var){
    analogWrite(fanApwm,vel);
    analogWrite(fanBpwm,vel);
  }
}

void disp(int temp,int vel){
  lcd.setCursor(5,1);
  lcd.print(temp);
  delay(100);
  lcd.setCursor(12,1);
  lcd.print(vel);
  delay(100);
}
