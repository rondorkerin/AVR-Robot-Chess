AVR Robot Chess
By Nick Bryant

Overview
-------------------------------
This application was created for senior design at UCF, Summer-Fall 2012. It is the code that accompanies our 
robotic chess set. We didn't use an AVR, actually - we used an Atmel Xmega128A1 microcontroller. This code can easily be
ported to use an AVR. 

The chess engine we used was an AVR port of the Micromax minimalist engine, the smallest engine in the world (at the time of writing.)


Installation
-------------------------------
Project files for AtmelStudio 6 are included with the code. There are plenty of examples in the solution that I found
useful for working with the Atmel Xplained board.

-Prerequisites
AtmelStudio 6


Improvements/Todo
-------------------------------
We wanted to implement a player versus player and computer versus computer mode, but that would require a lot of hacking of the chess engine. 
Currently we only support player versus computer. 

We are also interested in using a communications link to a network-connected computer to play chess with other chess playing robots over the internet.

If you have any questions or comments on my code (it's not the most beautiful thing I've ever written), please message sbryant31 [at] knights.ucf.edu.


Understanding the code
------------------------------
The code is broken down into three modules: Main, I/O, and Chess Engine. 

The main module performs the high level process of gameplay.
The I/O module implements low level device communication as well as providing a high level interface to the main module.
The chess engine maintains the state of the game and "thinks" of chess AI moves.

For more details on the project please check the "docs" folder. 

