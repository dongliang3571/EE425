
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"


#define CE_PIN   8
#define CSN_PIN 10
#define JoyStick_X_PIN     A5  //Pin Numbers
#define JoyStick_Y_PIN     A4

#define CLIENT_ADDRESS 1      // For Radio Link
#define SERVER_ADDRESS 2

const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe

// Create an instance of the radio driver
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

int joystick[2];

void setup()  /****** SETUP: RUNS ONCE ******/
{
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(pipe);
}



void loop() /****** LOOP: RUNS CONSTANTLY ******/
{
  //Read the joystick values, scale them to 8-bit type and store them in the joystick[] array.
  Serial.println("Reading joystick values ");
  // Take the value of Joystick voltages which are 0 to 1023 (10 bit), and convert them to 0 to 255 (8 bit)
  joystick[0] = analogRead(JoyStick_X_PIN);
  joystick[1] = analogRead(JoyStick_Y_PIN);

  //Display the joystick values in the serial monitor.
  Serial.print("x:");
  Serial.print(joystick[0]);
  Serial.print("y:");
  Serial.println(joystick[1]);

  radio.write( joystick, sizeof(joystick) );
}


