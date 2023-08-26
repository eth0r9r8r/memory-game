# memory-game
This is a personal project I made while taking an embedded systems course at the University of Calgary. During this course we worked with a PIC24F16KA101 microcontroller to learn about embedded system programming and interfacing. After finishing a lab on digital I/O, I wanted to keep experimenting with the microcontroller, and so I made this game.

## Gameplay
The game interface consists of three LEDs corresponding to three pushbuttons. The LEDs blink out a sequence that the player must copy with the buttons. The sequence starts with a single LED blinking, and builds on itself with each correct response.

A youtube demo of the gameplay can be found [here](https://youtu.be/e5uB8w_L7gE).

## How it works
The hardest part about creating this game was making it so that each sequence is random and unique. My solution was to start a timer when the microcontroller was connected to power, and then store the value of that timer when a button was eventually pressed. I used the timer value as a seed for the C `srand` function, which allows me to get a random integer whenever I need one with a call to `rand`.

After startup, the program enters the main loop. This loop starts by generating a random sequence of 100 integers ranging from 0-2. An idle animation then plays until the player presses another button. The program then displays the sequence and checks the player's response for mistakes. When the player does make a mistake, the program returns to the start of the main loop.
