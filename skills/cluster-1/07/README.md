#  Battery Voltage Monitor

Author: Suhani Mitra

Date: 2024-09-10

### Summary

This skill involved wiring a voltage divider circuit and measuring the battery voltage. I used 2 resistors in series to drop the voltage and then I used the esp monitor with the provided code to track the output voltage. My first resistor was 330 ohms and the second was 1k ohms. When solving for the expected voltage output (using the 3.3 V source voltage), we find that Vo = (3.3/ (1000 + 300)) * 1000 = 2.48 Volts.

As we can see in the images below, the voltage divider is working correctly and reading a value close to the expected (calculated) value.

### Evidence of Completion
<p align="center">
<img src="./images/07-VoltageReading.png" width="50%">
</p>
<p align="center">
Circiut and Voltage Reading
</p>

<p align="center">
<img src="./images/07-VoltageReadingClose.png" width="50%">
</p>
<p align="center">
Close Up of the Voltage Reading
</p>

### AI and Open Source Code Assertions

- I have documented in my code readme.md and in my code any
software that we have adopted from elsewhere
- I used AI for coding and this is documented in my code as
indicated by comments "AI generated" 



