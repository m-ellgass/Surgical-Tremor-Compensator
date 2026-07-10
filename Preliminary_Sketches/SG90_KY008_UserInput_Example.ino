/* 
  Reads a value from Serial Monitor and moves servo to that angle
  Turns on laser pointer when in use
*/

#include <Servo.h>
#define LASER_PIN 5 // define digital pin connected to laser pointer

Servo serv;  // create Servo object to control a servo

int pos = 0;  // variable to store servo position
bool laser = false;  // variable to store on/off state of laser
bool asked = false;  // variable stop instructions being repeated forever

void setup() {
  Serial.begin(115200); // serial connection at 115200 baud 
  serv.attach(9);  // attaches the servo on pin 9 to the Servo object
  pinMode(LASER_PIN, OUTPUT);  // configures laser pointer pin as an output
  delay(1000);  // wait a second after startup to allow for board reset
}

void loop() {
  // Print instructions once, then wait for a response
  if (!asked) {
    Serial.println("Servo Testing Started.");
    Serial.println("Enter servo angle (0-180)");
    Serial.println("At any point, enter l to toggle laser on/off or enter i to repeat these instructions");
    asked = true;
  }

  // Check user response
  if (Serial.available() > 0) {
    String input = Serial.readString();
    input.trim(); // removes trailing whitespace/newline

    if (input.length() > 0) {
      // Check if the input is a valid positive number
      bool isNumber = true;
      for (int i = 0; i < input.length(); i++) {
        if (!isDigit(input.charAt(i))) {
          isNumber = false;
          break;
        }
      }

      if (isNumber) { // if input was a number, set servo state
        int angle = input.toInt();
        if (angle > 180) {
          angle = 180;
        }
        serv.write(angle);
        Serial.print("Servo set to ");
        Serial.print(angle);
        Serial.println(" degrees");
      }
      else { // if input was not a number and instead was "l" or "L" then toggle laser state
        input.toLowerCase(); // make it lowercase
        if (input == "l") {
          laser = !laser;
          if (laser) {
            Serial.println("Laser toggled on");
          }
          else {
            Serial.println("Laser toggled off");
          }
        } // if input was an "i" or "I" then repeat the instructions
        else if (input == "i") {
          Serial.println("Enter servo angle (0-180), l to toggle laser on/off, or i to repeat these instructions.");          
        }
        else {
          Serial.println("Error: Invalid Input");
          Serial.println("Enter servo angle (0-180), l to toggle laser on/off, or i to repeat these instructions.");          
        }
      }
    }
  }

  // control laser state
  if (laser) {
    digitalWrite(LASER_PIN, HIGH);
  } else {
    digitalWrite(LASER_PIN, LOW);
  }
}
