#  Console IO

Author: Suhani Mitra

Date: 2024-09-19

### Summary

This skill was about communicating between my laptop and the ESP32. This involved sending keystrokes to the ESP32 and have it echo the message back.
To do this, I had to first configure the correct ports (UART port 0, RX 3 and TX 1 ports) as well as the Baud rate - 115200.

To view the communications on Mac I used the CoolTerm console.

I created a new program that has 3 modes the user can toggle between. To toggle between modes, the user sends an 's' character. The state was stored in a global variable and enumration so the program was always aware of what current state it was in.

The first mode is an LED toggle mode. This invovles using the onboard LED (port 13). I reused some of my code from skill cluster-0/06 to assist with this. Everytime the user entered 't', I triggered the LED to light up.

The next mode was an echo mode. This mode allowed for 2 or more characters from the user to be echoed back the console. I did this just by echoing the string back.

Finally, the third mode echoes a decimal number input as hex. To do this I just use the C++ printing format (%X) to convert the in put to hex.

### Evidence of Completion
<p align="center">
<img src="./images/CoolTerm.png" width="50%">
</p>
<p align="center">
Screenshot of the CoolTerm Console of All Modes
</p>

- [Link to video of console/esp communication in real time](https://drive.google.com/file/d/1sQo9YfXzDmauzWUtPAMwwcSpTLyoZGxV/view?usp=drive_link). 

### AI and Open Source Code Assertions

- I have documented in my code readme.md and in my code any
software that we have adopted from elsewhere
- I used AI for coding and this is documented in my code as
indicated by comments "AI generated" 



