

const int buzzer = 9; //buzzer to arduino pin 9
const int led = 8; //
const int pushBtn = 2; // direct to 5V


// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 1000;           // interval at which to blink (milliseconds)

int state = LOW; 

int isButtonPressed = 0;
int weFinished = 0;

void setup(){

  pinMode(pushBtn, INPUT);
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output
  pinMode(led, OUTPUT);
}

void loop(){
  
    isButtonPressed = digitalRead(pushBtn);
    delay(200); 
    if(!weFinished && isButtonPressed){
        state = LOW;
        noTone(buzzer);     // Stop sound...
        digitalWrite(led, state); 
        weFinished = 1;
    }
    else if(!weFinished){
      
      unsigned long currentMillis = millis();
     
      if (currentMillis - previousMillis >= interval) {
        // save the last time you blinked the LED
        previousMillis = currentMillis;
    
        // if the LED is off turn it on and vice-versa:
        if (state == LOW) { 
          state = HIGH;
          tone(buzzer, 100000); // Send 1KHz sound signal...
          digitalWrite(led, state);   
        } 
        else {
          state = LOW;
          noTone(buzzer);     // Stop sound...
          digitalWrite(led, state);     
        }
      }  
    }
  
}
