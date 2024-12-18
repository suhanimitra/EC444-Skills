#  Accelerometer I2C

Author: Suhani Mitra

Date: 2024-09-26

### Summary

This skill involved wiring an ADXL343 with the ESP32 and reading in acceleration values to determine roll and pitch. To do this, we used the I^2C interface of the ADAXL343. Using the base code, I followed a similar format to the utility testConnection function to read and write from single registers. With the help of AI, I got these to work. Using the single register read, I then created a function to read 16 bits of data. I read each byte value separately, combined these into a signed word by ORing the LSB with the MSB shifted to the left by 8, and then returned this as a signed 16-bit word.

This returned value allowed me to calculate the acceleration values.
Once I had acceleration values for all 3 axes (x, y, z), I then was able to generate tilt data. To calculate the roll, I took the arctan of (y/z) and multiplied it by the scaling value 57.3. For pitch, I took the arctan of (-x/sqrt(y^2 + z^2)) and multiplied it by the scaling value 57.3.

Since we know that acceleration is measured in terms of gravity, we expect that at any one point in time when the ADXL343 is parallel/perpendicular to the ground, two axes are near 0 and the other is near 9.8. In the video below, we can see this in real time. When flat, the z axis is near 9.8. When tilted to the left, the x axis is near 9.8. When tilted toward us, the y axis is near 9.8.

### Evidence of Completion

<p align="center">
<img src="./images/accelerometer-circuit.png" width="50%">
</p>
<p align="center">
ADXL343 and ESP32 Circuit (ignore button on the right)
</p>

- [Link to video of x, y, z axes changing with movement. At any given moment, one axis is close to 9.8 (gravity) and the other two are ~0](https://drive.google.com/file/d/1VDuT7_t0MK8UeX8xB8IeHHBOtWWtat81/view?usp=drive_link).

### AI and Open Source Code Assertions

- I have documented in my code readme.md and in my code any
software that we have adopted from elsewhere
- I used AI for coding and this is documented in my code as
indicated by comments "AI generated" 



