#include <Key.h>
#include <Keypad.h>


#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

// GSM with arduino UNO connection
// Arduino pin 12 with GSM Tx
// Arduino pin 13 with GSM Rx
// Arduino gnd with GSM gnd
// GSM gnd and 5V with serial to USB converter gnd and 5V respectively



// initialize the library with the numbers of the interface pins
SoftwareSerial SIM900(12, 13); //12 -> tx 13 -> rx

LiquidCrystal lcd(A0,A1,A2,A3,A4,A5);

const byte ROWS = 4; // Four rows

const byte COLS = 3; // Three columns

 int n = 0;
 
char number[16] = {};

// Define the Keymap

char keys[ROWS][COLS] = {

  {'1','2','3'},

  {'4','5','6'},

  {'7','8','9'},

  {'*','0','#'}

};

// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
// SEX: connect pin 1 to key pad to pin 2 arduino

byte rowPins[ROWS] = { 8, 7, 6, 5 };

// Connect keypad COL0, COL1 and COL2 to these Arduino pins.

byte colPins[COLS] = { 4, 3, 2 };

//  Create the Keypad

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup()

{
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("KEYPAD LCD TEST"); 
  SIM900.begin(19200);
}

void loop()

{

  char key = kpd.waitForKey(); //storing pressed key value in a char

  if (key != NO_KEY)

  {  
        Serial.println(key);
        // set the cursor to column 0, line 1
        // (note: line 1 is the second row, since counting begins with 0):
        lcd.setCursor(n, 1);
        // print the number of seconds since reset:
        if(key == '#' && n != 0){
          n--;
          lcd.setCursor(n, 1);
          lcd.print(' ');
          number[n] = ' ';
          Serial.println(number);
        }
        else if (key != '#' && key != '*' && n!=16) {
           lcd.print(key);
           number[n] = key;
           n++; 
           Serial.println(number);
        } 
        else if(key == '*'){
           sendSMS();
        }
        
    }

}                


void sendSMS() {
  // AT command to set SIM900 to SMS mode
  SIM900.print("AT+CMGF=1\r"); 
  delay(100);

  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  String tmp1 = "AT + CMGS = \"+39";
  String tmp2 = number;
  String tmp3 = "\";";
  String tmp = tmp1 + tmp2 + tmp3;
  SIM900.println(tmp); 
  delay(100);
  Serial.println(tmp);
  // REPLACE WITH YOUR OWN SMS MESSAGE CONTENT
  SIM900.println("COGLIONE"); 
  delay(100);

  // End AT command with a ^Z, ASCII code 26
  SIM900.println((char)26); 
  delay(100);
  SIM900.println();
  // Give module time to send SMS
  delay(5000); 
}


