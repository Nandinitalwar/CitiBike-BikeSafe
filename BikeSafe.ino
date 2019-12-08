//Name: Nandini Talwar
//Purpose: To run this code on the Arduino, which would activate the accelerometer and GPS system to track a CitiBike
//Source: Open Source on GitHub, edited

#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
MFRC522::MIFARE_Key key;

#include <Wire.h>
#include <ADXL345.h>
ADXL345 adxl; //variable adxl is an instance of the ADXL345 library

long randNumber;

void setup() {
    Serial.begin(9600); // Initialize serial communications with the PC

  randomSeed(analogRead(0));
    
    while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
    SPI.begin();        // Init SPI bus
    mfrc522.PCD_Init(); // Init MFRC522 card

  adxl.powerOn();

  //set activity/ inactivity thresholds (0-255)
  adxl.setActivityThreshold(75); //62.5mg per increment
  adxl.setInactivityThreshold(75); //62.5mg per increment
  adxl.setTimeInactivity(10); // how many seconds of no activity is inactive?
 
  //look of activity movement on this axes - 1 == on; 0 == off 
  adxl.setActivityX(1);
  adxl.setActivityY(1);
  adxl.setActivityZ(1);
 
  //look of inactivity movement on this axes - 1 == on; 0 == off
  adxl.setInactivityX(1);
  adxl.setInactivityY(1);
  adxl.setInactivityZ(1);
 
  //look of tap movement on this axes - 1 == on; 0 == off
  adxl.setTapDetectionOnX(0);
  adxl.setTapDetectionOnY(0);
  adxl.setTapDetectionOnZ(1);
 
  //set values for what is a tap, and what is a double tap (0-255)
  adxl.setTapThreshold(50); //62.5mg per increment
  adxl.setTapDuration(15); //625us per increment
  adxl.setDoubleTapLatency(80); //1.25ms per increment
  adxl.setDoubleTapWindow(200); //1.25ms per increment
 
  //set values for what is considered freefall (0-255)
  adxl.setFreeFallThreshold(7); //(5 - 9) recommended - 62.5mg per increment
  adxl.setFreeFallDuration(45); //(20 - 70) recommended - 5ms per increment
 
  //setting all interrupts to take place on int pin 1
  //I had issues with int pin 2, was unable to reset it
  adxl.setInterruptMapping( ADXL345_INT_SINGLE_TAP_BIT,   ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_DOUBLE_TAP_BIT,   ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_FREE_FALL_BIT,    ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_ACTIVITY_BIT,     ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_INACTIVITY_BIT,   ADXL345_INT1_PIN );
 
  //register interrupt actions - 1 == on; 0 == off  
  adxl.setInterrupt( ADXL345_INT_SINGLE_TAP_BIT, 1);
  adxl.setInterrupt( ADXL345_INT_DOUBLE_TAP_BIT, 1);
  adxl.setInterrupt( ADXL345_INT_FREE_FALL_BIT,  1);
  adxl.setInterrupt( ADXL345_INT_ACTIVITY_BIT,   1);
  adxl.setInterrupt( ADXL345_INT_INACTIVITY_BIT, 1);    
}


void loop() {
  delay(400);
  // print a random number from 0 to 299
  Serial.print("lat=");
  Serial.print("121."); 
  randNumber = random(530000);
  Serial.println(randNumber);  

  // print a random number from 10 to 19
  Serial.print("lng=");
  Serial.print("25.0"); 
  randNumber = random(21614, 63641);
  Serial.println(randNumber);
 
 //Coding the accelerometer and giving it commands   
  int x,y,z;  
 
  double xyz[3];
  double ax,ay,az;
  adxl.getAcceleration(xyz);
  ax = xyz[0];
  ay = xyz[1];
  az = xyz[2];
  Serial.print("x=");
  Serial.print(ax);
  Serial.println(" ");
    
  Serial.print("y=");
  Serial.print(ay);
    Serial.println(" ");
    
  Serial.print("z=");
  Serial.print(az);
    Serial.println(" ");
  
Serial.print(F("uid="));

    // Look for new cards
    if ( (! mfrc522.PICC_IsNewCardPresent()) || ( ! mfrc522.PICC_ReadCardSerial())) {
      Serial.println("00000000");
      Serial.println("*");
      return;
    }

    // Show some details of the PICC (that is: the tag/card)
    
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();
    Serial.println("*");
}

/**
 * Helper routine to dump a byte array as hex values to Serial.
 */
void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ?  "0" : "");
        Serial.print(buffer[i], HEX);
    }

    
}
