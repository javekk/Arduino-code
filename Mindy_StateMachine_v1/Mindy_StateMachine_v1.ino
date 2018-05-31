
#include <LiquidCrystal.h>
//#include <Key.h>
#include <Keypad.h>

LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
const byte ROWS = 4; // Four rows

const byte COLS = 3; // Three columns

 int n = 0;
 
char realclock[6] = {};
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
enum process_mindy
{
  initclk,
  alarmstk1,
  alarmstk2,
  monitorAlarm,
  pillDispense,
  awayMode
};
int process_state;

enum
{
  E_NOTOK=0,
  E_OK
};

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
int clk_status;
int alarm1 = 1;
int alarm2 = 2;
char alarm1_buffer[6] = {};
char alarm2_buffer[6] = {};
int alrm_status1;
int alrm_status2;
long alarm1_Hours = 0;
long alarm1_Mins = 0;
long alarm2_Hours = 0;
long alarm2_Mins = 0;
char key;
int getHr(char buffer[])
{
  hr = ((buffer[0]-'0')*10) + (buffer[1]-'0');
  return hr;
}
int getMin(char buffer[])
{
  minute = (buffer[2]-'0')*10 + (buffer[3]-'0');
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
int checkclktime()
{
   int retval = E_NOTOK;
   int error = false;
   switch(process_state)
   {
    case initclk :    initialHours = getHr(realclock);
                      initialMins = getMin(realclock);
                       if((initialHours<=24)&&(initialMins<=60))
                       {
                         dispclkflag = true;  
                         lcd.clear();
                         retval = E_OK;
                       }
                       else
                       {
                        error = true;
                       }
                       break;
    case alarmstk1:    alarm1_Hours = getHr(alarm1_buffer);
                       alarm1_Mins = getMin(alarm1_buffer);
                       if((alarm1_Hours<=24)&&(alarm1_Mins<=60))
                       {
                        // dispclkflag = true;  
                         lcd.clear();
                         retval = E_OK;
                       }
                       else
                       {
                        error = true;
                       }
                       break;
    case alarmstk2:    alarm2_Hours = getHr(alarm2_buffer);
                       alarm2_Mins = getMin(alarm2_buffer);
                       if((alarm2_Hours<=24)&&(alarm2_Mins<=60))
                       {
                        // dispclkflag = true;  
                         lcd.clear();
                         retval = E_OK;
                       }
                       else
                       {
                        error = true;
                       }
                       break;
   }

  if(error)
   {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Incorrect Time");
      lcd.setCursor(0,1);
      lcd.print("Please reset"); 
      retval = E_NOTOK;           
   }
   return retval;
}
int getAlarm(int Alarm, char Alarm_buffer[])
{
     n=0;
    int retval= E_NOTOK;
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
            Alarm_buffer[n] = ' ';
            Serial.println(Alarm_buffer);
          }
          else if (key != '#' && key != '*' && n!=5) {
             lcd.print(key);
             Alarm_buffer[n] = key;
             n++; 
             Serial.println(Alarm_buffer);
          } 
          else if(key == '*'){
            retval = checkclktime();            
          }  
      }
   return retval;
}
int getRealclk()
{
  int retval= E_NOTOK;
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
            realclock[n] = ' ';
            Serial.println(realclock);
          }
          else if (key != '#' && key != '*' && n!=5) {
             lcd.print(key);
             realclock[n] = key;
             n++; 
             Serial.println(realclock);
          } 
          else if(key == '*'){
            retval = checkclktime();            
          }  
      }
    }
   return retval;
}

void setup() { 
  Serial.begin(9600);
  delay(1000);
  lcd.begin(16, 2);  
  lcd.setCursor(0,0);
  lcd.print("Time?(hhmm):");
  process_state = initclk;
  clk_status = E_NOTOK;
  alrm_status1 = E_NOTOK;
  alrm_status2 = E_NOTOK;
  dispclkflag = false;
//  lcd.print("May, 5, 2018"); // print out the date
}

//this loop will conintue to keep looping so that the time can go as follows
void loop(){

    if( dispclkflag == true){
      digitalClockDisplay();
     }
  
    switch(process_state)
    {
      case initclk:  
                       clk_status = getRealclk();
                       if(clk_status == E_OK){
                        process_state = alarmstk1;
                       }
                       else
                       {
                         key = kpd.waitForKey(); //storing pressed key value in a char
                         if (key == '#')
                           { 
                            lcd.clear();
                            process_state = initclk;
                           }                        
                       }
                      break;
      case alarmstk1: 
                       alrm_status1 = getAlarm(alarm1,alarm1_buffer);

                       if(alrm_status1 == E_OK)
                       {
                        lcd.clear();
                        process_state = alarmstk2;
                       }
                       else
                       {
                         key = kpd.waitForKey(); //storing pressed key value in a char
                         if (key == '#')
                           { 
                            lcd.clear();
                            process_state = initclk;
                           }      
                       }      
                      break;
      case alarmstk2: 
                       alrm_status2 = getAlarm(alarm2,alarm2_buffer);
                       
                       if(alrm_status2 == E_OK)
                       {
                        lcd.clear();
                        process_state = monitorAlarm;
                       }
                       else
                       {
                         key = kpd.waitForKey(); //storing pressed key value in a char
                         if (key == '#')
                           { 
                            lcd.clear();
                            process_state = initclk;
                           }      
                       } 
                       break;
      case monitorAlarm : 
                          //monitorAlarm();
                        break;

      case pillDispense :
                          //pilldispense();
                        break;

      case awayMode :
                      //awaymode(); 
                        break;
                        
      default :        break;
      
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
