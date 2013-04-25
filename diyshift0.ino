// Electromechanical derailleur project v.0.0
// Use this sketch to determine proper servo
// positions for each gear


#include <Servo.h> 
#define db_time 20             //Button debounce time in mS
#define servoDelta 1           //Amt servo moves with each button push (degress)
#define upButtonPin 2          //Up button push grounds it
#define dnButtonPin 3          //Dn button push grounds it

Servo rearServo;  // Create servo object to control a servo 
int   lastUpButtonState = 1;
int   lastDnButtonState = 1;
int rearPos = 90;   // variable to store the servo position 
 
void setup() 
{ 
  rearServo.attach(9);                                  //Attache the rear servo on pin 9 to the servo object
  pinMode(upButtonPin, INPUT);
  pinMode(dnButtonPin, INPUT);
  
  digitalWrite(upButtonPin, HIGH);                      //Enable internal pullup resistors
  digitalWrite(dnButtonPin, HIGH);

  Serial.begin(9600);
} 
 
 
void loop() 
{ 
  int upButtonState = digitalRead(upButtonPin);
  int dnButtonState = digitalRead(dnButtonPin);

  if (upButtonState == 0){                               //Button was pushed
    delay(db_time);                                      //Wait a bit to debounce the button
    upButtonState = digitalRead(upButtonPin);            //Test again
    if (upButtonState == 0 && lastUpButtonState == 1){   //If still down AND was up before
      rearPos = rearPos + servoDelta;                    //Move servo (toward higher gear)
      rearServo.write(rearPos);    
      Serial.print("Servo at: ");
      Serial.println(rearPos);
      lastUpButtonState = 0;                            //Save state
    }
  }
  else {                                                  //upButton must be open (button state = 1)
    delay (db_time);                                      //Wait a bit to debounce the button
    upButtonState = digitalRead(upButtonPin);             //Test again
    if (upButtonState == 1){                              //Still open, save stated
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
      rearPos = rearPos - servoDelta;                    //Move servo (toward lower gear)
      rearServo.write(rearPos); 
      Serial.print("Servo at: ");
      Serial.println(rearPos);
      lastDnButtonState = 0;                            //Save state
    }
  }
  else {                                                  //upButton must be open (button state = 1)
    delay (db_time);                                      //Wait a bit to debounce the button
    upButtonState = digitalRead(upButtonPin);             //Test again
    if (dnButtonState == 1){                              //Still open, save stated
      lastDnButtonState = 1;
    }
  }
 
} 
