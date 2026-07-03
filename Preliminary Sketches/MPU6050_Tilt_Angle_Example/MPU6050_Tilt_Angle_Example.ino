// MPU6050 Calculate Tilt Angle from Accelerometer Data

#include <Wire.h> // I2C communication library (SDA/SCL pins)
#include <MPU6050.h> // Rowberg library (I2C commands)

MPU6050 mpu;

// Global Variables
float pitchOffset = 0; 
float rollOffset = 0; 
float filteredPitch = 0;
float filteredRoll = 0;
// alpha controls how much weight new reading vs historical average holds in filter/moving average
// can tune alpha up if response feels too sluggish for intentional motion, or down for more smoothing
const float alpha = 0.1; // 0.1 means 10% new to 90% history

void setup() {
  Serial.begin(115200); // serial connection at 115200 baud (relatively arbitrary choice but 115200 is a good habit for more data-intensive projects)
  Wire.begin(); // starts I2C
  mpu.initialize(); // wakes up MPU6050
  Serial.println(mpu.testConnection() ? "MPU6050 connected" : "Connection failed"); // if connection test passes print connected, otherwise print failed

  int16_t ax, ay, az, gx, gy, g