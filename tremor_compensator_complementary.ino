/*
  This program is the filtered version such that it implements a both a low-pass and a complementary high-pass filter
  Reads pitch of MPU (rotate along long side axis) using accelerometer data
  Reads pitch of MPU using gyroscope data
  Filters readings using exponential moving average (EMA) to low-pass filter accel while high-pass filtering gyro
  Maps servo angle to filtered MPU pitch
  Note: Be careful that MPU is in upright (90 deg) position on upload for calibration
  Also note that SG90 doesn't have decimal precision (angles are integers)
*/

// Libraries
#include <Wire.h> 
#include <MPU6050.h> 
#include <Servo.h> 

// Objects
MPU6050 mpu;
Servo serv;

// Global Variables
#define LASER_PIN 5 // define digital pin connected to laser pointer
float pitchOffset = 0; 
float filteredPitch = 0;
unsigned long lastTime = millis(); // time for gyro integration
// const float alpha = 0.15; // alpha is redundant in the complementary filter (alpha = 1 - beta)
const float beta = 0.9; // beta controls gyro and accel weight simultaneously (0.9 means 90% gyro to 10% accel)
int pos = 0;  // variable to store servo position


void setup() {
  Serial.begin(115200); // serial connection at 115200 baud (relatively arbitrary choice but 115200 is a good habit for more data-intensive projects)
  Wire.begin(); // starts I2C
  mpu.initialize(); // wakes up MPU6050
  Serial.println(mpu.testConnection() ? "MPU6050 connected" : "Connection failed"); // if connection test passes print connected, otherwise print failed
  serv.attach(9);  // attaches the servo on pin 9 to the Servo object
  pinMode(LASER_PIN, OUTPUT);  // configures laser pointer pin as an output

  // Start calibration for MPU (pitch) angle offset 0.5s after startup
  Serial.println("Calibrating... keep MPU still and upright.");
  delay(500);

  const int numSamples = 50; // number of readings to average
  float pitchSum = 0;

  for (int i = 0; i < numSamples; i++) {
    int16_t ax, ay, az, gx, gy, gz; // 3 accelerometer values, 3 gyro values
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Raw accelerometer values are arbitrary integer counts
  // Dividing by 16384 converts them to units of g (gravitational acceleration)
    float axg = ax / 16384.0;
    float ayg = ay / 16384.0;
    float azg = az / 16384.0;

    float pitchSample = RAD_TO_DEG * atan2(axg, sqrt(ayg * ayg + azg * azg));
    pitchSum += pitchSample;

    delay(10); // small gap between samples
  }

  // pitch offset is average of all samples taken
  pitchOffset = pitchSum / numSamples; 
  Serial.print("Calibration complete. Offset: ");
  Serial.println(pitchOffset);
}

void loop() {
  // need change in time (dt) for gyro integration
  unsigned long currentTime = millis();
  float dt = (currentTime - lastTime) / 1000.0; // convert ms to seconds
  lastTime = currentTime;
  
  // same process as above -- get mpu accel and gyro values and then convert accel to units of g and gyro to degrees per second
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  float axg = ax / 16384.0;
  float ayg = ay / 16384.0;
  float azg = az / 16384.0;
  float gyroY = gY / 131.0; // !! check that this is the right axis

  // Get angle from accelerometer
  // Calculate tilt angles in degrees (same way as offset values were calculated above) then subtract offset calculated at startup
  float accel_angle = RAD_TO_DEG * atan2(axg, sqrt(ayg * ayg + azg * azg)) - pitchOffset;

  /* OLD LOW-PASS FILTER
  // Exponential Moving Average Filter (otherwise noise causes numbers to change frequently even at stable position)
  // Each new filtered value is a blend of the current raw reading (weighted by alpha -- 10%) and the previous filtered value (weighted by 1-alpha -- 90%).
  filteredPitch = alpha * pitch + (1 - alpha) * filteredPitch;
  pos = round(filteredPitch); // round pitch (otherwise map function auto-truncates)
  pos = map(pos, -80, 80, 0, 180); // will match if servo points up at 90 deg and mpu wires on left side
  pos = constrain(pos, 0, 180);
  */

  // Get angle from gyroscope
  
  // Turn on laser pointer
  digitalWrite(LASER_PIN, HIGH);

  // Map servo angle
  serv.write(pos);

  // Print current MPU pitch and mapped angle
  Serial.print("Unfiltered:");
  Serial.print(pitch);
  Serial.print(", Pitch:");
  Serial.print(filteredPitch);
  Serial.print(", Servo_Angle:");
  Serial.println(pos);

  delay(100);
}
