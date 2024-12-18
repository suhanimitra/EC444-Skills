#  Thermistor

Author: Suhani Mitra

Date: 2024-09-10

### Summary

This skill invovled creating a voltage divider circuit with a thermistor and using this thermistor voltage reading to sense temperature. To do this, my circuit consisted of a 10k ohm resistor in series with the thermistor. We expect Vs = (3.3/{10000 + 10000 (these are resistor / thermistor resistance values)}) * 10000 (r2) = 1.65 Volts when the temperature is around 25 degrees celcius (thermistor acts as 10k resistor at this temperature).

To convert the voltage reading to celcius, I used documentation online (source found in comment of code). I essentially had to rearrange the above equation and solve for the thermistor resistance. Once I obtained this, I used the Steinhart-hart equation to convert to Kelvin and subsequently to Celcius. 

I also had to change the ADC attenuation value to read voltages within the correct range. ADC_ATTEN_DB_6 was fitting for this. 

### Evidence of Completion

- [Link to demo video of temperature changing with body heat/cold water bottle](https://drive.google.com/file/d/1eJPpNcU8NefS6o-vINJwhKqZ_89HAAjq/view?usp=drive_link).

### AI and Open Source Code Assertions

- I have documented in my code readme.md and in my code any
software that we have adopted from elsewhere
- I used AI for coding and this is documented in my code as
indicated by comments "AI generated" 



