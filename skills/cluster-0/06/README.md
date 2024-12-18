#  Create a New Project and Use GPIO to Control LEDs

Author: Suhani Mitra

Date: 2024-09-05

### Summary

This skill is about creating a new project and drawing from the existing blink example code to create a new LED flash pattern. To do this I copied over the esp-idf blink code and modified it to align with the indicated flashing pattern. I updated the configure_led function to allow an integer input which represented the correct port to which the LED was connected to in the circuit. I then manually turned each LED on by setting the state level to 1, then waiting for 1 second delay before turning the next LED on. 
I then performed a binary countdown on the LEDs by toggling the appropriate LED to match a 1111 -> 0000 binary bitwise countdown. This was inside of a while loop which keeps repeating.

I used the reference on GitHub which identifies port names and their respective port identification number to help me with setting the correct LEDs on/off.

My modified code is accessible in the 'code' subfolder of this skill.

### Evidence of Completion

- [Link to video of LED circuit illuminating as instructed](https://drive.google.com/file/d/16_kfzbKCp-gxJQeko1cbmYlInsWctYD2/view?usp=drive_link).

### AI and Open Source Code Assertions

- I have documented in my code readme.md and in my code any
software that we have adopted from elsewhere
- I used AI for coding and this is documented in my code as
indicated by comments "AI generated" 



