#  WASD Remote Control

Author: Suhani Mitra

Date: 2024-11-23

### Summary

This skill involved integrating our Purple Car movements with a 'remote control' interface. To do this, we had to integrate the ESP32 and laptop (hosting the remote control) such that they communicated. This involved hosting a node client on the laptop that sent char input to the ESP32 server which, upon receiving a char command, moved the car in the according direction. We created a UDP server on the ESP and UDP client on the node side (laptop).

We denoted that 'w' = forward, 'a' = turning left, 's' = reverse, and 'd' = turn right. The video below shows functionality.

### Evidence of Completion

- [Link to video demo of live remote control using 'w' 'a' 's' 'd' keys and the car moving accordingly.](https://drive.google.com/file/d/1PpKA3qECo_GNaA2xeO8yOeKTMfWSQzfH/view?usp=drive_link).

### AI and Open Source Code Assertions

- I have documented in my code readme.md and in my code any
software that we have adopted from elsewhere
- I used AI for coding and this is documented in my code as
indicated by comments "AI generated" 



