#  Hardware Interrupts

Author: Suhani Mitra

Date: 2024-09-19

### Summary

This skill invovles using a button to toggle LEDs in two mannerisms. The first method invovled polling the button constatly. To do this, I used a while loop to read the state of the button, compare it with the last button state, and then toggle the LEDs whenever the button signal was HIGH and the last state was not. I toggled through the LEDs by having a current LED index and incrementing through an array of my LED pin numbers in order (making sure I didn't exceed 3 (0, 1, 2, 3 indices allowed)).

The second method invovled using hardware interrupts. To do this, I used the included interrupt program and integrated my LED toggling task from the polling method. I intialized the GPIO to detect button press as interrupt and define an ISR handler that sets the flag on an interrupt.

### Evidence of Completion

I have included two different videos, though the have the same functionality, to demonstrate both methods I implemented.

- [Link to video of polling method](https://drive.google.com/file/d/1NL0z3dDgXjccJoCCY2N28ycS2FLm8ZEB/view?usp=drive_link).
- [Link to video of interrupt method](https://drive.google.com/file/d/1fq7XvxS3r-XcprnbStsXwTZcB-eQ7O44/view?usp=drive_link).

### AI and Open Source Code Assertions

- I have documented in my code readme.md and in my code any
software that we have adopted from elsewhere
- I used AI for coding and this is documented in my code as
indicated by comments "AI generated" 