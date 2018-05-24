const int buzzer = 9; //buzzer to arduino pin 9
const int led = 8;

void setup(){

 

  pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output
   pinMode(led, OUTPUT); 


}


void loop(){

 

  tone(buzzer, 5000); // Send 1KHz sound signal...
  digitalWrite(led, HIGH);
  
  delay(1000);        // ...for 1 sec

  noTone(buzzer);     // Stop sound...
  digitalWrite(led, LOW);
  delay(1000);        // ...for 1sec

  

}
