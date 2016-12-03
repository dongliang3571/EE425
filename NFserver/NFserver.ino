/* YourDuinoStarter Example:RECEIVE nRF24L01 Joystick data to control Pan Tilt Servos Over Radio.
   QUESTIONS? terry@yourduino.com
 -WHAT IT DOES:
  -Receives Joystick Analog Values over a nRF24L01 Radio Link, using the Radiohead library.
  - Sends Joystick position to 2 servos, usually X,Y to pan-tilt arrangement
  - TODO! Send the Joystick push-down click to turn Laser on and off
 - SEE the comments after "//" on each line below
 - CONNECTIONS: nRF24L01 Modules See:
 http://arduino-info.wikispaces.com/Nrf24L01-2.4GHz-HowTo
   1 - GND
   2 - VCC 3.3V !!! NOT 5V
   3 - CE to Arduino pin 8
   4 - CSN to Arduino pin 10
   5 - SCK to Arduino pin 13
   6 - MOSI to Arduino pin 11
   7 - MISO to Arduino pin 12
   8 - UNUSED


   -V2.00 7/12/14 by Noah King
   Based on examples at http://www.airspayce.com/mikem/arduino/RadioHead/index.html
*/

/*-----( Import needed libraries )-----*/
// SEE http://arduino-info.wikispaces.com/Arduino-Libraries  !!
// NEED the SoftwareServo library installed
// http://playground.arduino.cc/uploads/ComponentLib/SoftwareServo.zip
// #include <SoftwareServo.h>  // Regular Servo library creates timer conflict!

// NEED the RadioHead Library installed!
// http://www.airspayce.com/mikem/arduino/RadioHead/RadioHead-1.23.zip
#include <RHReliableDatagram.h>
#include <RH_NRF24.h>

#include <SPI.h>

/*-----( Declare Constants and Pin Numbers )-----*/
#define CLIENT_ADDRESS     1
#define SERVER_ADDRESS     2

#define LaserPIN           6

#define ServoMIN_H  0  // Don't go to very end of servo travel
#define ServoMAX_H  160 // which may not be all the way from 0 to 180. 
#define ServoMIN_V  0  // Don't go to very end of servo travel
#define ServoMAX_V  140 // which may not be all the way from 0 to 180. 


/*-----( Declare objects )-----*/
//SoftwareServo HorizontalServo;
//SoftwareServo VerticalServo;  // create servo objects to control servos

// Create an instance of the radio driver
RH_NRF24 RadioDriver;

// Create an instance of a manager object to manage message delivery and receipt, using the driver declared above
RHReliableDatagram RadioManager(RadioDriver, SERVER_ADDRESS);

/*-----( Declare Variables )-----*/
int HorizontalJoystickReceived; // Variable to store received Joystick values
int HorizontalServoPosition;    // variable to store the servo position

int VerticalJoystickReceived;   // Variable to store received Joystick values
int VerticalServoPosition;      // variable to store the servo position

uint8_t ReturnMessage[] = "JoyStick Data Received";  // 28 MAX
// Predefine the message buffer here: Don't put this on the stack:
uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];


int speedPin1 = 3;
int speedPin2 = 9;
int motor1APin = 6;
int motor2APin = 7;
int motor1BPin = 4;
int motor2BPin = 5;
int speed1;
int speed2;

//--------------------------------( SETUP Runs ONCE )-----------------------------------------------------
void setup()
{
  Serial.begin(115200);


  ////////////////////// setup motors //////////////////////////
  pinMode(speedPin1, OUTPUT);
  pinMode(motor1APin, OUTPUT);
  pinMode(motor2APin, OUTPUT);
  pinMode(speedPin2, OUTPUT);
  pinMode(motor1BPin, OUTPUT);
  pinMode(motor2BPin, OUTPUT);
  speed1 = 0;
  speed2 = 0;
  
  if (!RadioManager.init()) // Initialize radio. If NOT "1" received, it failed.
    Serial.println("init failed");
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
}



//--------------------------------( LOOP runs continuously )-----------------------------------------------------
void loop()
{
  if (RadioManager.available())
  {
 // Wait for a message addressed to us from the client
    uint8_t len = sizeof(buf);
    uint8_t from;

    //Serial Print the values of joystick
    if (RadioManager.recvfromAck(buf, &len, &from)) {
//      Serial.print("got request from : 0x");
//      Serial.print(from, HEX);
//      Serial.print(": X = ");
//      Serial.println(buf[0]);
//      Serial.print(" Y = ");
//      Serial.println(buf[1]);

      if(buf[1] <= 123 && buf[0] >= 125) {
        // put motor in forward motion and left wheel is faster or equal than right wheel
        digitalWrite(motor1APin, LOW); // set leg 1 of the H-bridge low
        digitalWrite(motor2APin, HIGH); // set leg 2 of the H-bridge high
        digitalWrite(motor1BPin, LOW); // set leg 1 of the H-bridge low
        digitalWrite(motor2BPin, HIGH); // set leg 2 of the H-bridge high
        speed1 = (123 - buf[1])/128.0 * 255.0;
//        speed2 = speed1 * (1 - (buf[0] - 125)/128);
//        Serial.println("IF 1 ");
//        Serial.print("speed 1: ");
//        Serial.println(speed1);
//        Serial.print("speed 2: ");
//        Serial.println(speed2);
      } else if(buf[1] <= 123 && buf[0] < 125) {
        // put motor in forward motion and right wheel is faster than left wheel
        digitalWrite(motor1APin, LOW); // set leg 1 of the H-bridge low
        digitalWrite(motor2APin, HIGH); // set leg 2 of the H-bridge high
        digitalWrite(motor1BPin, LOW); // set leg 1 of the H-bridge low
        digitalWrite(motor2BPin, HIGH); // set leg 2 of the H-bridge high
        speed2 = (123 - buf[1])/128.0 * 255.0;
//        speed1 = speed2 * (1 - (125 - buf[0])/128.0);
//        Serial.println("IF 2 ");
//        Serial.print("speed 1: ");
//        Serial.println(speed1);
//        Serial.print("speed 2: ");
//        Serial.println(speed2);
      } else if(buf[1] > 123 && buf[0] >= 125) {
        // put motor in backward motion and left wheel is faster or equal than right wheel
        digitalWrite(motor1APin, HIGH); // set leg 1 of the H-bridge low
        digitalWrite(motor2APin, LOW); // set leg 2 of the H-bridge high
        digitalWrite(motor1BPin, HIGH); // set leg 1 of the H-bridge low
        digitalWrite(motor2BPin, LOW); // set leg 2 of the H-bridge high
        speed1 = (buf[1] - 123)/132.0 * 255.0;
//        speed2 = speed1 * (1 - (buf[0] - 125)/128.0);
//        Serial.println("IF 3 ");
//        Serial.print("speed 1: ");
//        Serial.println(speed1);
//        Serial.print("speed 2: ");
//        Serial.println(speed2);
      } else if(buf[1] > 123 && buf[0] < 125) {
        // put motor in backward motion and right wheel is faster than left wheel
        digitalWrite(motor1APin, HIGH); // set leg 1 of the H-bridge low
        digitalWrite(motor2APin, LOW); // set leg 2 of the H-bridge high
        digitalWrite(motor1BPin, HIGH); // set leg 1 of the H-bridge low
        digitalWrite(motor2BPin, LOW); // set leg 2 of the H-bridge high
        speed2 = (buf[1] - 123)/132.0 * 255.0;
//        speed1 = speed2 * (1 - (125 - buf[0])/128.0);
//        Serial.println("IF 4 ");
//        Serial.print("speed 1: ");
//        Serial.println(speed1);
//        Serial.print("speed 2: ");
//        Serial.println(speed2);
      } else {
        // put motor in forward motion and left wheel equal to right wheel
        digitalWrite(motor1APin, LOW); // set leg 1 of the H-bridge low
        digitalWrite(motor2APin, HIGH); // set leg 2 of the H-bridge high
        digitalWrite(motor1BPin, LOW); // set leg 1 of the H-bridge low
        digitalWrite(motor2BPin, HIGH); // set leg 2 of the H-bridge high
        speed1 = (123 - buf[1])/128.0 * 255.0;
        speed2 = speed1;
//        Serial.println("IF 5 ");
//        Serial.print("speed 1: ");
//        Serial.println(speed1);
//        Serial.print("speed 2: ");
//        Serial.println(speed2);
      }

      // control the speed 0- 255
      analogWrite(speedPin1, speed1); // output speed as PWM value
      analogWrite(speedPin2, speed2); // output speed as PWM value

      // Send a reply back to the originator client, check for error
      if (!RadioManager.sendtoWait(ReturnMessage, sizeof(ReturnMessage), from))
        Serial.println("sendtoWait failed");
    }// end 'IF Received data Available
  }// end 'IF RadioManager Available

}// END Main LOOP
