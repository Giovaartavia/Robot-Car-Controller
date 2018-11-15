/*********************************************************************
 This is an example for our nRF51822 based Bluefruit LE modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

#include <Wire.h>
#include <Adafruit_MotorShield.h>

#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

#define LED_PIN 13

String tempDirection;
int tempSpeed;

/*=========================================================================
    APPLICATION SETTINGS

    FACTORYRESET_ENABLE       Perform a factory reset when running this sketch
   
                              Enabling this will put your Bluefruit LE module
                              in a 'known good' state and clear any config
                              data set in previous sketches or projects, so
                              running this at least once is a good idea.
   
                              When deploying your project, however, you will
                              want to disable factory reset by setting this
                              value to 0.  If you are making changes to your
                              Bluefruit LE device via AT commands, and those
                              changes aren't persisting across resets, this
                              is the reason why.  Factory reset will erase
                              the non-volatile memory where config data is
                              stored, setting it back to factory default
                              values.
       
                              Some sketches that require you to bond to a
                              central device (HID mouse, keyboard, etc.)
                              won't work at all with this feature enabled
                              since the factory reset will clear all of the
                              bonding data stored on the chip, meaning the
                              central device won't be able to reconnect.
    MINIMUM_FIRMWARE_VERSION  Minimum firmware version to have some new features
    MODE_LED_BEHAVIOUR        LED activity, valid options are
                              "DISABLE" or "MODE" or "BLEUART" or
                              "HWUART"  or "SPI"  or "MANUAL"
    -----------------------------------------------------------------------*/
    #define FACTORYRESET_ENABLE         1
    #define MINIMUM_FIRMWARE_VERSION    "0.6.6"
    #define MODE_LED_BEHAVIOUR          "MODE"
/*=========================================================================*/

// Create the bluefruit object, either software serial...uncomment these lines

SoftwareSerial bluefruitSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);

Adafruit_BluefruitLE_UART ble(bluefruitSS, BLUEFRUIT_UART_MODE_PIN,
                      BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1 and M2
Adafruit_DCMotor *leftMotor = AFMS.getMotor(1); //chooses our left motor to be from spot M1
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2); //chooses our second motor (right motor) to be from spot M2


// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

/**************************************************************************/
/*!
    @brief  Sets up the HW an the BLE module (this function is called
            automatically on startup)
*/
/**************************************************************************/
void setup(void)
{
  while (!Serial);  // required for Flora & Micro
  delay(500);

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


  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(9600);
  Serial.println(F("Starting..."));
  Serial.println(F("---------------------------------------"));

  /* Initialise the module */

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  Serial.println(F("READY FOR CONNECTION!"));

  ble.verbose(false);  // debug info is a little annoying after this point!

  /* Wait for connection */
  while (! ble.isConnected()) {
      delay(500);
  }
}

/**************************************************************************/
/*!
    @brief  Constantly poll for new command or response data
*/
/**************************************************************************/
void loop(void)
{
  // Check for user input
  char inputs[BUFSIZE+1];

  if ( getUserInput(inputs, BUFSIZE) )
  {
    // Send characters to Bluefruit
    Serial.print("[Send] ");
    Serial.println(inputs);

    ble.print("AT+BLEUARTTX=");
    ble.println(inputs);

    // check response stastus
    if (! ble.waitForOK() ) {
      Serial.println(F("Failed to send?"));
    }
  }

  // Check for incoming characters from Bluefruit
  ble.println("AT+BLEUARTRX");
  ble.readline();
  if (strcmp(ble.buffer, "OK") == 0) {
    // no data
    return;
  }

  if(Serial.available() > 0){
    Serial.println("Serial available!");
  }

  // Some data was found, its in the buffer ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //ble.buffer = ble.buffer.replace("\n","");
  tempDirection = ble.buffer;
  tempDirection.trim();
  tempSpeed = tempDirection.substring(tempDirection.indexOf('/')+1).toInt();
  tempDirection = tempDirection.substring(0, tempDirection.indexOf('/'));


  //Receive data and act accordingly
  Serial.print(F("[Recv] ")); Serial.println(tempDirection);
  if(tempDirection == "fwd"){
    bothFwd(tempSpeed);
  }
  else if(tempDirection=="back"){
    bothBack(tempSpeed);
  }
  else if(tempDirection == "right"){
    turnRight(tempSpeed);
  }
  else if(tempDirection == "left"){
    turnLeft(tempSpeed);
  }
  else if(tempDirection == "fleft"){
    halfFwdLeft(tempSpeed);
  }
  else if(tempDirection == "fright"){
    halfFwdRight(tempSpeed);
  }
  else if(tempDirection == "bleft"){
    halfBackLeft(tempSpeed);
  }
  else if(tempDirection == "bright"){
    halfBackRight(tempSpeed);
  }
  else if(tempDirection == "stop"){
    stopFunc();
  }
  
  ble.waitForOK();
}

/**************************************************************************/
/*!
    @brief  Checks for user input (via the Serial Monitor)
*/
/**************************************************************************/
bool getUserInput(char buffer[], uint8_t maxSize)
{
  // timeout in 100 milliseconds
  TimeoutTimer timeout(100);

  memset(buffer, 0, maxSize);
  while( (!Serial.available()) && !timeout.expired() ) { delay(1); }

  if ( timeout.expired() ) return false;

  delay(2);
  uint8_t count=0;
  do
  {
    count += Serial.readBytes(buffer+count, maxSize);
    delay(2);
  } while( (count < maxSize) && (Serial.available()) );

  return true;
}


//**********Turning functions**********
void bothFwd(int speed){ //this function tells both wheels to move at same speed, so should move forward at whatever direction robot is facing
  Serial.print("Entered bothFwd()"); //prints to Serial to check if function ran
  //  uint8_t i=100; //might need to change int speed to be uint8_t later, so left code for reference
    leftMotor->run(FORWARD); //tells motors to go forward (versus backwards)
    rightMotor->run(FORWARD);
    rightMotor->setSpeed(speed);  //sets the speed of the motors to be whatever speed was passed in as)
    leftMotor->setSpeed(speed);  
    //delay(1000); //delays because if you don't delay the speed will be set back to 0 really fast
    //rightMotor->setSpeed(0);
    Serial.print("motor2 ran forwards "); 
    //leftMotor->setSpeed(0);
    Serial.println("motor1 ran forwards ");
    //leftMotor->run(RELEASE); //releases the motors, supposed to be necessary
    //rightMotor->run(RELEASE);
    //delay(1000); //delay before next function is run, may be not necessary later just for demonstration purposes
}


void bothBack(int speed){ //this function does the same thing as bothFwd, but the motors are told to go backwards this time
    Serial.print("Entered bothBack() ");
    leftMotor->run(BACKWARD); //these two lines tell the motors to go backwards (versus forwards)
    rightMotor->run(BACKWARD);
    rightMotor->setSpeed(speed);  
    leftMotor->setSpeed(speed);  
    //delay(1000);
    //rightMotor->setSpeed(0);
    Serial.print("motor2 ran backwards ");
    //leftMotor->setSpeed(0);
    Serial.println("motor1 ran backwards ");
    //leftMotor->run(RELEASE);
    //rightMotor->run(RELEASE);
    //delay(1000);
}


void turnRight(int speed){ //this function turns the motors right by allowing the right motor to remain its speed, and changing left motor to be a speed
    rightMotor->run(FORWARD);
    rightMotor->setSpeed(speed);  
    //delay(10);
    leftMotor->setSpeed(0);
    Serial.print("motor2 ran forwards");
    //rightMotor->run(RELEASE);
    //delay(1000);
}


void turnLeft(int speed){ //this function turns left by keeping one motor its previous speed (0) and setting the other to full speed
    leftMotor->run(FORWARD);
    leftMotor->setSpeed(speed);  
    //delay(10); //for demonstration purposes
    rightMotor->setSpeed(0);
    Serial.print("motor1 ran forwards"); //prints out that only motor1 ran forwards, so you will notice that motor2 was absent
    //leftMotor->run(RELEASE);
    //delay(1000);
}


void halfBackLeft(int speed){ //this function should turn left with one motor full speed and the other half speed
    Serial.print("Entered bothBack() ");
    leftMotor->run(BACKWARD);
    rightMotor->run(BACKWARD);
    rightMotor->setSpeed(speed/4);  
    leftMotor->setSpeed(speed);  
    //delay(1000);
    //rightMotor->setSpeed(0);
    Serial.print("motor2 ran backwards ");
    //leftMotor->setSpeed(0);
    Serial.println("motor1 ran backwards ");
    //leftMotor->run(RELEASE);
    //rightMotor->run(RELEASE);
    //delay(1000);
}


void halfBackRight(int speed){
  leftMotor->run(BACKWARD);
  rightMotor->run(BACKWARD);
  rightMotor->setSpeed(speed);  
  leftMotor->setSpeed(speed/4);
}


void halfFwdLeft(int speed){
  leftMotor->run(FORWARD);
  rightMotor->run(FORWARD);
  rightMotor->setSpeed(speed/4);  
  leftMotor->setSpeed(speed);
}


void halfFwdRight(int speed){
  leftMotor->run(FORWARD);
  rightMotor->run(FORWARD);
  rightMotor->setSpeed(speed);  
  leftMotor->setSpeed(speed/4);
}


void stopFunc(){
  leftMotor->setSpeed(0);
  rightMotor->setSpeed(0);
}
