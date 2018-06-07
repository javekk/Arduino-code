#include <LiquidCrystal.h>
#include <Keypad.h>
//#include <SoftwareSerial.h>
#include<Wire.h>


/********************************/
/*  ____    ___   _   _         */
/* |  _ \  |_ _| | \ | |  ___   */
/* | |_) |  | |  |  \| | / __|  */
/* |  __/   | |  | |\  | \__ \  */
/* |_|     |___| |_| \_| |___/  */
/********************************/




LiquidCrystal lcd(A0, A1, A2, A3, 12, 13); 


const byte COLS = 3; // Three columns
const byte ROWS = 4; // Four rows
byte colPins[COLS] = { 4, 3, 2 };
byte rowPins[ROWS] = { 8, 7, 6, 5 };



const int pushBtn = 11; // direct to 5V // changing from 11 to 0
const int awayModePushBtn=10; // direct to 5V // changing from 10 to 1
const int ledPin_stck1 = 9;  //12
const int ledPin_stck2 = 9; //13



/************************************************************************************************************/
/*   ____   _           _               _    __     __                 _           _       _                */
/*  / ___| | |   ___   | |__     __ _  | |   \ \   / /   __ _   _ __  (_)   __ _  | |__   | |   ___   ___   */
/* | |  _  | |  / _ \  | '_ \   / _` | | |    \ \ / /   / _` | | '__| | |  / _` | | '_ \  | |  / _ \ / __|  */
/* | |_| | | | | (_) | | |_) | | (_| | | |     \ V /   | (_| | | |    | | | (_| | | |_) | | | |  __/ \__ \  */
/*  \____| |_|  \___/  |_.__/   \__,_| |_|      \_/     \__,_| |_|    |_|  \__,_| |_.__/  |_|  \___| |___/  */
/************************************************************************************************************/


/**************************/
/*********keypad***********/
/**************************/

char keys[ROWS][COLS] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}
};
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS ); //Mapping pins with keys or viceversa as you prefer
char key;   // the input of the key


/*******************************/
/********PushButton*************/
/*******************************/

unsigned long previousMillis = 0;        // will store last time LED was updated
// constants won't change:
const long interval = 1000;           // interval at which to blink (milliseconds)
int state = LOW; 
int isButtonPressed = 0;   //the button of the blinking
int isAwayModePressed = 0;  //the button of the away mode
int weFinished = 0;        //we finished to blink
int val = 0;      //used inside time addition of away mode

int timer = 0; //to calculate 60 seconds for time out to send SMS if button not pressed

/***********************/
/********Enums ? *******/
/***********************/

//states
enum process_mindy {
    initclk,
    mobileNumber,
    alarmstk1,
    alarmstk2,
    monitorAlarm,
    SetAlarm1,
    SetAlarm2,
    SetbothAlarms,
    pillDispense,
    awayMode, 
    awayModeMonitorAlarm,
    awayModeDispensePill
};

// return values of some functions 
enum {
    E_NOTOK=0,
    E_OK,
    E_OKRunning,
    E_Timer // added for new timer function
};

enum {
    stack1, 
    stack2
};

int process_state; //current state of the machine



/****************************/
/*********GLobal Time********/
/****************************/

bool dispclkflag; //Display Clock Flag, to check if the input clok time is correct
int n = 0; //Counter 
char realclock[6] = {}; //Take the input HHMM 
long s=0;  //calculationg final second
long sec=0;
long hrs=0;

long minutes=0;
long initialHours = 0; //variable to initiate hours
long initialMins = 0; //variable to initiate minutes
long initialSecs = 0; //variable to initiate seconds
long minute=0; //
long hr=0; //
int clk_status; //getting the state of the initial clock


/****************************/
/*********Set up************/
/****************************/

int alrm_status1; // contains the state of the get alarm function 1
int alrm_status2; // same as above

/****************************/
/*********Allarms************/
/****************************/

const int alarm1 = 1; // const to associate alarm to stack 1 
const int alarm2 = 2; // same as above
char alarm1_buffer[6] = {}; // take in input the alarm 1
char alarm2_buffer[6] = {}; // same as above
int monitorAlarm_status; // tells us which is the stack of the alarm
int awayModeMonitorAlarm_status;  // tells us which is the stack of the alarm for away mode
long alarm1_Hours = 0; 
long alarm1_Mins = 0;
long alarm2_Hours = 0;
long alarm2_Mins = 0;

bool resetAlarm; // holds if we want to set the stacks
int SetAlarm1_status; 
int SetAlarm2_status;
int SetBothAlarm_status;
int Stacknum =0; // holds the value of the stack that needs to dispence the pill

bool alarm_flag = 0; // both allarms in the same time 
bool alarm1_flag = 0; // if it only alarm 1 
bool alarm2_flag = 0; // if it only alarm 2


int checkAlarm_status;

/****************************/
/*********Others************/
/****************************/


int pillDispense_status;


//SoftwareSerial SIM900(0, 1); //GSM
char number[16] = {};
char awayHour = 'f';
bool enddd = false; //this is used in the input number function
int nnnnnnn = 0;  //this is used in the input number function
int nn = 0; //used for input of away hours
int alarmCount_Stack1 = 0; //count of alarms for away mode
int alarmCount_Stack2 = 0; //count of alarms for away mode
int count1 = 0;// counter for away mode to dispense pill
int count2 = 0;// counter for away mode to dispense pill
/**********************************************/
/*  ____    _____   _____     _   _   ____    */
/* / ___|  | ____| |_   _|   | | | | |  _ \   */
/* \___ \  |  _|     | |     | | | | | |_) |  */
/*  ___) | | |___    | |     | |_| | |  __/   */
/* |____/  |_____|   |_|      \___/  |_|      */
/**********************************************/                                            

void setup(){
    
    Serial.begin(9600);
    delay(1000);
    lcd.begin(16, 2);  
    lcd.setCursor(0,0);
    lcd.print("Time?(hhmm):");
    process_state = initclk; //go to the first state
    clk_status = E_NOTOK;
    alrm_status1 = E_NOTOK;
    alrm_status2 = E_NOTOK;
    monitorAlarm_status = 0;
    resetAlarm = 0;
    checkAlarm_status = E_NOTOK;
    SetAlarm1_status = E_NOTOK;
    awayModeMonitorAlarm_status = 0;
    dispclkflag = false;

    Wire.begin();
   // SIM900.begin(19200);
    
    pinMode(pushBtn, INPUT); // set the PushButton for the pill dispening 
   // pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output
    pinMode(ledPin_stck1, OUTPUT); // led for the first stack
    pinMode(ledPin_stck2, OUTPUT); // led for the second stack
    pinMode(awayModePushBtn, INPUT); // set the PushButton for the away mode
    
}


/************************************/
/*  _        ___     ___    ____    */
/* | |      / _ \   / _ \  |  _ \   */
/* | |     | | | | | | | | | |_) |  */
/* | |___  | |_| | | |_| | |  __/   */
/* |_____|  \___/   \___/  |_|      */
/************************************/                               

void loop(){
    
    /*
     * if the clock time is display correctly it display the time
     */
    if( dispclkflag == true){
        digitalClockDisplay();
    }
    
    /*
     
     */
    switch(process_state){
        
        
        
        /*STATE: INITIAL SET_UP******/
        /*********INPUT TIME*********/
        /****************************/
        case initclk:  
            clk_status = getRealclk();
            if(clk_status == E_OK){
                process_state = mobileNumber;                       
            }
            else{
                
                key = kpd.waitForKey(); //storing pressed key value in a char
                if (key == '#'){ 
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("Time?(hhmm):"); 
                    n = 0;
                    process_state = initclk;
                }                        
            }
            
            break;
            
            
            /*STATE: INITIAL SET_UP******/
            /*Setup the number*********/
            /****************************/
            
        case mobileNumber:        
          lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Insert number:");
          while(!enddd){
            Serial.println("Inside mobileNumber");
            key = kpd.waitForKey(); //storing pressed key value in a char
            
          if (key != NO_KEY)  {  
                Serial.println(key);
                // set the cursor to column 0, line 1
                // (note: line 1 is the second row, since counting begins with 0):
                lcd.setCursor(nnnnnnn, 1);
                // print the number of seconds since reset:
                if(key == '#' && nnnnnnn != 0){
                  nnnnnnn--;
                  lcd.setCursor(nnnnnnn, 1);
                  lcd.print(' ');
                  number[nnnnnnn] = ' ';
                  Serial.println(number);
                }
                else if (key != '#' && key != '*' && nnnnnnn!=16) {
                   lcd.print(key);
                   number[nnnnnnn] = key;
                   nnnnnnn++; 
                   Serial.println(number);
                } 
                else if(key == '*'){
                   lcd.clear();
                   lcd.setCursor(0,0);
                   lcd.print("# to reset");
                   lcd.setCursor(0,1);
                   lcd.print("any key, ok");
                  
                   
                  //sendSMS(number, "Mindy mobile number setup");
                 
                  
                   key = kpd.waitForKey(); //storing pressed key value in a char
                   if(key == '#'){
                     lcd.clear();
                     lcd.setCursor(0,0);
                     lcd.print("Insert number:");
                     nnnnnnn = 0;
                   }
                   else{
                      enddd = true;
                      process_state = alarmstk1; 
                   }
                }
                
            }
            
            
          }
        
        break;
            
            
            
        /*STATE: SET UP ALLARMS******/
        /*set up the alarm for the **/
        /*first stack****************/
        /****************************/    
        case alarmstk1: 
            alrm_status1 = getAlarm(alarm1,alarm1_buffer);
            alarm1_flag = 0;
            if(alrm_status1 == E_OK){
                Serial.println("new alarm");
                lcd.clear();
                if(resetAlarm == 0){
                    process_state = alarmstk2;
                }
                else{
                    process_state = monitorAlarm;
                }
            }
            else{
                key = kpd.waitForKey(); //storing pressed key value in a char
                if (key == '#'){ 
                    lcd.clear();
                    process_state = initclk;
                }      
            }      
            break;
            
            
            
        /*STATE: SET UP ALLARMS******/
        /*set up the alarm for the **/
        /*second stack***************/
        /****************************/
        case alarmstk2: 
            alrm_status2 = getAlarm(alarm2,alarm2_buffer);
            alarm2_flag = 0;
            if(alrm_status2 == E_OK) {
                lcd.clear();
                process_state = monitorAlarm;
            }
            else{
                key = kpd.waitForKey(); //storing pressed key value in a char
                if (key == '#'){ 
                    lcd.clear();
                    process_state = initclk;
                }      
            } 
            break;
            
        /*STATE: MONITOR ALARMS******/
        /****************************/
        /****************************/    
        case monitorAlarm : 

            isAwayModePressed = digitalRead(awayModePushBtn);
            
            if (isAwayModePressed==0){           
           Serial.println ("isAwayModePressed: "+String(isAwayModePressed));
            monitorAlarm_status = func_monitorAlarm();
            Serial.println("monitorAlarm_status: "+String(monitorAlarm_status));
            Serial.println(monitorAlarm_status);
    
            if((monitorAlarm_status == alarm1)&&(alarm1_flag==0)) {
                weFinished =0;
                
                process_state = SetAlarm1;
            }
            else if((monitorAlarm_status == alarm2)&&(alarm2_flag==0)){
                weFinished =0;
                process_state = SetAlarm2;
            }
            else if((monitorAlarm_status == (alarm1+alarm2))&&(alarm_flag==0)){
                weFinished =0;
                process_state = SetbothAlarms;
            }
            
            else{
                process_state = monitorAlarm;
            }
            }
             else{
               Serial.println ("isAwayModePressed: "+String(isAwayModePressed));
              process_state = awayMode;
             }
            
            key = kpd.getKey();
            if (key == '#'){
                
                lcd.setCursor(0,0);
                lcd.print("Which Alarm No?"); 
                key = kpd.waitForKey(); 
                
                if(key == '1'){
                    process_state = alarmstk1;
                    resetAlarm = 1;
                }
                else if (key == '2'){
                    process_state = alarmstk2;
                    resetAlarm = 1;
                }
                else{
                    lcd.setCursor(0,0);
                    lcd.print("Wrong Alarm No");
                }
            }   
            break;
            
            
            /*STATE: ALARM TIME******/
            /*SET, because we have to change*/
            /*the state of the alarm one to 1*/
            /****************************/  
            case SetAlarm1    : 
            
            SetAlarm1_status = func_buzzerLED(ledPin_stck1, -1);
            Serial.println("SetAlarm1_status: "+String(SetAlarm1_status));
            if(SetAlarm1_status == E_OK){
                //  process_state = pillDispense;
                Serial.println("Dispense Stack1");
                process_state = pillDispense;
                Stacknum = 1;
                Serial.println("Stacknum 1: "+Stacknum);
                alarm1_flag = 1;
            }
            else if (SetAlarm1_status == E_Timer){
                process_state = monitorAlarm; //added to return from process state 'SMS via GSM'
            }
             else {
                process_state = SetAlarm1; 
            }
            break;
            
            
            /*STATE: ALARM TIME******/
            /*SET, because we have to change*/
            /*the state of the alarm two to 1*/
            /****************************/ 
            case SetAlarm2    : 
            
            SetAlarm2_status = func_buzzerLED(ledPin_stck2, -1);
            Serial.println("SetAlarm2_status"+String(SetAlarm2_status));
            if(SetAlarm2_status == E_OK){
                //  process_state = pillDispense;
                Serial.println("Dispense Stack2");
                process_state = pillDispense;
                Stacknum = 2;
                Serial.println("Stacknum 2 : "+Stacknum);
                alarm2_flag = 1;
            }
             else if (SetAlarm2_status == E_Timer){
              
                process_state = monitorAlarm; //added to return from process state 'SMS via GSM'
            }
            else{
                process_state = SetAlarm2;
            }
            break;
            
            
             /*STATE: ALARM TIME******/
            /*SET, because we have to change*/
            /*the state of the alarm two to 1*/
            /****************************/ 
            case SetbothAlarms    : 
            
                SetBothAlarm_status = func_buzzerLED(ledPin_stck1, ledPin_stck2);
                
                Serial.println("SetBoth_status: 1= "+String(SetAlarm1_status) + "2=" + String(SetAlarm2_status));
                 
                if(SetBothAlarm_status == E_OK){
                    //  process_state = pillDispense;
                    Serial.println("Dispense Stack1 + Stack2");
                    process_state = pillDispense;
                    Stacknum = 3;
                    Serial.println("Stacknum both: "+Stacknum);
                    alarm_flag = 1;
                }
                else if (SetBothAlarm_status == E_Timer){
              
                process_state = monitorAlarm; //added to return from process state 'SMS via GSM'
                }
                
                else {
                    process_state = SetbothAlarms;
                } 
                 
            
            break;
            
            /*STATE: DISPENSE PILL******/
            /*DISPENCE THE PILL*/
            /****************************/ 
            case pillDispense :                         
               pillDispense_status = func_pillDispense();
                //lcd.clear();
                Serial.println("Stacknum: "+Stacknum);
                Wire.beginTransmission(9); // transmit to device #9
                Wire.write(Stacknum);    
                Serial.println("Stacknum: "+Stacknum);
                // sends x 
                Wire.endTransmission();
                
                lcd.setCursor(0,1);
                lcd.print("Collect Pill");
                delay(2000);
              
                
                if(pillDispense_status == E_OK){
                    lcd.setCursor(0,1);
                    lcd.print("                ");
                    process_state = monitorAlarm;
                }
                //pilldispense();
                
                break;
            
        
            /*STATE: AWAY MODE******/
            /**/
            /****************************/     
            case awayMode :
                  //awaymode(); 
  
  
                  Serial.println("Away mode is ON");
                   
                 
                  
                   lcd.clear();
                   lcd.setCursor(0,0);
                  lcd.print("Away hours(1-4)?"); 
  
                  lcd.setCursor(0,1);
                  if(awayHour!= 'f') lcd.print(awayHour);
                  key = kpd.waitForKey(); 
                      
                if (key != NO_KEY)  {  
                  
                      Serial.println(key);
                      
                     // lcd.setCursor(0,1);
                    
                      if(key == '#' ){  
                      //  lcd.setCursor(0,1);                                                
                        lcd.print(' ');
                        awayHour = ' ';
                        Serial.println(awayHour);
                      }
                      else if (key != '#' && key != '*') {  
                       // lcd.setCursor(0,1);                     
                         lcd.print(key);                      
                         awayHour = key;  
                         int someInt = awayHour - '0'; // converting char to int               
                         val = hours()+someInt;
                         
                      } 
                      else if(key == '*'){
                         lcd.clear();
                         lcd.setCursor(0,0);
                         lcd.print("# to turn off");                  
                         lcd.setCursor(0,1);
                         lcd.print("any key, ok"); 
                       
                         key = kpd.waitForKey(); //storing pressed key value in a char
                         if(key == '#'){
                           lcd.clear();
                           nn = 0;
                           isAwayModePressed =0;                         
                           process_state=monitorAlarm;
                           break;                          
                         }
                         
                        
                         //sendSMS(number, "Away Mode is ON");
                        process_state=awayModeMonitorAlarm;
                      }
                }
  
                  
                  break;

            /*STATE: Away mode monitor alarms******/
            /*Check alarms for away mode, if any**/
            /****************************/

           case awayModeMonitorAlarm :
                    
                      Serial.println("Dispense pills for away mode");
                      
                      
                       awayModeMonitorAlarm_status = func_awayModeMonitorAlarm(val);

                      // Serial.println("alarm 1 flag value"+String(alarm1_flag));
                       
                      if((awayModeMonitorAlarm_status == alarm1)) {//&&(alarm1_flag==0)
                                alarmCount_Stack1++;
                                // Serial.println("alarm 1 flag value"+String(alarm1_flag));
                                Serial.println("Here is alarm1");
                              //  Serial.println(alarmCount_Stack1);
                                process_state = awayModeDispensePill;
                                lcd.clear();
                      }
                      else if(awayModeMonitorAlarm_status == alarm2){//&&(alarm2_flag==0
                              alarmCount_Stack2++;
                              
                               Serial.println("Here is alarm2");
                            //   Serial.println(alarmCount_Stack2);
                              process_state = awayModeDispensePill;
                              lcd.clear();
                     }
                     else if((awayModeMonitorAlarm_status == (alarm1+alarm2))){//&&(alarm_flag==0)
                             alarmCount_Stack1++;  
                             alarmCount_Stack2++;    
                            process_state = awayModeDispensePill;
                            lcd.clear();
                      }
                    
                    else{
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("No Pills to take");
                        delay(7000);
                        lcd.clear();
                        process_state = monitorAlarm;
                    }
                      
                  break;

             /*STATE: Away mode dispense pills******/
            /*Dispense pills for alarms for away mode, if any*/
            /****************************/

          case awayModeDispensePill :

                   //Serial.println("Away mode dispense pills for alarms");
                      
                    
                   
                    lcd.setCursor(0,1);
                    lcd.print("Away mode is ON");
                   
                    if(alarmCount_Stack1!=0 && alarmCount_Stack2!=0 && count1==0 && count2 ==0){
                      Stacknum=3;
                      count1++;
                      count2++;

                            Serial.println("Stacknum: "+Stacknum);
                            Wire.beginTransmission(9); // transmit to device #9
                            Wire.write(Stacknum);    
                            Serial.println("Stacknum: "+Stacknum);
                            // sends x 
                            Wire.endTransmission();
                            
                           
                      
                      
                      Serial.println("Dispense pills for the alarm Stack 1 and Stack 2");
                    }
                     if((count1!=alarmCount_Stack1)&&(alarmCount_Stack1!=0)){
                      Stacknum=1;
                     


                          Serial.println("Stacknum: "+Stacknum);
                          Wire.beginTransmission(9); // transmit to device #9
                          Wire.write(Stacknum);    
                          Serial.println("Stacknum: "+Stacknum);
                          
                          Wire.endTransmission();
                          
                          

                
                      count1++;
                      
                     Serial.println("Dispense pills for the alarm Stack 1");
                      
                    }
                    else if ((count2!=alarmCount_Stack2)&&(alarmCount_Stack2!=0)){
                      Stacknum=2;
                   

                          Serial.println("Stacknum: "+Stacknum);
                          Wire.beginTransmission(9); // transmit to device #9
                          Wire.write(Stacknum);    
                          Serial.println("Stacknum: "+Stacknum);
                          // sends x 
                          Wire.endTransmission();
                          
                     
                
                      count2++;
                     
                    Serial.println("Dispense pills for the alarm Stack 2");
                    }
    
                    else{
                      
                     process_state = awayModeDispensePill;
                    /*   if ((hours()==alarm1_Hours)&&(mins()==alarm1_Mins)){
                        sendSMS(number,"Alarm! Take one pill from Stack 1");
                       }
                       else if ((hours()==alarm2_Hours)&&(mins()==alarm2_Mins)){
                        sendSMS(number,"Alarm! Take one pill from Stack 2");
                       }
                     */
                      
                      if (hours()>=val){
                        
                        
                         Serial.println("Away Mode is OFF now");
                         lcd.clear();
                         count1 =0;
                         alarmCount_Stack1 =0;
                         count2 =0;
                         alarmCount_Stack2 =0;
                         process_state = monitorAlarm;
                      }
                     
                    }
                    
                    break;

              
                
            default :        break;
            
    }
}



/**************************************************************************/
/*  _____   ___   __  __   _____     _____   _   _   _   _    ____        */
/* |_   _| |_ _| |  \/  | | ____|   |  ___| | | | | | \ | |  / ___|  ___  */
/*   | |    | |  | |\/| | |  _|     | |_    | | | | |  \| | | |     / __| */
/*   | |    | |  | |  | | | |___    |  _|   | |_| | | |\  | | |___  \__ \ */
/*   |_|   |___| |_|  |_| |_____|   |_|      \___/  |_| \_|  \____| |___/ */                                                                      
/**************************************************************************/



/*in the initial input of the user we get the hours*/
int getHr(char buffer[]){
    hr = ((buffer[0]-'0')*10) + (buffer[1]-'0');
    return hr;
}

/*in the initial input of the user we get the mins*/
int getMin(char buffer[]){
    minute = (buffer[2]-'0')*10 + (buffer[3]-'0');
    return minute;
}

/*We are not taking any input from the user, 
 *but we need this function to simulate the clock*/
int seconds() {
    s = initialHours*3600;  
    s = s+(initialMins*60);
    s = s+initialSecs;
    s = s+(millis()/1000); 
    return s;
}

/*We are not taking any input from the user, 
 *but we need this function to simulate the clock*/
int hours(){
    hrs = seconds();
    if(initialHours>9){
        hrs = hrs+65536;
    }
    hrs = hrs/3600;
    hrs = hrs%24;
    return hrs;
}


/*We are not taking any input from the user, 
 *but we need this function to simulate the clock*/
int mins(){
    minutes = seconds();
    if(initialHours>9)
    {
        minutes = minutes+65536;
    }
    minutes = minutes/60;
    minutes = minutes%60;
    return minutes;
}

/*We are not taking any input from the user, 
 *but we need this function to simulate the clock*/
int secs(){
    sec = seconds();
    if(initialHours>9)
    {
        sec = sec+65536;
    }
    sec = sec%60;
    return sec;
}

/*Check if the user input is correct, hours and minute*/
int checkclktime(){
    int retval = E_NOTOK;
    int error = false;
    switch(process_state){
        
        case initclk :    initialHours = getHr(realclock);
        initialMins = getMin(realclock);
        if((initialHours<=24)&&(initialMins<=60)){
            dispclkflag = true;  
            lcd.clear();
            retval = E_OK;
        }
        else{
            error = true;
        }
        break;
        case alarmstk1:    alarm1_Hours = getHr(alarm1_buffer);
        alarm1_Mins = getMin(alarm1_buffer);
        if((alarm1_Hours<=24)&&(alarm1_Mins<=60)){
            // dispclkflag = true;  
            lcd.clear();
            retval = E_OK;
        }
        else{
            error = true;
        }
        break;
        case alarmstk2:    alarm2_Hours = getHr(alarm2_buffer);
        alarm2_Mins = getMin(alarm2_buffer);
        if((alarm2_Hours<=24)&&(alarm2_Mins<=60)){
            // dispclkflag = true;  
            lcd.clear();
            retval = E_OK;
        }
        else{
            error = true;
        }
        break;
    }
    
    if(error){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Incorrect Time");
        lcd.setCursor(0,1);
        lcd.print("To reset press #"); 
        retval = E_NOTOK;           
    }
    return retval;
}

/*Uses to get the user input of the clock*/
int getRealclk(){
    int retval= E_NOTOK;
    int clkflag = 1;
    if( dispclkflag == false){
        while(clkflag){
            char key = kpd.waitForKey(); //storing pressed key value in a char
            if (key != NO_KEY){  
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

/****************************************************************************************/
/*     _      _          _      ____    __  __     _____   _   _   _   _    ____        */
/*    / \    | |        / \    |  _ \  |  \/  |   |  ___| | | | | | \ | |  / ___|  ___  */
/*   / _ \   | |       / _ \   | |_) | | |\/| |   | |_    | | | | |  \| | | |     / __| */
/*  / ___ \  | |___   / ___ \  |  _ <  | |  | |   |  _|   | |_| | | |\  | | |___  \__ \ */
/* /_/   \_\ |_____| /_/   \_\ |_| \_\ |_|  |_|   |_|      \___/  |_| \_|  \____| |___/ */
/****************************************************************************************/

/*To check the alarm for the stacks*/
int getAlarm(int Alarm, char Alarm_buffer[]) {
    n=0;
    int retval= E_NOTOK;
    int clkflag = 1;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Alarm"+String(Alarm)+"(hhmm):");
    while(clkflag) {
        char key = kpd.waitForKey(); //storing pressed key value in a char
        if (key != NO_KEY){  
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

/*this it check if the alarm is from stack1/stack2/both*/
int func_monitorAlarm() {
    int retval1 = 0;
    int retval2 = 0;
    if((hours()== alarm1_Hours)&&(mins()==alarm1_Mins)){
        retval1 = 1;
        Serial.println("Stack1");
    }
    if((hours()== alarm2_Hours)&&(mins()== alarm2_Mins)){
        retval2 = 2;
        Serial.println("Stack2");
    }
    return (retval1+retval2);
}


int func_awayModeMonitorAlarm(int val) {
    int retval1 = 0;
    int retval2 = 0;
    
    

    if((alarm1_flag==0)&&(alarm_flag==0) && (val<24)){
      if ((alarm1_Hours<val)){
         retval1 = 1;
        Serial.println("Stack1");
      }
      else if ((alarm1_Hours==val)&&(alarm1_Mins<=mins())){
        retval1 = 1;
        Serial.println("Stack1");
      }
       
    }
    if((alarm2_flag==0)&&(alarm_flag==0)&&(val<24)){

        if ((alarm2_Hours<val)){
         retval2 = 2;
        Serial.println("Stack2");
      }
      else if ((alarm2_Hours==val)&&(alarm2_Mins<=mins())){
        retval1 = 2;
        Serial.println("Stack2");
      }
        
    }
    return (retval1+retval2);
}


/*******************************************************************************************/
/*   ___    _____   _   _   _____   ____    ____      _____   _   _   _   _    ____        */
/*  / _ \  |_   _| | | | | | ____| |  _ \  / ___|    |  ___| | | | | | \ | |  / ___|  ___  */
/* | | | |   | |   | |_| | |  _|   | |_) | \___ \    | |_    | | | | |  \| | | |     / __| */
/* | |_| |   | |   |  _  | | |___  |  _ <   ___) |   |  _|   | |_| | | |\  | | |___  \__ \ */
/*  \___/    |_|   |_| |_| |_____| |_| \_\ |____/    |_|      \___/  |_| \_|  \____| |___/ */
/*******************************************************************************************/                                

/*Beep the buzzer and blink the LED, 
 *
 * GET the stop button Pressed
 
 */
int func_buzzerLED(int led1, int led2){
    Serial.println("isButtonPressed"+String(isButtonPressed));
    isButtonPressed = digitalRead(pushBtn);
    Serial.println("isButtonPressed"+String(isButtonPressed));
    delay(200); 
    if(!weFinished && isButtonPressed){
        state = LOW;
        //       noTone(buzzer);     // Stop sound...
        
        digitalWrite(led1, state);
         if(led2!=-1) digitalWrite(led2, state); 
        weFinished = 1;
        timer=0;
        //    lcd.clear();
        return E_OK;
    }
    else if(!weFinished){
        
        unsigned long currentMillis = millis();
        
        if (currentMillis - previousMillis >= interval) {
            // save the last time you blinked the LED
            previousMillis = currentMillis;
            timer++; // ADDED NEW
            Serial.println("Timer value: " + String(timer));
            
            // if the LED is off turn it on and vice-versa:
            if (state == LOW) { 
                state = HIGH;
                //         tone(buzzer, 100000); // Send 1KHz sound signal...
                digitalWrite(led1, state);  
                if(led2!=-1) digitalWrite(led2, state);
            } 
            else {
                state = LOW;
                //          noTone(buzzer);     // Stop sound...
                digitalWrite(led1, state);  
                if(led2!=-1) digitalWrite(led2, state);  
            }

             /*STATE: SMS via GSM******/
            /*Send SMS to mobile number if no button is pressed for 60 seconds*/
            /****************************/ 

            if(timer==60){
              
              Serial.println("Sending SMS because no button press in " + String(timer)+" seconds");
                            
          //    sendSMS(number,"Alert! patient missed to take pill.");
              
              state = LOW;
                //          noTone(buzzer);     // Stop sound...
                digitalWrite(led1, state);  
                if(led2!=-1) digitalWrite(led2, state);

              weFinished = 1;
              timer=0;
              
              return E_Timer;
            }
        }  
    }
    return E_OKRunning;
}


int func_pillDispense(){
    return E_OK;
}




void printDigits(byte digits){
    if(digits < 10)
        lcd.print('0');
    lcd.print(digits);
}

/* to print the colon*/
char sep(){
    s = millis()/1000;
    if(s%2==0)
    {
        lcd.print(":");
    }
    else {
        lcd.print(" ");
    }  
}

/*Print the clock time*/
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



void sendSMS(String number, String message) {
  // AT command to set SIM900 to SMS mode
/* SIM900.print("AT+CMGF=1\r"); 
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
  SIM900.println(message); 
  delay(100);

  // End AT command with a ^Z, ASCII code 26
  SIM900.println((char)26); 
  delay(100);
  SIM900.println();
  // Give module time to send SMS
  delay(5000); */
}
