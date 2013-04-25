// Electromechanical derailleur project v.1.0
// Assumes you know the servo positions for each gear (determined using v.0.0 software)

#include <Servo.h> 
#include <EEPROM.h>            //Need this because we'll be saving the gear selection in EEPROM (to avoid shifts on startup)

#define db_time 20             //Button debounce time in mS
#define servo_pin 4            //HW pin to which servo signal line is attached
#define servoDelta 1           //Amt servo moves with each button push (degress)
#define upButtonPin 2          //Up button push grounds it
#define dnButtonPin 3          //Dn button push grounds it
#define highestGear 7          //Number of sprockets on rear hub

#define rearGear1 40           //Servo position values for the various rear sprockets (numbered low to high gears, not by size)
#define rearGear2 61           //These were determined empirically using the diyshift0 sketch while hooked up to a laptop
#define rearGear3 76
#define rearGear4 90
#define rearGear5 106
#define rearGear6 121
#define rearGear7 138

#define rearGearAddr 0            //EEPROM address for saving the gear selected

Servo rearServo;                  // Create servo object to control a servo 
int   lastUpButtonState = 1;
int   lastDnButtonState = 1;
int   rearPos = 90;               // variable to store the servo position
int   rearGear;                   //Numbered sprocket (1 is lowest gear; 8 is highest)
 
void setup() 
{ 
  rearGear = EEPROM.read(rearGearAddr);
  rearServo.attach(servo_pin);                          //Attach the rear servo to the servo object
  
  if ((rearGear > 0) && (rearGear < 8))
  {
    shiftToRearGear(rearGear);                          //And immediately set it to whatever gear was last saved in EEPROM
  }
  else
  {
    shiftToRearGear(3);                                 //If not valid, goto gear 3
  }
  
  pinMode(upButtonPin, INPUT);
  pinMode(dnButtonPin, INPUT);
  
  digitalWrite(upButtonPin, HIGH);                      //Enable internal pullup resistors
  digitalWrite(dnButtonPin, HIGH);

  Serial.begin(9600);    //USED ONLY FOR DEBUG
} 
 
 
void loop() 
{ 
  int upButtonState = digitalRead(upButtonPin);          //Poll the up/dn buttons
  int dnButtonState = digitalRead(dnButtonPin);

  if (upButtonState == 0){                               //Button was pushed
    delay(db_time);                                      //Wait a bit to debounce the button
    upButtonState = digitalRead(upButtonPin);            //Test again
    if (upButtonState == 0 && lastUpButtonState == 1){   //If still down AND was up before
      upOneGear();                                       //Shift up one gear
      lastUpButtonState = 0;                             //Save state
    }
  }
  else {                                                  //upButton must be open (button state = 1)
    delay (db_time);                                      //Wait a bit to debounce the button
    upButtonState = digitalRead(upButtonPin);             //Test again
    if (upButtonState == 1){                              //Still open, save state
      lastUpButtonState = 1;
    }
  }
  //
  //Now same for dowm
  //
  if (dnButtonState == 0){                               //Button was pushed
    delay(db_time);                                      //Wait a bit to debounce the button
    dnButtonState = digitalRead(dnButtonPin);            //Test again
    if (dnButtonState == 0 && lastDnButtonState == 1){   //If still down AND was up before
      dnOneGear();                                       //Shift down one gear
      lastDnButtonState = 0;                             //Save state
    }
  }
  else {                                                  //dnButton must be open (button state = 1)
    delay (db_time);                                      //Wait a bit to debounce the button
    dnButtonState = digitalRead(dnButtonPin);             //Test again
    if (dnButtonState == 1){                              //Still open, save stated
      lastDnButtonState = 1;
    }
  }
}
//
//Functions called
//
  void upOneGear(){
    if (rearGear < highestGear){
      rearGear = rearGear + 1;
    }
    shiftToRearGear(rearGear);
  }
  
    void dnOneGear(){
    if (rearGear > 1){
      rearGear = rearGear - 1;
    }
    shiftToRearGear(rearGear);
  }
  
  void shiftToRearGear(int gear){
      EEPROM.write(rearGearAddr, gear);
      switch (gear) {
    case 1:
      rearServo.write(rearGear1);
      break;
    case 2:
      rearServo.write(rearGear2);
      break;
    case 3:
      rearServo.write(rearGear3);
      break;
    case 4:
      rearServo.write(rearGear4);
      break;
    case 5:
      rearServo.write(rearGear5);
      break;
    case 6:
      rearServo.write(rearGear6);
      break;
    case 7:
      rearServo.write(rearGear7);
      break;
    }
  }
  
