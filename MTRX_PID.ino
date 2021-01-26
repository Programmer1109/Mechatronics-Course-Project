#include <LiquidCrystal.h>
#include <DHT.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Pins for temperature sensor
#define DHTPIN A0
#define DHTTYPE DHT11

// pins for fan control
#define fanApwm 3
#define fanBpwm 6
#define relayPin 8
#define setTemp 40.0

//Constants for the pid controller
#define Kp 0.0
#define Ki 0.0
#define Kd 0.0

int pwm;
bool flag= LOW;
double error,prevError,P,I,D,PID;

DHT dht(DHTPIN, DHTTYPE);

//functions for the program
void fan(bool ,int );
int temp_PID(double );
void disp(int ,int );

void setup() {
  Serial.begin(9600);
  dht.begin();
  
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Temp Controller");
  lcd.setCursor(0,1);
  lcd.print("Temp ");
  lcd.setCursor(7,1);
  lcd.print(" PWM ");
  
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
  double Tc = dht.readTemperature(); 
  Serial.print("Temperature(in C)= ");
  Serial.print(Tc);
  Serial.println();
  if (isnan(Tc)){
    Serial.println(F("Failed to read from DHT sensor!"));
  }
  else if(isnan(Tc)){
    Serial.print("Temperature(in C)= ");
    Serial.print(Tc);
    Serial.println();
  }  
  if(Tc>=50){
    Serial.println("the temperautre is greater than 60 degrees.");
    digitalWrite(relayPin,LOW);
    flag=HIGH;
    pwm=255;
    fan(flag,pwm);
    disp((int)Tc,pwm);

    if(digitalRead(relayPin)){
      digitalWrite(relayPin,LOW);
    }
    Serial.print("Relay State= ");
    Serial.print(digitalRead(relayPin));
    Serial.println();
    if(digitalRead(relayPin))
      Serial.println("unfortunate turn of events");
    else if(!digitalRead(relayPin))
      Serial.println("Switched Off Relay");
    Serial.print("PWM= ");
    Serial.print(pwm);
    Serial.print(" flag= ");
    Serial.print(flag);
    Serial.println();
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
  
  else if(Tc <50 || Tc >30){
    Serial.println("the temperautre is lesser than 50 degrees but greater than 30 degrees.");
    if(digitalRead(relayPin)){
      digitalWrite(relayPin,LOW);
    }
    flag= HIGH;
    pwm= temp_PID(Tc);
    fan(flag,pwm);
    disp((int)Tc,pwm);
    
    Serial.print("Relay State= ");
    Serial.print(digitalRead(relayPin));
    Serial.println();
    if(digitalRead(relayPin))
      Serial.println("unfortunate turn of events");
    else if(!digitalRead(relayPin))
      Serial.println("Switched Off Relay");
    Serial.print("PWM= ");
    Serial.print(pwm);
    Serial.print(" flag= ");
    Serial.print(flag);
    Serial.println();
    Serial.print("FanA State= ");
    Serial.print(digitalRead(fanApwm));
    Serial.println();
    Serial.print("FanB State= ");
    Serial.print(digitalRead(fanBpwm));
    Serial.println();
    if(digitalRead(fanApwm) && digitalRead(fanBpwm))
      Serial.println("Switched on both fans");
    else if(digitalRead(fanApwm) && !digitalRead(fanBpwm))
      Serial.println("Switched on  fanA, couldn't switch fanB.");
    else if(!digitalRead(fanApwm) && digitalRead(fanBpwm))
      Serial.println("Switched on  fanB, couldn't switch fanA.");
    else if(!digitalRead(fanApwm) && !digitalRead(fanBpwm))
      Serial.println("couldn't switch on either fanA or fanB");
  }
  
  else if(Tc<=30){
    Serial.println("the temperautre is lesser than 30 degrees");
    digitalWrite(relayPin,HIGH);
    flag= LOW;
    pwm=0;
    fan(flag,pwm);
    disp((int)Tc,pwm);
    
    Serial.print("Relay State= ");
    Serial.print(digitalRead(relayPin));
    Serial.println();
    if(digitalRead(relayPin))
      Serial.println("unfortunate turn of events");
    else if(!digitalRead(relayPin))
      Serial.println("Switched oOff Relay");
    Serial.print("PWM= ");
    Serial.print(pwm);
    Serial.print(" flag= ");
    Serial.print(flag);
    Serial.println();
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
  delay(300);
}

int temp_PID(double val){
  error = setTemp - val;
  Serial.print("Error= ");
  Serial.print(error);
  P= Kp*error;
  Serial.print(" P= ");
  Serial.print(P); 
  I= Ki*(error + prevError);
  Serial.print(" I= ");
  Serial.print(I);
  D= Kd*(error - prevError);
  Serial.print(" D= ");
  Serial.print(D);
  PID= (P+I+D);
  Serial.print(" PID= ");
  Serial.print(PID);
  prevError = error;
  Serial.print(" prevError= ");
  Serial.print(prevError);
  if(error>=0 && error<=20){
    return 150 -(int)PID;
  }
  else if (error<0 && error>=-20 ){
    return 150 +(int)PID;
  }
}

void fan(bool var,int fanspeed){
  if(!var){
    analogWrite(fanApwm,0);
    analogWrite(fanBpwm,0);
  }
  else if(var){
    analogWrite(fanApwm,fanspeed);
    analogWrite(fanBpwm,fanspeed);
  }
}

void disp(int temp,int vel){
  lcd.setCursor(5,1);
  lcd.print(temp);
  delay(300);
  lcd.setCursor(12,1);
  lcd.print(vel);
  delay(300);
}
