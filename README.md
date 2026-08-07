# Surgical-Tremor-Compensator

The goal of this project is to create a handheld instrument that mimics the rotational motion of the user’s hand, using a filter to remove shakes, tremors, and instrument noise. This is demonstrated via a laser pointer mounted on a servo, which tracks intentional hand motion while filtering out high-frequency tremor. Tremor filtering is a common feature of medical robotic and assistive devices, such as Carnegie Mellon's Micron Handheld Surgical Robot, Intuitive Robotics' da Vinci, Gyrogear's GyroGlove, and numerous others across the industry. The purpose of experimenting with this type of filtering system is to understand the signal processing on a simpler, smaller scale, but with the high-level applications in mind.

## Demo video
https://github.com/user-attachments/assets/98885060-1304-4821-b81c-c518528a7d22

## Hardware
- Elegoo Mega 2560
- MPU-6050 IMU
- SG90 micro servo
- 650nm laser diode module
- Breadboard (used two for aesthetic purposes)
- Pencil (handle for IMU)
- Green LED
- 1 kΩ resistor
- SPDT Slide Switch

### Wiring Diagram
<img src="https://github.com/user-attachments/assets/ace63ca7-94fb-43b0-90d3-b89e47cd580e" width = "40%" />

## How it Works
The tremor compensator instrument gets data input from the IMU chip (MPU 6050), which provides simultaneous accelerometer and gyroscope data. The accelerometer data is converted into tilt angle using trig formulas with gravitational acceleration. Theoretically, this could be translated to calculate both pitch and roll, but for this project, only pitch was calculated. In the unfiltered version, this calculated pitch value is simply mapped to the available servo angles via the built-in Arduino map function. In practice, mapping from -80 to 80 rather than the theoretical -90 to 90 produced more accurate and stable servo response. In the next iteration of the code, the low-pass filter, the raw calculated pitch data is run through an exponential moving average (EMA) filter. The EMA filter puts more weight on previous pitch angles, filtering out high-frequency changes. This filter is solid, but it doesn’t perform well for quick intentional jerks. In the eyes of the accelerometer, vibration, movement, and jitter are seen as acceleration, which adds to short-term noise. Over the long term, however, the accelerometer is stable, which is why filtering out the short-term noise provides a solid smoothing that works well for slow movements. The final filter version implements a complementary filter that takes advantage of the simultaneous gyroscope data to also smooth quick movements. The gyroscope measures rotational rate, which is very smooth and responsive with almost no noise. But to get an angle from it, you have to integrate the rate over time, and any tiny constant error in the rate reading accumulates with each integration step — causing the angle estimate to slowly drift away from reality over time. By weighting the gyroscope heavily for short-term responsiveness and the accelerometer lightly as a long-term drift correction, the two sensors complement each other — the gyroscope provides smooth, accurate tracking of fast intentional movements while the accelerometer continuously corrects for the drift that would otherwise accumulate from integrating the gyroscope rate over time.

## Results
<img width="375" height="235" alt="Screenshot 2026-08-06 at 5 05 07 PM" src="https://github.com/user-attachments/assets/778348c2-b477-458c-9f99-5342020db059" />

[Figure 1] Noisy, unfiltered (yellow) data vs smoothed, filtered (blue) data

<img width="375" height="235" alt="Screenshot 2026-08-06 at 5 06 27 PM" src="https://github.com/user-attachments/assets/f5514397-203e-40e9-8083-3b3998f572a3" />

[Figure 2] Circled in Red, unfiltered (yellow) jerk is not translated into filtered (blue) output

## Future Work
kalman filter, multidimensional

## Repository Structure
- `tremor_compensator_FINAL.ino` — demo sketch that allows for toggle between filtered and unfiltered output
- `tremor_compensator_complementary.ino` - final filtered sketch -- complementary filter on both accelerometer and gyro output
- `tremor_compensator_low-pass.ino` - first filtered sketch -- exponential moving average (EMA) filter on accelerometer output
- `tremor_compensator_unfiltered.ino` - raw, unfiltered IMU to servo angle (unfiltered output for final sketch)
- `Preliminary_Sketches` — preliminary sketches used during development
  - `MPU6050_Tilt_Angle_Example.ino` — basic IMU reading and angle calculation/filtering
  - `SG90_KY008_UserInput_Example.ino` — basic servo control (and laser) test via user input
