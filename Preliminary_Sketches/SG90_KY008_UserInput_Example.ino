/* 
  Reads a value from Serial Monitor and moves servo to that angle
  Turns on laser pointer when in use
*/

#include <Servo.h>
#define LASER_PIN 5 // define digital pin number connected to laser pointer

Servo serv;  // create Servo object to control a servo

int pos = 0;    // variable to store the servo position

void setup() {
  serv.attach(9);  // attaches the servo on pin 9 to the Servo object
  pinMode(LASER_PIN, OUTPUT); // configures laser pointer pin as an output
}

void loop() {
  // insert code for mapping servo angle and turning on laser
  // if running
  digitalWrite(LASER_PIN, HIGH);

}
