const int IR_Sensor=5;
int counter = 0;

void setup() {

// initialize the digital pin as an output.

// Pin 13 has an LED  on  Arduino boards:
  Serial.begin(9600);
   while (! Serial); 
pinMode(4, OUTPUT);

//Pin 2 is connected to the output of IR_Sensor

pinMode(IR_Sensor,INPUT);

}

void loop() {

if(digitalRead(IR_Sensor)==HIGH)      //Check the sensor output

{
  digitalWrite(4, HIGH);   // set the LED on
  
}

else

{

  
digitalWrite(4, LOW);    // set the LED off
  
  counter++;
  Serial.println(counter);
  delay(300);
}

delay(1);              // wait for a second

}
