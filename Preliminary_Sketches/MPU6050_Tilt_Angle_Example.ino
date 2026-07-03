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

  int16_t ax, ay, az, gx, gy, gz; // 3 accelerometer values, 3 gyro values
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz); // & symbols pass variables by reference (function writes directly into variables instead of returning)

  // Raw accelerometer values are arbitrary integer counts
  // Dividing by 16384 converts them to units of g (gravitational acceleration)
  float axg = ax / 16384.0;
  float ayg = ay / 16384.0;
  float azg = az / 16384.0;

  // takes one reading at startup to capture chip's resting angle --> stores it as the offset for pitch and roll
  // note that RAD_TO_DEG is a built-in Arduino constant (57.2958) that converts from radians to degrees
  pitchOffset = RAD_TO_DEG * atan2(axg, sqrt(ayg * ayg + azg * azg));
  rollOffset  = RAD_TO_DEG * atan2(ayg, sqrt(axg * axg + azg * azg));
  // atan2() -- built in math function that computes an angle from two values 
  //    - in this case, first value is the axis we care about (x for pitch, y for roll) and other value (sqrt) is magnitude of remaining axes combined
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
  float roll  = RAD_TO_DEG * atan2(ayg, sqrt(axg * axg + azg * azg)) - rollOffset;

  // Exponential Moving Average Filter (otherwise noise causes numbers to change frequently even at stable position)
  // Each new filtered value is a blend of the current raw reading (weighted by alpha -- 10%) and the previous filtered value (weighted by 1-alpha -- 90%).
  filteredPitch = alpha * pitch + (1 - alpha) * filteredPitch;
  filteredRoll  = alpha * roll  + (1 - alpha) * filteredRoll;

  // Print filtered values
  Serial.print("Pitch: " + String(filteredPitch) + " | ");
  Serial.println("Roll: " + String(filteredRoll));

  delay(100);
}
