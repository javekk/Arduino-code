const int buzzer = 9; //buzzer to arduino pin 9
const int led = 8; // from pin 8 to resistance to led to gnd
const int button = 7; // from pin 7 to btn to 5v

int stopp = 0;
int weFinished = 0;

void setup(){

   pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output
   pinMode(led, OUTPUT); 
   pinMode(button, INPUT); 

}


void loop(){
      
  if(!weFinished){
      //  digitalWrite(led,HIGH);
        tone(buzzer, 1000); // Send 1KHz sound signal...
        delay(1000);
        
        if(digitalRead(button)){
             noTone(buzzer);
             delay(1000);
      //       digitalWrite(led,LOW);
             weFinished = 1;
        }
        
  }
}
