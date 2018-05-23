/*https://arduino.stackexchange.com/questions/9483/how-to-communicate-the-arduino-board-with-sim900
 
*/
#include <SoftwareSerial.h>

SoftwareSerial GPRS(7, 8);         //7 = TX, 8 = RX
unsigned char buffer[64]; //port
int count=0;     
int i = 1;                         //if i = 0, send SMS. Hvis sat til 0, vil sms med "test" blive sendt ved start op.
void setup(){
  //delay(10000);
  GPRS.begin(19200);               // the GPRS baud rate   
  Serial.begin(19200);             // the Serial port of Arduino baud rate.
  Serial.print("I'm ready ");
  Serial.print("Hello?");
}

void loop(){
  if (GPRS.available()){         // if date is comming from softwareserial port ==> data is comming from gprs shield

    while(GPRS.available()){          // reading data into char array
      buffer[count++]=GPRS.read();     // writing data into array
      if(count == 64)break;
    }
  Serial.write(buffer,count);            // if no data transmission ends, write buffer to hardware serial port
  clearBufferArray();              // call clearBufferArray function to clear the storaged data from the array
  count = 0;                       // set counter of while loop to zero
}

if (Serial.available())            // if data is available on hardwareserial port ==> data is comming from PC or notebook
  GPRS.write(Serial.read());       // write it to the GPRS shield
  if(i == 0){
    GPRS.print("AT+CMGF=1\r"); //sending SMS in text mode
    Serial.println("AT+CMGF=1\r");
    delay(1000);
    GPRS.print("AT+CMGS=\"+CCyournumber"); // phone number
    Serial.println("AT+CMGS=\"+CCyournumber\"\r");
    delay(1000);
    GPRS.print("Test\r"); // message
    Serial.println("Test\r");
    delay(1000);
    GPRS.write(0x1A); //send a Ctrl+Z(end of the message)
    delay(1000);
    Serial.println("SMS sent successfully");
    i++;
  }   
}

void clearBufferArray(){              // function to clear buffer array
  for (int i=0; i<count;i++){
    buffer[i]=NULL;                  // clear all index of array with command NULL
  }
}
