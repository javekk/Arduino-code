#define IN1  3
#define IN2  4
#define IN3  5
#define IN4  6
#define IN1B  8
#define IN2B  9
#define IN3B  10
#define IN4B  11

int Steps = 0;
int Steps2 = 0;
boolean Direction = true;
boolean Direction2 = true;
unsigned long last_time;
unsigned long last_time2;
unsigned long currentMillis ;
unsigned long currentMillis2 ;
int steps_left=4095;
int steps_left2=4095;
long time;


const int IR_Sensor=12;
const int IR_Sensor2=13;
int counter = 0;
int counter2 = 0;

boolean dispense_detech_stack1 = false;
boolean dispense_detech_stack2 = false;
void setup()
{
Serial.begin(115200);
pinMode(IN1, OUTPUT); 
pinMode(IN2, OUTPUT); 
pinMode(IN3, OUTPUT); 
pinMode(IN4, OUTPUT); 
pinMode(IN1B, OUTPUT); 
pinMode(IN2B, OUTPUT); 
pinMode(IN3B, OUTPUT); 
pinMode(IN4B, OUTPUT); 

    
pinMode(IR_Sensor,INPUT);
pinMode(IR_Sensor2,INPUT);

}
void loop()
{
    while(!dispense_detech_stack1){
    while(steps_left>0){
    currentMillis = micros();
    if(currentMillis-last_time>=1000){
    stepper(1);     
    time=time+micros()-last_time;
    last_time=micros();
    steps_left--;
    }
    }
    Serial.println("Wait...!");
    delay(1000);
    Direction=!Direction;
    steps_left=4095;
  }


  while(!dispense_detech_stack2){
    while(steps_left2>0){
    currentMillis2 = micros();
    if(currentMillis2-last_time2>=2000){
    stepper2(1);     
    time=time+micros()-last_time2;
    last_time2=micros();
    steps_left2--;
    }
    }
    Serial.println("Wait 2...!");
    delay(1000);
    Direction2=!Direction2;
    steps_left2=4095;
  }
  

 
   

  
}

void stepper(int xw){
  for (int x=0;x<xw;x++){
switch(Steps){
   case 0:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, HIGH);
   break; 
   case 1:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, HIGH);
     digitalWrite(IN4, HIGH);
   break; 
   case 2:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, HIGH);
     digitalWrite(IN4, LOW);
   break; 
   case 3:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, HIGH);
     digitalWrite(IN3, HIGH);
     digitalWrite(IN4, LOW);
   break; 
   case 4:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, HIGH);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
   break; 
   case 5:
     digitalWrite(IN1, HIGH); 
     digitalWrite(IN2, HIGH);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
   break; 
     case 6:
     digitalWrite(IN1, HIGH); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
   break; 
   case 7:
     digitalWrite(IN1, HIGH); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, HIGH);
     
     pill_dispensed();
   break; 
   default:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
     
   break; 
}
SetDirection();
}
} 

void stepper2(int xw){
  for (int x=0;x<xw;x++){
switch(Steps2){
   case 0:
     digitalWrite(IN1B, LOW); 
     digitalWrite(IN2B, LOW);
     digitalWrite(IN3B, LOW);
     digitalWrite(IN4B, HIGH);
   break; 
   case 1:
     digitalWrite(IN1B, LOW); 
     digitalWrite(IN2B, LOW);
     digitalWrite(IN3B, HIGH);
     digitalWrite(IN4B, HIGH);
   break; 
   case 2:
     digitalWrite(IN1B, LOW); 
     digitalWrite(IN2B, LOW);
     digitalWrite(IN3B, HIGH);
     digitalWrite(IN4B, LOW);
   break; 
   case 3:
     digitalWrite(IN1B, LOW); 
     digitalWrite(IN2B, HIGH);
     digitalWrite(IN3B, HIGH);
     digitalWrite(IN4B, LOW);
   break; 
   case 4:
     digitalWrite(IN1B, LOW); 
     digitalWrite(IN2B, HIGH);
     digitalWrite(IN3B, LOW);
     digitalWrite(IN4B, LOW);
   break; 
   case 5:
     digitalWrite(IN1B, HIGH); 
     digitalWrite(IN2B, HIGH);
     digitalWrite(IN3B, LOW);
     digitalWrite(IN4B, LOW);
   break; 
     case 6:
     digitalWrite(IN1B, HIGH); 
     digitalWrite(IN2B, LOW);
     digitalWrite(IN3B, LOW);
     digitalWrite(IN4B, LOW);
   break; 
   case 7:
     digitalWrite(IN1B, HIGH); 
     digitalWrite(IN2B, LOW);
     digitalWrite(IN3B, LOW);
     digitalWrite(IN4B, HIGH);
     
     pill_dispensed2();
   break; 
   default:
     digitalWrite(IN1B, LOW); 
     digitalWrite(IN2B, LOW);
     digitalWrite(IN3B, LOW);
     digitalWrite(IN4B, LOW);
     
   break; 
}
SetDirection2();
}
} 

void SetDirection(){
if(Direction==1){ Steps++;}
if(Direction==0){ Steps--; }
if(Steps>7){Steps=0;}
if(Steps<0){Steps=7; }
    
}

void SetDirection2(){
if(Direction2==1){ Steps2++;}
if(Direction2==0){ Steps2--; }
if(Steps2>7){Steps2=0;}
if(Steps2<0){Steps2=7; }
    
}


void pill_dispensed(){
   
  
if(digitalRead(IR_Sensor)==HIGH){
 
  
} 
else{
 
  counter++;
  Serial.println(counter );
  Serial.println(micros());
  delay(300);
  dispense_detech_stack1 = true;
} 
  
}

void pill_dispensed2(){
   
  
if(digitalRead(IR_Sensor2)==HIGH){
 
  
} 
else{
 
  counter2++;
  Serial.println(counter2+" Counter 2");
  Serial.println(micros());
  delay(300);
  dispense_detech_stack2 = true;
} 
  
}
