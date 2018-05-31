
#include <LiquidCrystal.h>
//#include <Key.h>
#include <Keypad.h>

LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
const byte ROWS = 4; // Four rows
const byte COLS = 3; // Three columns

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

/*******************BuzzerPushButton******************************/
const int ledPin_stck1 = 12;
const int ledPin_stck2 = 13;
const int buzzer = 9; //buzzer to arduino pin 9
const int pushBtn = 11; // direct to 5V
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated
// constants won't change:
const long interval = 1000;           // interval at which to blink (milliseconds)
int state = LOW; 
int isButtonPressed = 0;
int weFinished = 0;
/*********************************************************************/
enum process_mindy
{
  initclk,
  alarmstk1,
  alarmstk2,
  monitorAlarm,
  SetAlarm1,
  SetAlarm2,
  SetbothAlarms,
  pillDispense,
  awayMode
};
int process_state;

enum
{
  E_NOTOK=0,
  E_OK,
  E_OKRunning
};

enum
{
  stack1,
  stack2
};

  //thhis is a list of int variables used in this clock program
bool dispclkflag;
int n = 0;
char realclock[6] = {};
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
int monitorAlarm_status;
int checkAlarm_status;
long alarm1_Hours = 0;
long alarm1_Mins = 0;
long alarm2_Hours = 0;
long alarm2_Mins = 0;
char key;
int resetAlarm;
int SetAlarm1_status;
int SetAlarm2_status;
int Stacknum;
int pillDispense_status;
int alarm_flag = 0;
int alarm1_flag = 0;
int alarm2_flag = 0;
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
      lcd.print("To reset press #"); 
      retval = E_NOTOK;           
   }
   return retval;
}
int getAlarm(int Alarm, char Alarm_buffer[])
{
     n=0;
    int retval= E_NOTOK;
    int clkflag = 1;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Alarm"+String(Alarm)+"(hhmm):");
    while(clkflag)
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
            clkflag=0;         
          }  
      }
    }
   return retval;
}
int getRealclk()
{
  int retval= E_NOTOK;
  int clkflag = 1;
  if( dispclkflag == false){
    while(clkflag)
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
            clkflag = 0;         
          }  
      }
    }
    }
   return retval;
}

int func_monitorAlarm()
{
  int retval1 = 0;
  int retval2 = 0;
  if((hours()== alarm1_Hours)&&(mins()==alarm1_Mins))
  {
    retval1 = 1;
    Serial.println("Stack1");
  }
  if((hours()== alarm2_Hours)&&(mins()== alarm2_Mins))
  {
    retval2 = 2;
    Serial.println("Stack2");
  }
  return (retval1+retval2);
}

int func_buzzerLED(int led)
{
    isButtonPressed = digitalRead(pushBtn);
    Serial.println("isButtonPressed"+String(isButtonPressed));
    delay(200); 
    if(!weFinished && isButtonPressed){
        state = LOW;
 //       noTone(buzzer);     // Stop sound...
        digitalWrite(led, state); 
        weFinished = 1;
    //    lcd.clear();
        return E_OK;
    }
    else if(!weFinished){
      
      unsigned long currentMillis = millis();
     
      if (currentMillis - previousMillis >= interval) {
        // save the last time you blinked the LED
        previousMillis = currentMillis;
    
        // if the LED is off turn it on and vice-versa:
        if (state == LOW) { 
          state = HIGH;
 //         tone(buzzer, 100000); // Send 1KHz sound signal...
          digitalWrite(led, state);   
        } 
        else {
          state = LOW;
//          noTone(buzzer);     // Stop sound...
          digitalWrite(led, state);     
        }
      }  
    }
  return E_OKRunning;
}

int func_pillDispense(int stack)
{
  return E_OK;
}
//int checkAlarm(int val)
//{
//  int retval=E_NOTOK; 
//  if(val == 1)
//  {
//     Serial.println("val == 1");
//     retval= func_buzzerLED(ledPin_stck1,val);
// //    digitalWrite(ledPin_stck1, HIGH);
//  }
//  else if(val == 2)
//  {
//     Serial.println("val == 2");
//     retval= func_buzzerLED(ledPin_stck2,val);
// //    digitalWrite(ledPin_stck2, HIGH);
//  }
//  else if(val == 3)
//  {
//    func_buzzerLED(ledPin_stck1,val);
//    func_buzzerLED(ledPin_stck2,val);
////    digitalWrite(ledPin_stck1, HIGH);
// //   digitalWrite(ledPin_stck2, HIGH);
//  }
//  else if(val == 0)
//  {
//    Serial.println("weFinished == 0");
//    weFinished = 0;
//  }
//  return retval;
//}

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
  monitorAlarm_status = 0;
  resetAlarm = 0;
  checkAlarm_status = E_NOTOK;
  SetAlarm1_status = E_NOTOK;
  dispclkflag = false;
  
/*******************BuzzerPushButton******************************/
  pinMode(pushBtn, INPUT);
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output
  pinMode(ledPin_stck1, OUTPUT);
  pinMode(ledPin_stck2, OUTPUT);
  
/*******************BuzzerPushButton******************************/
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
                            lcd.setCursor(0,0);
                            lcd.print("Time?(hhmm):"); 
                            n = 0;
                            process_state = initclk;
                           }                        
                       }
                      break;
      case alarmstk1: 
                       alrm_status1 = getAlarm(alarm1,alarm1_buffer);

                       if(alrm_status1 == E_OK)
                       {
                        lcd.clear();
                        if(resetAlarm == 0)
                        {
                          process_state = alarmstk2;
                        }
                        else
                        {
                          process_state = monitorAlarm;
                        }
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
                          
                          monitorAlarm_status = func_monitorAlarm();
                          Serial.println("monitorAlarm_status"+String(monitorAlarm_status));
                          Serial.println(monitorAlarm_status);
                    //      checkAlarm_status = checkAlarm(monitorAlarm_status);
//                           if(checkAlarm_status == E_OK)
//                           {
//                           // lcd.clear();
//                            Serial.println("Inside"); 
//                         // process_state = pillDispense;
//                            process_state = monitorAlarm;
//                           }
//                           else
//                           {
//                            process_state = monitorAlarm;
//                           } 

                           if((monitorAlarm_status == alarm1)&&(alarm1_flag==0))
                           {
                            weFinished =0;
                             Serial.println("alarm1");
                             process_state = SetAlarm1;
                           }
                           else if((monitorAlarm_status == alarm2)&&(alarm2_flag==0))
                           {
                            weFinished =0;
                             process_state = SetAlarm2;
                           }
                           else if((monitorAlarm_status == (alarm1+alarm2))&&(alarm_flag==0))
                           {
                            weFinished =0;
                             process_state = SetbothAlarms;
                           }
                           else
                           {
                             process_state = monitorAlarm;
                           }
                           
                           key = kpd.getKey();
                            if (key == '#')
                           { 
                            lcd.setCursor(0,0);
                            lcd.print("Which Alarm No?"); 
                            key = kpd.waitForKey(); 
                            if(key == '1')
                            {
                            process_state = alarmstk1;
                            resetAlarm = 1;
                            }
                            else if (key == '2')
                            {
                            process_state = alarmstk2;
                            resetAlarm = 1;
                            }
                            else
                            {
                            lcd.setCursor(0,0);
                            lcd.print("Wrong Alarm No");
                            }
                           }   
                          break;

      case SetAlarm1    : SetAlarm1_status = func_buzzerLED(ledPin_stck1);
                          Serial.println("SetAlarm1_status"+String(SetAlarm1_status));
                          if(SetAlarm1_status == E_OK)
                          {
                          //  process_state = pillDispense;
                           Serial.println("Dispense Stck1");
                           process_state = pillDispense;
                           Stacknum = stack1;
                           alarm1_flag = 1;
                          }
                          else
                          {
                            process_state = SetAlarm1;
                          }
                          break;

      case SetAlarm2    : SetAlarm2_status = func_buzzerLED(ledPin_stck2);
                          Serial.println("SetAlarm2_status"+String(SetAlarm2_status));
                          if(SetAlarm2_status == E_OK)
                          {
                          //  process_state = pillDispense;
                           Serial.println("Dispense Stck2");
                           process_state = pillDispense;
                           Stacknum = stack2;
                           alarm2_flag = 1;
                          }
                          else
                          {
                            process_state = SetAlarm2;
                          }
                          break;
      case pillDispense :                          
                          pillDispense_status = func_pillDispense(Stacknum);
                          //lcd.clear();
                          lcd.setCursor(0,1);
                          lcd.print("Collect Pill");
                          delay(200);
                          if(pillDispense_status == E_OK)
                          {
                            process_state = monitorAlarm;
                          }
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
    lcd.setCursor(0,0);
    lcd.print("Time:");
    lcd.setCursor(6,0);
    printDigits(hours());
    sep();
    printDigits(mins());
   // sep();
   // printDigits(secs());
}
