#  Sharp IR Range Sensor

Author: Suhani Mitra

Date: 2024-11-23

### Summary

This skill involved wiring an IR range sensor to provide us with accurate range readings. To do so, I connected the ground, power, and analog pins to the ESP32. This allowed me to read, through the ADC channel, a raw voltage reading. I modified the attenuation value so that it would read in the correct range. Given the output voltage value, I ran some tests to calibrate the distance. I found the approximate voltage value when an object was at the near-end range of the sensor (closest) as well as an approximate voltage for when the object was further away. Using this, I aimed to calculate my distance in a hyperbolic model where, as voltage increases, distance increases accordingly.


### Evidence of Completion

- [Link to video demo of IR sensor recognizing my hand and the distance increasing as I move my hand further away.](https://drive.google.com/file/d/1n-JE_esvWl2Qo5T-_jlIjuetvkn_nfhu/view?usp=drive_link)

### AI and Open Source Code Assertions

- I have documented in my code readme.md and in my code any
software that we have adopted from elsewhere
- I used AI for coding and this is documented in my code as
indicated by comments "AI generated" 



