#define IN1  3
#define IN2  4
#define IN3  5
#define IN4  6

int Steps = 0;
boolean Direction = true;
unsigned long last_time;
unsigned long currentMillis ;
int steps_left=4095;
long time;

void setup()
{
Serial.begin(115200);
pinMode(IN1, OUTPUT); 
pinMode(IN2, OUTPUT); 
pinMode(IN3, OUTPUT); 
pinMode(IN4, OUTPUT); 


}
void loop()
{
  while(steps_left>0){
  currentMillis = micros();
  if(currentMillis-last_time>=1000){
  stepper(1); 
  time=time+micros()-last_time;
  last_time=micros();
  steps_left--;
  }
  }
   Serial.println(time);
  Serial.println("Wait...!");
  delay(1000);
  Direction=!Direction;
  steps_left=5095;
}

void stepper(int xw){
  for (int x=0;x<xw;x++){
  switch(Steps){
     case 0:
       digitalWrite(IN1, LOW); 
       digitalWrite(IN2, LOW);
       digitalWrite(IN3, LOW);
       digitalWrite(IN4, HIGH);
       Serial.println("case 0");
     break; 
     case 1:
       digitalWrite(IN1, LOW); 
       digitalWrite(IN2, LOW);
       digitalWrite(IN3, HIGH);
       digitalWrite(IN4, HIGH);
       Serial.println("case 1");
     break; 
     case 2:
       digitalWrite(IN1, LOW); 
       digitalWrite(IN2, LOW);
       digitalWrite(IN3, HIGH);
       digitalWrite(IN4, LOW);
       Serial.println("case 2");
     break; 
     case 3:
       digitalWrite(IN1, LOW); 
       digitalWrite(IN2, HIGH);
       digitalWrite(IN3, HIGH);
       digitalWrite(IN4, LOW);
       Serial.println("case 3");
     break; 
     case 4:
       digitalWrite(IN1, LOW); 
       digitalWrite(IN2, HIGH);
       digitalWrite(IN3, LOW);
       digitalWrite(IN4, LOW);
       Serial.println("case 4");
     break; 
     case 5:
       digitalWrite(IN1, HIGH); 
       digitalWrite(IN2, HIGH);
       digitalWrite(IN3, LOW);
       digitalWrite(IN4, LOW);
       Serial.println("case 5");
     break; 
       case 6:
       digitalWrite(IN1, HIGH); 
       digitalWrite(IN2, LOW);
       digitalWrite(IN3, LOW);
       digitalWrite(IN4, LOW);
       Serial.println("case 6");
     break; 
     case 7:
       digitalWrite(IN1, HIGH); 
       digitalWrite(IN2, LOW);
       digitalWrite(IN3, LOW);
       digitalWrite(IN4, HIGH);
       Serial.println("case 7");
     break; 
     default:
       digitalWrite(IN1, LOW); 
       digitalWrite(IN2, LOW);
       digitalWrite(IN3, LOW);
       digitalWrite(IN4, LOW);
       Serial.println("case  default");
     break; 
  }
SetDirection();
}
} 
void SetDirection(){
if(Direction==1){ Steps++;}
if(Direction==0){ Steps--; }
if(Steps>7){Steps=0;}
if(Steps<0){Steps=7; }
}
