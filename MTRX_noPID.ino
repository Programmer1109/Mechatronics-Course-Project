#define thermistorPin A0
#define fanA1 2 
#define fanA2 4
#define fanApwm 3
#define fanB1 5
#define fanB2 7
#define fanBpwm 6
#define relayPin 8
double pwm;
bool flag= LOW;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
//  Wire.begin();
  pinMode(thermistorPin,INPUT);
  pinMode(fanA1,OUTPUT);
  pinMode(fanA2,OUTPUT);
  pinMode(fanApwm,OUTPUT);
  pinMode(fanB1,OUTPUT);
  pinMode(fanB2,OUTPUT);
  pinMode(fanBpwm,OUTPUT);
  pinMode(relayPin,OUTPUT);
  
  Serial.println("Starting...");
}


void loop() {
  // put your main code here, to run repeatedly:
  double temp = analogRead(thermistorPin);
  Serial.print("temp= ");
  Serial.print(temp);
  Serial.println();
  if(temp >60){
    flag= HIGH;
    digitalWrite(relayPin,LOW);
    Serial.println("Switching OFF relay, Switching ON Fan");
    delay(100);
    fan(flag);
    temp = 60;
  }
  else if(temp<10)
    temp = 0;
  
  else if(temp<=20){
    flag= LOW; 
    Serial.println("Switching ON relay, Switching OFF Fan");
    digitalWrite(relayPin,HIGH);
    fan(flag);
    delay(100);     
  }
  pwm = dmap(temp,20,60,80,255);
  analogWrite(fanApwm,(int)pwm);
  analogWrite(fanBpwm,(int)pwm);  
  delay(300);
}

double dmap(double x,double in_min,double in_max,double out_min,double out_max){
  double val = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  return val; 
}

void fan(bool var){
  digitalWrite(fanA1,var);
  digitalWrite(fanA2,var);
  digitalWrite(fanB1,var);
  digitalWrite(fanB2,var);
  if(!var){
    analogWrite(fanApwm,35);
    analogWrite(fanBpwm,35);
  }
  else{
    analogWrite(fanApwm,255);
    analogWrite(fanBpwm,255);
  }
}
