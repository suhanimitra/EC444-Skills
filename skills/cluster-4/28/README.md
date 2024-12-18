#  Leader Election

Author: Suhani Mitra

Date: 2024-11-01

### Summary

This skill invovled having our 4 ESPs communicate. We had to integrate this such that all ESPs would connect to the internet and send messages to each other. When someone is a leader, the leader's green LED toggles on. All other boards' red LEDs will be on. When an election is running, both red/green LEDs are on to indicate this. We hard-coded different IDs on each ESP when flashin, but the rest of the functionality is identical.

When an election is running, we identify the person with the lowest ID. They are the leader. They continously send keep alive messages to other members. In the case that they fail (we unplug them), we start running an election again and elect the member with the current lowest ID as leader. If the old leader replugs and comes back on, they regain their leader status.


### Evidence of Completion

<p align="center">
<img src="./images/state-diagram.png" width="50%">
</p>
<p align="center">
Our Bully Algorithm State Diagram
</p>


- [Link to video of live leader election/updating](https://drive.google.com/file/d/1HtD_7_EbP_z3_TraLswNNjPM_ra9iD7M/view?usp=drive_link).

### AI and Open Source Code Assertions

- I have documented in my code readme.md and in my code any
software that we have adopted from elsewhere
- I used AI for coding and this is documented in my code as
indicated by comments "AI generated" 



