
# BrickBreaker

BrickBreaker is a classic brick breaker game implemented in C++ using the BGI graphics library. It features retro-style gameplay with paddle and ball physics, sound effects, and high score tracking.


## Features



- Classic Gameplay: Bounce a ball off a paddle to break a grid of bricks.
- Smooth Graphics: Rendered using the graphics.h library with double buffering for smooth animation.
- Sound Effects: Audio feedback with sound effects when the ball hits bricks and when the game is over.
- High Score Tracking: Automatically saves and loads the high score from a file (highscore.txt).
- Menu System: Main menu to start a new game, view the high score, or exit.
- Dynamic Brick Layout: Bricks arranged in a 10×20 grid with color variations.
- Ball Physics: Realistic collision detection with walls, paddle, and bricks with incremental speed increases.
- Saw-Tooth Ground Effect: A unique visual detail at the bottom of the screen.


## Requirements

- Platform: Windows (this project uses Windows-specific libraries like windows.h and mmsystem.h).
- Compiler: A C++ compiler that supports the BGI graphics library (e.g., Code::Blocks configured with WinBGIm).
- Libraries/Dependencies:
    - graphics.h
    - conio.h
    - windows.h
    - mmsystem.h (link with winmm.lib)


Make sure the following files are present in the executable’s working directory:

- hit.wav
- gameover.wav
- background.wav (if used)
- highscore.txt (for storing high scores)
## Installation

-  Clone the Repository:

``` bash
git clone https://github.com/leninLD/BrickBreaker.git
```
- Open the Project:

Open the project in your preferred C++ IDE configured to use the BGI graphics library (e.g., Code::Blocks with WinBGIm).

- Configure the Build:

    - Set up the include and library paths for graphics.h.
    - Link against winmm.lib to enable sound functionality.
- Compile the Project:

Build the project using your IDE’s build command.
## Usage

- Launching the Game:

- Run the compiled executable. The main menu appears with three options:

Start New Game
View High Score
Exit
- Controls:

    - Left Arrow Key: Move the paddle left.
    - Right Arrow Key: Move the paddle right.
    - Spacebar: Launch the ball (if not already launched).
- Gameplay:

- The ball bounces off the walls, paddle, and bricks.
- Each brick hit destroys the brick, increases your score, and slightly increases the ball’s speed.
- The game ends when the ball falls below the paddle or when all bricks are destroyed.
- A Game Over screen displays your score and the high score.

## Future Improvements

- Add multiple levels with varying brick layouts and increased difficulty.
- Enhance visual effects with detailed sprites and animations.
- Introduce power-ups or special bricks.
- Refine collision physics for even more dynamic gameplay.