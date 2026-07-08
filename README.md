# Surgical-Tremor-Compensator

The goal of this project is to create a handheld instrument that mimics the rotational motion of the user’s hand, using a filter to remove shakes or tremors. This is demonstrated via a laser pointer mounted on a servo, which tracks intentional hand motion while filtering out high-frequency tremor. Tremor filtering is a common feature of medical robotic and assistive devices, such as Carnegie Mellon's Micron Handheld Surgical Robot, Intuitive Robotics' da Vinci, Gyrogear's GyroGlove, and numerous others across the industry. The purpose of experimenting with this type of filtering system is to understand the signal processing on a simpler, smaller scale, but with the high-level applications in mind.

## Demo video

## Hardware
- Elegoo Mega 2560
- MPU-6050 IMU
- SG90 micro servo
- 650nm laser diode module
- Breadboard
- 

## Repository Structure
- [name here ->] — main project sketch
- `Preliminary_Sketches` — preliminary sketches used during development
  - `MPU6050_Tilt_Angle_Example.ino` — basic IMU reading and angle calculation
  - `SG90_KY008_UserInput_Example.ino` — basic servo control test via user input with running laser
