# Surgical-Tremor-Compensator

The goal of this project is to create a handheld instrument that mimics the rotational motion of the user’s hand, using a filter to remove shakes, tremors, and instrument noise. This is demonstrated via a laser pointer mounted on a servo, which tracks intentional hand motion while filtering out high-frequency tremor. Tremor filtering is a common feature of medical robotic and assistive devices, such as Carnegie Mellon's Micron Handheld Surgical Robot, Intuitive Robotics' da Vinci, Gyrogear's GyroGlove, and numerous others across the industry. The purpose of experimenting with this type of filtering system is to understand the signal processing on a simpler, smaller scale, but with the high-level applications in mind.

## Demo video

## Hardware
- Elegoo Mega 2560
- MPU-6050 IMU
- SG90 micro servo
- 650nm laser diode module
- Breadboard (used two for aesthetic purposes)
- Green LED
- 1 kΩ resistor
- SPDT Slide Switch

### Wiring Diagram

## How it Works
sensor
filter
output

## Results

## Repository Structure
- `tremor_compensator_FINAL.ino` — demo sketch that allows for toggle between filtered and unfiltered output
- `tremor_compensator_complementary.ino` - final filtered sketch -- complementary filter on both accelerometer and gyro output
- `tremor_compensator_low-pass.ino` - first filtered sketch -- exponential moving average (EMA) filter on accelerometer output
- `tremor_compensator_unfiltered.ino` - raw, unfiltered IMU to servo angle (unfiltered output for final sketch)
- `Preliminary_Sketches` — preliminary sketches used during development
  - `MPU6050_Tilt_Angle_Example.ino` — basic IMU reading and angle calculation/filtering
  - `SG90_KY008_UserInput_Example.ino` — basic servo control (and laser) test via user input
