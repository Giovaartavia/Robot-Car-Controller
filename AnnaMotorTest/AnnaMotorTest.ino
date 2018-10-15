/* 
 *  CURRENT STATUS: Code written for halfLeft and threeQuartersLeft, but has not been tested on Arduino. 
 *  Other functions have been tested.
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
Written by Anna
 * NOTES:
    //35 makes it twitch a lil
    //36 and 37 is slow
    //40 is on the slower side
    //regular speed 100 is about a good safe speed
    //200 is pretty fast
*/

#include <Wire.h>
#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1 and M2
Adafruit_DCMotor *leftMotor = AFMS.getMotor(1); //chooses our left motor to be from spot M1
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2); //chooses our second motor (right motor) to be from spot M2

void setup() { //setup and loop are required functions for arduino code
  Serial.println("Entered setup()");
  Serial.begin(9600);           // set up Serial library at 9600 bps

  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  
  ////////////////Below chunk of code appears to be necessary to start motors////////////////
  rightMotor->setSpeed(0);  
  leftMotor->setSpeed(0);  
  leftMotor->run(FORWARD);
  rightMotor->run(FORWARD);
  leftMotor->run(RELEASE);
  rightMotor->run(RELEASE);
  ///////////////////////////////////////////////////////////////////////////////////////////
}

void loop(){ //using the loop that repeats forever to call test functions
    Serial.println("Entered loop()");
    delay(10000); //delay for ten seconds (10,000 mS) to give chance to program and set robot on floor
    bothFwd(36); //runs function bothFwd with speed 36 (slow)
    bothBack(36); //runs bothBack with speed 36 (slow)
    delay(1000000); //delay at end to give a chance to turn off robot (delays for 1,000 seconds, 1,000,000 mS, before restarting loop)
}

void bothFwd(int speed){ //this function tells both wheels to move at same speed, so should move forward at whatever direction robot is facing
  Serial.print("Entered bothFwd()"); //prints to Serial to check if function ran
  //  uint8_t i=100; //might need to change int speed to be uint8_t later, so left code for reference
    leftMotor->run(FORWARD); //tells motors to go forward (versus backwards)
    rightMotor->run(FORWARD);
    rightMotor->setSpeed(speed);  //sets the speed of the motors to be whatever speed was passed in as)
    leftMotor->setSpeed(speed);  
    delay(1000); //delays because if you don't delay the speed will be set back to 0 really fast
    rightMotor->setSpeed(0);
    Serial.print("motor2 ran forwards "); 
    leftMotor->setSpeed(0);
    Serial.println("motor1 ran forwards ");
    leftMotor->run(RELEASE); //releases the motors, supposed to be necessary
    rightMotor->run(RELEASE);
    delay(1000); //delay before next function is run, may be not necessary later just for demonstration purposes
}

void bothBack(int speed){ //this function does the same thing as bothFwd, but the motors are told to go backwards this time
    Serial.print("Entered bothBack() ");
    leftMotor->run(BACKWARD); //these two lines tell the motors to go backwards (versus forwards)
    rightMotor->run(BACKWARD);
    rightMotor->setSpeed(speed);  
    leftMotor->setSpeed(speed);  
    delay(1000);
    rightMotor->setSpeed(0);
    Serial.print("motor2 ran backwards ");
    leftMotor->setSpeed(0);
    Serial.println("motor1 ran backwards ");
    leftMotor->run(RELEASE);
    rightMotor->run(RELEASE);
    delay(1000);
}

void turnRight(int speed){ //this function turns the motors right by allowing the right motor to remain its speed, and changing left motor to be a speed
    leftMotor->run(FORWARD);
    leftMotor->setSpeed(speed);  
    delay(10); //for demonstration purposes
    leftMotor->setSpeed(0);
    Serial.print("motor1 ran forwards"); //prints out that only motor1 ran forwards, so you will notice that motor2 was absent
    leftMotor->run(RELEASE);
    delay(1000);
}

void turnLeft(int speed){ //this function turns left by keeping one motor its previous speed (0) and setting the other to full speed
    rightMotor->run(FORWARD);
    rightMotor->setSpeed(speed);  
    delay(10);
    rightMotor->setSpeed(0);
    Serial.print("motor2 ran forwards");
    rightMotor->run(RELEASE);
    delay(1000);
}

void halfLeft(int speed){ //this function should turn left with one motor full speed and the other half speed
    Serial.print("Entered bothBack() ");
    leftMotor->run(BACKWARD);
    rightMotor->run(BACKWARD);
    rightMotor->setSpeed(speed);  
    leftMotor->setSpeed(speed/0.5);  
    delay(1000);
    rightMotor->setSpeed(0);
    Serial.print("motor2 ran backwards ");
    leftMotor->setSpeed(0);
    Serial.println("motor1 ran backwards ");
    leftMotor->run(RELEASE);
    rightMotor->run(RELEASE);
    delay(1000);
}

void threeQuartersLeft(int speed){ //this function should turn left with one motor full speed and the other much less
    Serial.print("Entered bothBack() ");
    leftMotor->run(BACKWARD);
    rightMotor->run(BACKWARD);
    rightMotor->setSpeed(speed);  
    leftMotor->setSpeed(speed/0.75);  
    delay(1000);
    rightMotor->setSpeed(0);
    Serial.print("motor2 ran backwards ");
    leftMotor->setSpeed(0);
    Serial.println("motor1 ran backwards ");
    leftMotor->run(RELEASE);
    rightMotor->run(RELEASE);
    delay(1000);
}
