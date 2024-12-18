#  Alphanumeric Display I2c

Author: Suhani Mitra

Date: 2024-09-26

### Summary

This skill invovled wiring up an alphanumeric display and displaying a user prompted string with scroll. To do this, I had to utilize the ascii decoder table found in the Adafruit LED Backpack file. This allowed me to convert from user inputted chars to LED-compatible encoding. I prompted the user for input for up to 16 chars, took each char and converted it to the corret eoncding, then looped through this by looping through the char array while maintining an index and offset such that we would rotate every 1 second for a scroll effect.

### Evidence of Completion

- [Link to video of real-time user input of "SUHANI MITRA".](https://drive.google.com/file/d/1cfJxLDYvn9C_VbJ10tQwSYK_3CqXnLIL/view?usp=drive_link)

### AI and Open Source Code Assertions

- I have documented in my code readme.md and in my code any
software that we have adopted from elsewhere
- I used AI for coding and this is documented in my code as
indicated by comments "AI generated" 



