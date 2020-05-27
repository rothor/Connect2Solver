# Connect 2 Solver
This program solves puzzles in the game [Connect2](https://www.kongregate.com/games/sadistfairy/connect-2). This program is written in C++, and built with Visual Studio 2019. This program also emulates the game Connect2, so it can be used to play the game normally and create your own levels.

License: [The Unlicense](https://unlicense.org)

## Building
- Load and build the project in Visual Studio 2019. Copy the included 'sqlite.dll' file and 'levels' folder to the same place as the built executable, and then run it. Alternatively, pre-built binaries are located in the 'bin' folder.
- This program is a console application, so you use it by typing commands in the console window. Type "help" for a list of commands.
- Level information is located in the 'levels' folder, and is accessed at run time. The level information is stored in plain text files, so levels can be edited easily. New levels can be added by simply creating a new text file in this folder.

## Basic commands
- By default level 1 is loaded on program start. Load a level by typing "load x", where x is from 1 to 30. If you want to load a custom level, type the name of the text file you created instead.
- After loading a level, you can play it just like the actual game. To move a path type the path number followed by the direction letter, with no space. The directions are: up=e, down=d, left=s, right=f. For example, if a level has 2 paths, to move the first path up type "1e", and to move the second path right type "2f".
- To reset the level, type "t".
- To attempt to find solutions for the level (from the current board state), type "z". You should reset the level before doing this.s
- To attempt to find possible end states for a level (what the level looks like after it has been solved), type "y". If a program can't solve a level, this is the best alternative.
- These are the basic commands. All other commands are listed by typing "help".
