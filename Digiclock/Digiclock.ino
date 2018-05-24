
#include <LiquidCrystal.h>
//#include <Key.h>
#include <Keypad.h>

LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
const byte ROWS = 4; // Four rows

const byte COLS = 3; // Three columns

 int n = 0;
 
char number[6] = {};
bool dispclkflag;
// Define the Keymap

char keys[ROWS][COLS] = {

  {'1','2','3'},

  {'4','5','6'},

  {'7','8','9'},

  {'*','0','#'}

};

byte rowPins[ROWS] = { 8, 7, 6, 5 };
byte colPins[COLS] = { 4, 3, 2 };

//  Create the Keypad

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() { 
  Serial.begin(9600);
  delay(1000);
  lcd.begin(16, 2);  
  lcd.setCursor(0,0);
  lcd.print("Time?(hhmm):");
  dispclkflag = false;
//  lcd.print("May, 5, 2018"); // print out the date
}
  //thhis is a list of int variables used in this clock program
long s=0;
long sec=0;
long hrs=0;
long minutes=0;
long initialHours = 00;//variable to initiate hours
long initialMins = 0;//variable to initiate minutes
long initialSecs = 00;//variable to initiate seconds
long minute=0;
long hr=0;

int getHr()
{
  hr = ((number[0]-'0')*10) + (number[1]-'0');
  return hr;
}
int getMin()
{
  minute = (number[2]-'0')*10 + (number[3]-'0');
  return minute;
}
//this method is for seconds
int seconds()
{
    s = initialHours*3600;  
    s = s+(initialMins*60);
    s = s+initialSecs;
    s = s+(millis()/1000); 
    return s;
}
//this method is for hours
int hours()
{
    hrs = seconds();
    if(initialHours>9)
    {
      hrs = hrs+65536;
    }
    hrs = hrs/3600;
    hrs = hrs%24;
    return hrs;
}
//this method is for minutes
int mins()
{
    minutes = seconds();
    if(initialHours>9)
    {
      minutes = minutes+65536;
    }
    minutes = minutes/60;
    minutes = minutes%60;
    return minutes;
}

int secs()
{
    sec = seconds();
    if(initialHours>9)
    {
      sec = sec+65536;
    }
    sec = sec%60;
    return sec;
}
//this loop will conintue to keep looping so that the time can go as follows
void loop(){
  if( dispclkflag == false){
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
        else if (key != '#' && key != '*' && n!=7) {
           lcd.print(key);
           number[n] = key;
           n++; 
           Serial.println(number);
        } 
        else if(key == '*'){
           initialHours = getHr();
           initialMins = getMin();
           if((initialHours<=24)&&(initialMins<=60))
           {
           dispclkflag = true;  
           lcd.clear();
           }
           else
           {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Incorrect Time");
            lcd.setCursor(0,1);
            lcd.print("Please reset");
            
           }
        }
        
    }
  }
if( dispclkflag == true){
    digitalClockDisplay();
}

}
void printDigits(byte digits){
    if(digits < 10)
        lcd.print('0');
    lcd.print(digits);
}
char sep()
{
    s = millis()/1000;
    if(s%2==0)
    {
        lcd.print(":");
    }
    else {
        lcd.print(" ");
    }  
}
void digitalClockDisplay(){
    lcd.setCursor(0,1);
    printDigits(hours());
    sep();
    printDigits(mins());
   // sep();
   // printDigits(secs());
}
