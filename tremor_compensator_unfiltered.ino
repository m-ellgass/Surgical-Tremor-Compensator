/*
  This program is the Unfiltered version such that it does not take into account tremors
  but note that the pitch calculation uses rolling average to filter out the MPU noise
  Reads pitch of MPU (rotate along long side axis)
  Maps servo angle to MPU pitch
  Note: Be careful that MPU is in upright (90º) position on upload for calibration
  Also note that SG90 doesn't have decimal precision
*/

// Libraries
#include <Wire.h> 
#include <MPU6050.h> 
#include <Servo.h> 

// Objects
MPU6050 mpu;
Servo serv;

// Global Variables
float pitchOffset = 0; 
float filteredPitch = 0;
// alpha controls how much weight new reading vs historical average holds in filter/moving average
// can tune alpha up if response feels too sluggish for intentional motion, or down for more smoothing
const float alpha = 0.1; // 0.1 means 10% new to 90% history
int pos = 0;  // variable to store servo position


void setup() {
  Serial.begin(115200); // serial connection at 115200 baud (relatively arbitrary choice but 115200 is a good habit for more data-intensive projects)
  Wire.begin(); // starts I2C
  mpu.initialize(); // wakes up MPU6050
  Serial.println(mpu.testConnection() ? "MPU6050 connected" : "Connection failed"); // if connection test passes print connected, otherwise print failed
  serv.attach(9);  // attaches the servo on pin 9 to the Servo object

  int16_t ax, ay, az, gx, gy, gz; // 3 accelerometer values, 3 gyro values
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Raw accelerometer values are arbitrary integer counts
  // Dividing by 16384 converts them to units of g (gravitational acceleration)
  float axg = ax / 16384.0;
  float ayg = ay / 16384.0;
  float azg = az / 16384.0;

  // takes one reading at startup to capture chip's resting angle --> stores it as pitch offset
  pitchOffset = RAD_TO_DEG * atan2(axg, sqrt(ayg * ayg + azg * azg));
}

void loop() {
  // same process as above -- get mpu accel and gyro values and then convert to units of g for new reading every loop iteration
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  float axg = ax / 16384.0;
  float ayg = ay / 16384.0;
  float azg = az / 16384.0;

  // Calculate tilt angles in degrees (same way as offset values were calculated above) then subtract offset calculated at startup
  float pitch = RAD_TO_DEG * atan2(axg, sqrt(ayg * ayg + azg * azg)) - pitchOffset;

  // Exponential Moving Average Filter (otherwise noise causes numbers to change frequently even at stable position)
  // Each new filtered value is a blend of the current raw reading (weighted by alpha -- 10%) and the previous filtered value (weighted by 1-alpha -- 90%).
  filteredPitch = alpha * pitch + (1 - alpha) * filteredPitch;
  pos = round(filteredPitch); // round pitch (otherwise map function auto-truncates)
  pos = map(pos, -90, 90, 0, 180);
  pos = constrain(pos, 0, 180);

  // Map servo angle
  serv.write(pos);

  // Print current MPU pitch and mapped angle
  Serial.print("Current pitch: " + String(filteredPitch) + " | ");
  Serial.println("Servo angle: " + String(pos));

  delay(100);
}