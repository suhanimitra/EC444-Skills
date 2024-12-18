#  Ultrasonic Sensor 1

Author: Suhani Mitra

Date: 2024-11-23

### Summary

This skill involved wiring the Ultrasonic sensor with the ESP32. I connected the ground/power pins, RX to power, and analog output for ADC reading. To measure, we first obtained the raw voltage data and then converted it by dividing by the according scaling factor based on the spec sheet for this part. This turned out to be around 9.8, based on 9.8 mV per cm from the datasheet. This gave us our estimated distance of the object.

### Evidence of Completion

- [Link to video demo of sensor recognizing hand and displaying approximate distance.](https://drive.google.com/file/d/1ff1OT1ADqHnNckUppm11NDyZgNA1LXdq/view?usp=drive_link).

### AI and Open Source Code Assertions

- I have documented in my code readme.md and in my code any
software that we have adopted from elsewhere
- I used AI for coding and this is documented in my code as
indicated by comments "AI generated" 
