#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 8
#define CSN_PIN 10

const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

int joystick[2];
int speedPin1 = 3;
int speedPin2 = 9;
int motor1APin = 6;
int motor2APin = 7;
int motor1BPin = 4;
int motor2BPin = 5;
int speed1;
int speed2;


void forward() {
  digitalWrite(motor1APin, LOW); // set leg 1 of the H-bridge low
  digitalWrite(motor2APin, HIGH); // set leg 2 of the H-bridge high
  digitalWrite(motor1BPin, LOW); // set leg 1 of the H-bridge low
  digitalWrite(motor2BPin, HIGH); // set leg 2 of the H-bridge high
}

void backward() {
  digitalWrite(motor1APin, HIGH); // set leg 1 of the H-bridge low
  digitalWrite(motor2APin, LOW); // set leg 2 of the H-bridge high
  digitalWrite(motor1BPin, HIGH); // set leg 1 of the H-bridge low
  digitalWrite(motor2BPin, LOW); // set leg 2 of the H-bridge high
}

void setup()
{
  Serial.begin(9600);
  delay(1000);
  Serial.println("starting program......");

  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();

  ////////////////////// setup motors //////////////////////////
  pinMode(speedPin1, OUTPUT);
  pinMode(motor1APin, OUTPUT);
  pinMode(motor2APin, OUTPUT);
  pinMode(speedPin2, OUTPUT);
  pinMode(motor1BPin, OUTPUT);
  pinMode(motor2BPin, OUTPUT);
  speed1 = 0;
  speed2 = 0;
}


void loop()
{
    if(radio.available()) {
      radio.read(joystick, sizeof(joystick));
    
      int joystickX = map(joystick[0],0,1023,0,255); // turn value of 0-1023 to 0-180 degrees
      int joystickY = map(joystick[1],0,1023,0,255);  // turn value of 0-1023 to 0-180 degrees

      Serial.print("X:");
      Serial.println(joystickX);
      Serial.print("Y:");
      Serial.println(joystickY);
//      delay(1000);
            
      if(joystickY < 123 && joystickX > 125) {
        Serial.println("IF 1");
        // put motor in forward motion and left wheel is faster or equal than right wheel
        forward();
        speed1 = (123 - joystickY)/128.0 * 255.0;
        speed2 = speed1 * (1 - (joystickX - 125)/128);

      } else if(joystickY < 123 && joystickX < 125) {
        Serial.println("IF 2");
        // put motor in forward motion and right wheel is faster than left wheel
        forward();
        speed2 = (123 - joystickY)/128.0 * 255.0;
        speed1 = speed2 * (1 - (125 - joystickX)/128.0);
      } else if(joystickY <= 30) {
        forward();
        speed1 = 255;
        speed2 = speed1;
      } else if(joystickY >= 230) {
        backward();
        speed1 = 255;
        speed2 = speed1;
      } else if(joystickX <= 30) {
        Serial.println("IF 3");
        digitalWrite(motor1APin, HIGH); // set leg 1 of the H-bridge low
        digitalWrite(motor2APin, LOW); // set leg 2 of the H-bridge high
        digitalWrite(motor1BPin, LOW); // set leg 1 of the H-bridge low
        digitalWrite(motor2BPin, HIGH); // set leg 2 of the H-bridge high
        speed1 = 255;
        speed2 = 255;
      } else if(joystickX >= 220) {
        Serial.println("IF 4");
        // put motor in backward motion and left wheel is faster or equal than right wheel
        digitalWrite(motor1APin, LOW); // set leg 1 of the H-bridge low
        digitalWrite(motor2APin, HIGH); // set leg 2 of the H-bridge high
        digitalWrite(motor1BPin, HIGH); // set leg 1 of the H-bridge low
        digitalWrite(motor2BPin, LOW); // set leg 2 of the H-bridge high
        speed1 = 255;
        speed2 = 255;
      } else if(joystickY > 123 && joystickX >= 125) {
        Serial.println("IF 5");
        // put motor in backward motion and left wheel is faster or equal than right wheel
        backward();
        speed1 = (joystickY - 123)/132.0 * 255.0;
        speed2 = speed1 * (1 - (joystickX - 125)/128.0);
      } else if(joystickY > 123 && joystickX < 125) {
        Serial.println("IF 6");
        // put motor in backward motion and right wheel is faster than left wheel
        backward();
        speed2 = (joystickY - 123)/132.0 * 255.0;
        speed1 = speed2 * (1 - (125 - joystickX)/128.0);
      } else {
        Serial.println("IF 7");
        speed1 = 0;
        speed2 = speed1;
      }
        
      // control the speed 0- 255
      analogWrite(speedPin1, speed1); // output speed as PWM value
      analogWrite(speedPin2, speed2); // output speed as PWM value

      delay(20);
    } else {
      Serial.println("not available");
    }
    
}
