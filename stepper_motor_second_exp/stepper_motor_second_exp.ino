#include <Key.h>
#include <Keypad.h>


#include <LiquidCrystal.h>


#include <Keypad.h>//header for keypad commands enabling

// initialize the library with the numbers of the interface pins

 LiquidCrystal lcd(A0,A1,A2,A3,A4,A5);

const byte ROWS = 4; // Four rows

const byte COLS = 3; // Three columns

// Define the Keymap

char keys[ROWS][COLS] = {

  {'1','2','3'},

  {'4','5','6'},

  {'7','8','9'},

  {'*','0','#'}

};

// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.

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
}

void loop()

{

  char key = kpd.getKey(); //storing pressed key value in a char

  if (key != NO_KEY)

  {
        Serial.println(key);
        // set the cursor to column 0, line 1
        // (note: line 1 is the second row, since counting begins with 0):
        lcd.setCursor(0, 1);
        // print the number of seconds since reset:
        lcd.print(key);


    }

}                

