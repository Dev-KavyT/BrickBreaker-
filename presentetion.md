
# Brick Breaker Game (C++ with Graphics)

This project is a **Brick Breaker** game developed in **C++** using the **graphics.h** library. It simulates the classic arcade-style brick-breaking gameplay with added features and improvements. The game includes a graphical user interface, sound effects, power-ups, multiple lives, and a clean pause-resume system.

---

## 🎮 Game Features

- 🎯 **Paddle and Ball Mechanics**: Move the paddle using the keyboard to bounce the ball and break bricks.
- 🧱 **Bricks with 2-Hit Durability**: Bricks require two hits to break completely, adding difficulty and strategy.
- ❤️ **Three Lives System**: The player gets 3 chances to win the game. Lives are displayed on the screen.
- ⚡ **Power-Ups**: Power-ups occasionally fall from broken bricks and enhance gameplay (e.g., bigger paddle).
- 🚀 **Dynamic Speed**: Ball speed dynamically increases as the game progresses for an added challenge.
- ⏸️ **Pause Functionality**: Press 'P' anytime to pause and resume the game.
- 🎵 **Background Music**: Looping sound enhances immersion.
- 🛑 **Escape Key to Exit**: Pressing Esc at any point exits the game cleanly with a message.
- 🧠 **High Score Saving**: Tracks and saves high scores across sessions.

---

## Pictures and ScreenShots of the game
![Screenshot 2025-04-12 134659](https://github.com/user-attachments/assets/896e65c2-6975-40c6-bdf5-1b4e44a43e58)
![Screenshot 2025-04-11 210838](https://github.com/user-attachments/assets/75e67071-d8d7-484d-a343-8cd4236acc74)
![Screenshot 2025-04-11 211000](https://github.com/user-attachments/assets/6f732dd4-ab76-4d96-b2fb-2d5467a68756)


## 🛠️ Concepts & Programming Techniques

This project showcases several key programming and game development concepts:

- **Object-Oriented Programming (OOP)**:
  - Organized using classes for Paddle, Ball, Brick, and PowerUps.
  - Concepts like **Encapsulation** and **Abstraction** are used to manage the complexity.

- **Graphics Programming**:
  - Uses `graphics.h` functions like `setcolor`, `fillellipse`, `readimagefile`, and `outtextxy` for visuals.
  - Implements **double buffering** using `setactivepage` and `setvisualpage` to prevent flickering.

- **Real-time Input Handling**:
  - Uses `kbhit()` and `getch()` for non-blocking user inputs.

- **File Handling**:
  - `fopen`, `fprintf`, and `fscanf` are used for storing and retrieving the high score.

- **Sound Integration**:
  - Uses `PlaySoundA()` to include looping background music.

- **Game Loop**:
  - Core logic wrapped inside a continuous loop for real-time rendering and interaction.

---

## 🧩 Key Functions Explained

### `initialize()`
Initializes the paddle, ball, bricks, and lives. Called before starting or restarting a game.

### `drawMainMenu()` and `handleMainMenuInput()`
Displays the main menu and takes the user’s input for starting a game, viewing high score, or exiting.

### `movePaddle()` and `drawPaddle()`
Handles user input to move the paddle and draws it on screen.

### `moveBall()` and `drawBall()`
Updates the ball’s position based on velocity and collisions. Also renders the ball on the screen.

### `drawBricks()` and `initializeBricks()`
Manages the brick grid and draws it. Each brick has a `strength` that reduces with every hit.

### `checkCollision()`
Checks ball collision with walls, paddle, and bricks. Handles bounce logic and brick destruction.

### `drawPowerUp()` and `updatePowerUp()`
Displays the power-up when active and updates its position and effect.

### `pauseGame()`
Displays a "PAUSED" message and waits until the user presses 'P' again to resume gameplay.

### `showGameOverWindow()`
Displays the final score and a “Game Over” screen.

### `saveHighScore()` and `loadHighScore()`
Handles persistent high score saving/loading using file I/O.

---

## ✨ New Features Added

> We have significantly upgraded the base Brick Breaker game with the following enhancements:

1. **Pause/Resume Functionality**  
   - Press 'P' anytime during the game to pause and unpause.

2. **Clean Exit Using Escape Key**  
   - Press 'Esc' during the game or menu to exit gracefully with a message.

3. **Three Lives System**  
   - You now get 3 lives before the game ends, shown visually on screen.

4. **Falling Power-Up**  
   - Occasionally, bricks drop a power-up that enhances paddle size.

5. **Dynamic Ball Speed**  
   - The ball gets faster as time progresses, increasing game difficulty.

6. **Two-Hit Bricks**  
   - Bricks now require 2 hits to break completely, tracked via a strength variable.

---

## 🖼️ Assets Used

- `bg.bmp` – Background image for the game screen.
- `back.wav` – Background music played during gameplay.

---

## ▶️ How to Run

1. Make sure you have a C++ compiler with `graphics.h` support (like Turbo C++ or using WinBGIm in Code::Blocks).
2. Place all asset files (`bg.bmp`, `back.wav`) in the same directory as the `.cpp` file.
3. Compile and run the program.
4. Use the following keys:
   - **Arrow Keys** to move paddle
   - **P** to Pause/Resume
   - **Esc** to Exit
   - **Enter** to select menu options

---
## 🧠 Data Structures Used in the Game

---

### 1. Arrays
- **Purpose:** Used to represent the grid of bricks with a 2D array (e.g., `bricks[ROWS][COLS]`).
- **Why:** Arrays provide efficient indexing and make it easy to access, update, or remove brick information during gameplay.

---

### 2. Structs
- **Purpose:** To group related properties of game objects such as the ball, paddle, and power-ups.
- **Example:**
  ```cpp
  struct Ball {
      int x, y;       // Position
      int dx, dy;     // Velocity
      int radius;
  };

  struct PowerUp {
      int x, y;
      bool active;
      int type;
  };
  ```
- **Why:** Structs improve modularity and organization of code, allowing each game entity to be handled cleanly.

---

### 3. Primitive Data Types (int, bool, char, float)
- **Purpose:** To manage control flags (`gameOver`, `ballLaunched`), positions, scores, and user input.
- **Why:** Simple and efficient for handling essential game state.

---

### 4. File Handling with Character Buffers
- **Purpose:** Used to load and store high scores using text files.
- **Why:** Enables persistent high score tracking between game sessions using functions like `fopen`, `fprintf`, `fscanf`, etc.

---

### 5. Timers and Counters
- **Purpose:** Used for tracking power-up durations, game speed, and switching animation frames.
- **Why:** Helps implement time-based game logic and enhances gameplay dynamics.

## Future Implications
- Future Work
There are several future enhancements we plan to implement in the Brick Breaker game:

- Upgrade Graphics Library: We aim to upgrade the current graphics library used in the game for better performance, more complex visual effects, and smoother rendering.
This could involve transitioning to more advanced graphics libraries that support modern graphics technologies.
- Different Power-ups: We would like to introduce a variety of power-ups, such as:
- Speed Boost: Temporarily increases the speed of the ball.
- Multi-ball: Splits the ball into multiple smaller balls for more challenging gameplay.
- Extended Paddle: Temporarily increases the size of the paddle, making it easier to catch the ball.
- Various Grid Shapes: Currently, the grid for breaking bricks is static. We plan to implement different grid shapes
(e.g., circular, triangular, or irregular shapes) to add variety and increase the game's complexity.
- Game Levels: Introduce multiple levels with increasing difficulty. Each level can have different brick arrangements, power-ups, and obstacles.
- Sound Effects and Background Music: While basic sound is already included, 
we plan to add more sound effects and a more immersive background soundtrack to enhance the gaming experience.
- Mobile Compatibility: In the future, we plan to explore the possibility of adapting the game for mobile devices, making it accessible on Android and iOS platforms.

## 🧑‍💻 Devlopers

- 👤 [Kavya Halani]
- 👥 [Ishit Shripal]
- 👥 [Vrinda Goradia]
- 👥 [Riya Soni]

---

Enjoy the game and feel free to build on top of it!
