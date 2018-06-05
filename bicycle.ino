//#include <LiquidCrystal.h>
#include <string.h>
float wheelSize = 26; // 26".  wheel diam., (inches)
int gerkonPinIN =  2;  // pin for reading gerkon state. Must allow interrupts. (pins 2 or 3 for Arduino Uno)
float measuringwindowDef = 0.5; // (seconds).  Time window using for calculate speed (instead of 1 second). I.e. refresh rate.
                          // Depends of last measured velocity, if speed is to low, then time window increasing for better precision.
float measuringwindow;

volatile unsigned long int whCount =0;
unsigned long i, whLastCount, time0, time1 = 0; 
float spd = 0;  // velocity 
int maxSpd = 30; // km/h. expected max velocity of bike, used for time window calculation.
float koef;
int outputPin = 13; 

/*

/////////////////// LCD SHIELD /////////////////////
//LiquidCrystal lcd(RS, E, D4,D5,D6,D7);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7 );
// Нажатые кнопки
int button;
const int BUTTON_NONE   = 0;
const int BUTTON_RIGHT  = 1;
const int BUTTON_UP     = 2;
const int BUTTON_DOWN   = 3;
const int BUTTON_LEFT   = 4;
const int BUTTON_SELECT = 5;
/////

int getPressedButton()
{
  int buttonValue = analogRead(0); // считываем значения с аналогового входа(A0) 
  if (buttonValue < 100) {
    return BUTTON_RIGHT;  
  }
  else if (buttonValue < 200) {
    return BUTTON_UP;
  }
  else if (buttonValue < 400){
    return BUTTON_DOWN;
  }
  else if (buttonValue < 600){
    return BUTTON_LEFT;
  }
  else if (buttonValue < 800){
    return BUTTON_SELECT;
  }
  return BUTTON_NONE;
}

/////////////////// LCD SHIELD /////////////////////


*/

// the setup routine runs once when you press reset:
void setup() {
  wheelSize =wheelSize* 2.54 /100;    //meters 
  koef = 3.14 * wheelSize;   // 2* Pi * R == Pi * D
  measuringwindow =  measuringwindowDef;
  pinMode(gerkonPinIN, INPUT);  
  attachInterrupt(digitalPinToInterrupt(gerkonPinIN), wheel, RISING);
 
 // pinMode(outputPin, OUTPUT);    


  Serial.begin(9600);
//  lcd.begin(16, 2); 
 // lcd.setCursor(0,0); 
}




void loop() {
  // put your main code here, to run repeatedly:

//  

time0 = millis();
if ((time0- time1) >= int(measuringwindow*1000)) {
  
spd = koef * ((whCount-whLastCount)/measuringwindow);  // calculating speed, in m/s:   koef (meters) * wheelturns / measuring time (sec) 
spd *= 3.6;  // converting speed to km/h                               //// 7.4651616 * w * t

time1 = time0;
whLastCount = whCount;

//lcd.setCursor(0,0); 
//lcd.print ("SPEED: "); 
//lcd.setCursor(10,0); 
//lcd.print (spd); 


Serial.println(spd);
//measuringwindow = timeadjust(spd);

//Serial.println(spd);
//Serial.println(measuringwindow);


}

else if (time0 == 4294967290) {
  time0=0; time1=0;

}
}




float timeadjust(float vel) {

int x = (100* int(vel))/ maxSpd; // Current bike velocity in percentage of its maximum
Serial.println(x);
if ( (x>100) || (x<=0) ){ return measuringwindowDef; }

float t = (measuringwindowDef/100  * (100 - x)) + measuringwindowDef;     // For example, if default time window = 0.5 sec, and last measured velosity is 3km/h (10% of 30km/h),
                                                                       // then measuringwindow will be on 90% more that default measuringwindowDef.  
                                                                       // If last velocity is 0, then measuring time will be 2 times bigger than default, for better precision.
                                                                       // So, depending of velocity, measuring time window will change from 1 to
return t;
}



void wheel()
{
 whCount ++;
}

