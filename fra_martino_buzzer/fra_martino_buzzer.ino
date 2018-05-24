const int buzzer = 9;
const int led = 8;

void setup(){

   pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output
   pinMode(led, OUTPUT); 
   
}



void loop(){
  doo();
  re();
  mi();
  doo();
  doo();
  re();
  mi();
  doo();
  mi();
  fa();
  sol();  
  mi();
  fa();
 
}

void doo(){
   tone(buzzer, 1047);      //DO 
   digitalWrite(led, HIGH);
   delay(1200);
   
   noTone(buzzer);     // Stop sound...
   digitalWrite(led, LOW);
   delay(300);
}

void re(){
   tone(buzzer, 1175);      //RE
   digitalWrite(led, HIGH);
   delay(1200);
   
   noTone(buzzer);     // Stop sound...
   digitalWrite(led, LOW);
   delay(300);
}

void mi(){
   tone(buzzer, 1319);      //MI
   digitalWrite(led, HIGH);
   delay(1200);
   
   noTone(buzzer);     // Stop sound...
   digitalWrite(led, LOW);
   delay(300);
}


void fa(){
   tone(buzzer, 1397);      //fa
   digitalWrite(led, HIGH);
   delay(1200);
   
   noTone(buzzer);     // Stop sound...
   digitalWrite(led, LOW);
   delay(300);
}

void sol(){
   tone(buzzer, 1568);      //SOL
   digitalWrite(led, HIGH);
   delay(1200);
   
   noTone(buzzer);     // Stop sound...
   digitalWrite(led, LOW);
   delay(300);
}

void la(){
   tone(buzzer, 1760);      //La 
   digitalWrite(led, HIGH);
   delay(1200);
   
   noTone(buzzer);     // Stop sound...
   digitalWrite(led, LOW);
   delay(300);
}

void si(){
   tone(buzzer, 1976);      //SI
   digitalWrite(led, HIGH);
   delay(1200);
   
   noTone(buzzer);     // Stop sound...
   digitalWrite(led, LOW);
   delay(300);
}


