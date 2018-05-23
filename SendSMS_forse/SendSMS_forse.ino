/*
 SMS sender
 
 This sketch, for the Arduino GSM shield,sends an SMS message 
 you enter in the serial monitor. Connect your Arduino with the 
 GSM shield and SIM card, open the serial monitor, and wait for 
 the "READY" message to appear in the monitor. Next, type a 
 message to send and press "return". Make sure the serial 
 monitor is set to send a newline when you press return.
 
 Circuit:
 * GSM shield 
 * SIM card that can send SMS
 
 created 25 Feb 2012
 by Tom Igoe
 
 This example is in the public domain.
 
 http://arduino.cc/en/Tutorial/GSMExamplesSendSMS
 
 */

// Include the GSM library
#include <GSM.h>
// initialize the library instance
GSM gsmAccess;
GSM_SMS sms;

void setup()
{
  // initialize serial communications and wait for port to open:
   
  Serial.println("Wait");
  Serial.begin(19200);
 delay(20000);   
  
  Serial.println("SMS Messages Sender");

}

void loop()
{

  Serial.print("Enter a mobile number: ");
  char remoteNum[20];  // telephone number to send sms
  readSerial(remoteNum);
  Serial.println(remoteNum);
    
  // sms text
  Serial.print("Now, enter SMS content: ");
  char txtMsg[200];
  readSerial(txtMsg);
  Serial.println("SENDING");
  Serial.println();
  Serial.println("Message:");
  Serial.println(txtMsg);
  
  // send the message
  sms.beginSMS(remoteNum);
  sms.print(txtMsg);
  sms.endSMS(); 
  Serial.println("\nCOMPLETE!\n");
}

/*
  Read input serial
 */
int readSerial(char result[])
{
  int i = 0;
  while(1)
  {
    while (Serial.available() > 0)
    {
      char inChar = Serial.read();
      if (inChar == '\n')
      {
        result[i] = '\0';
        Serial.flush();
        return 0;
      }
      if(inChar!='\r')
      {
        result[i] = inChar;
        i++;
      }
    }
  }
}
