#  Timer

Author: Suhani Mitra

Date: 2024-09-26

### Summary

This skill invovled creating a timer that counts up to 15 and then wraps back around to 00. These two digits must be displayed on the LED screen, and we also needed to integrate a reset button.

To do this, I wired up the button as I did in previous clusters. I used a flag to signify whether the button was pressed or not, and this would essentially set the counter variable back to 00 such that we restart our timer.

In terms of the actual counting, I used division and modulus to obtain a tens and ones place digit respectively, I then ensured that the first two buffer spots of the LED were empty and dispaly on the other two. I then incremented the counter index every second which caused us to accurately count up in seconds. I always performed counter % 16 to ensure that we wrapped back around to 00 once we hit 15 seconds.

### Evidence of Completion
<p align="center">
<img src="./images/14_circuit_pic.png" width="50%">
</p>
<p align="center">
LED/Button TimerCircuit
</p>

- [Link to video demo of timer displaying mod 16 and button reset functinality.](https://drive.google.com/file/d/1ZttHkC2SzKNFygLp_kF1TJzMGKBcBxt9/view?usp=drive_link)

### AI and Open Source Code Assertions

- I have documented in my code readme.md and in my code any
software that we have adopted from elsewhere
- I used AI for coding and this is documented in my code as
indicated by comments "AI generated" 



