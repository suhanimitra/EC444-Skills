#  RTOS -- Multiple Tasks

Author: Suhani Mitra

Date: 2024-09-19

### Summary


This skill required us to separate 3 functions into tasks and prioritize them accordingly. For the first task, binary count up/down, I utilized my code from cluster-0/06 and just reversed this for the binary countdown. I used a similar circuit to that of which I had in the cluster-0 task.

For the second task, button sensing, I first initialized the button port as GPIO, then read the level from this. 1 indicated that the button was pushed at 0 indicated that it wasn't. I wired this element of the circuit up with a resistor connected between teh button and ground. 

Finally, the third task invovled setting the left-most blue LED for when the circiut was performing binary countdown. I had to create a second variable that maintained the state of the count direction such that the LED would stay on throughout the duration of the count up. I set this "currState" binary value in the button sensing task depending on what the previous values of the state and button push ("flag" variable) were.

Both the flag and current state variables were globally scoped such that all tasks had visibility to these variables.

I prioritized these as such: 1 - button status, since it toggles, 2 - binary_count, since it must perform the count, 3 - setting the LED since this stays on for the duration of the count.

**Please note: when the button is pressed to toggle between counting directions, the current binary count will execute until completion (i.e. reaches 0000 for countdown or 1111 for count up). Only then will we see the blue LED indicate the opposite counting method and will the toggled counting method begin.

### Evidence of Completion

<p align="center">
<img src="./images/circuit.png" width="50%">
</p>
<p align="center">
Image of the Circuit
</p>

- [Link to video of binary count down, button press, completion of binary count down and then binary count up with LED indication.](https://drive.google.com/file/d/1bJP-4uR9mvvGSqqEku-OkxOlXVna5uD2/view?usp=drive_link). Please see note above for explanation of functionality.

### AI and Open Source Code Assertions

- I have documented in my code readme.md and in my code any
software that we have adopted from elsewhere
- I used AI for coding and this is documented in my code as
indicated by comments "AI generated" 



