#  PID

Author: Suhani Mitra

Date: 2024-11-23

### Summary

This skill involved using a sensor to toggle LEDs based on the error state, following a PID model. For this, I used my base code from skill 34 — and the corresponding ultrasonic sensor circuit — and the PID model given on this repository. I wired in 3 LEDs which served as indicators for object distance. Following the PID model, I calculated error. If the error was 0 (between the measured distance and desired distance), the green LED toggled. If the error was negative, the red LED toggled. If the error was positive, the blue LED toggled.

The video below demonstrates functionality. First, when my hand is not in range of the sensor, the red LED is on (object too far). Then, when my hand is in range but too close in comparison to the desired distance, the blue LED toggles. Finally, when my hand is in the desired location, the green LED toggles on. In this case (and for testing purposes), I set the desired setpoint to 11.22 cm based on previous tests I ran.

### Evidence of Completion

- [Link to video demo of LEDs toggling based on error.](https://drive.google.com/file/d/1ABvDdrTEPngjyUYhBjs1Z-xiMyaLOG3Z/view?usp=drive_link).

### AI and Open Source Code Assertions

- I have documented in my code readme.md and in my code any
software that we have adopted from elsewhere
- I used AI for coding and this is documented in my code as
indicated by comments "AI generated" 



