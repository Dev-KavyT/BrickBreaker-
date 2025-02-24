#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <fstream>
#include <windows.h>
#include <mmsystem.h> // Include for PlaySound function
#pragma comment(lib, "winmm.lib")  // Include Windows API for Beep function

// Screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Paddle dimensions
const int PADDLE_WIDTH = 100;
const int PADDLE_HEIGHT = 20;

// Ball dimensions
const int BALL_SIZE = 20;

// Brick dimensions (reduced to half)
const int BRICK_WIDTH = 40;  // Reduced from 80
const int BRICK_HEIGHT = 15; // Reduced from 30

// Game variables
int paddleX, paddleY; // Paddle position
int ballX, ballY;     // Ball position
int ballDirX, ballDirY; // Ball direction
bool gameOver;        // Game over flag
int bricks[10][20];   // Brick grid (1 = exists, 0 = destroyed) - Increased to 10x20   
bool ballLaunched = false; // Tracks if the ball is launched
int highScore = 0;
float ballSpeedX = 3.0;  // Initial speed in X direction
float ballSpeedY = 3.0;  // Initial speed in Y direction
int score = 0;           // Initialize score


// Function to load high score from a file
void loadHighScore() {
    std::ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> highScore;
        file.close();
    }
}

// Function to save high score to a file
void saveHighScore() {
    std::ofstream file("highscore.txt");
    if (file.is_open()) {
        file << highScore;
        file.close();
    }
}

// Function to initialize game variables
void initialize() {
    paddleX = (SCREEN_WIDTH - PADDLE_WIDTH)/2;
    paddleY = SCREEN_HEIGHT - 50;
    
    ballX = paddleX + PADDLE_WIDTH / 2; // Ball starts on the paddle
    ballY = paddleY - BALL_SIZE / 2;
    
    ballDirX = 4;  // Set ball direction, but don't move yet
    ballDirY = -4;

    // Initialize bricks
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {
            bricks[i][j] = 1;
        }
    }

    gameOver = false;
}

// Function to draw the paddle
void drawPaddle() {
    setcolor(RED);  // Set boundary color to red
    rectangle(paddleX, paddleY, paddleX + PADDLE_WIDTH, paddleY + PADDLE_HEIGHT);

    setfillstyle(SOLID_FILL, WHITE); // Fill paddle with white color
    floodfill(paddleX + 1, paddleY + 1, RED); // Fill inside with white
}

// Function to draw the ball
void drawBall() {
    setcolor(RED);
    setfillstyle(SOLID_FILL, RED);
    fillellipse(ballX, ballY, BALL_SIZE / 2, BALL_SIZE / 2);
}

// Function to draw the bricks
void drawBricks() {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {
            if (bricks[i][j] == 1) { // Only draw if the brick exists
                int brickX = j * BRICK_WIDTH;
                int brickY = i * BRICK_HEIGHT;

                // Adjust size for the upper brick (first row)
                if (i == 0) {
                    setcolor(YELLOW);
                    setfillstyle(SOLID_FILL, YELLOW);
                    bar(brickX, brickY, brickX + BRICK_WIDTH , brickY + BRICK_HEIGHT );
                } else {
                    setcolor(RED);
                    setfillstyle(SOLID_FILL, YELLOW);
                    bar(brickX, brickY, brickX + BRICK_WIDTH, brickY + BRICK_HEIGHT);
                }

                // Draw purple border around the brick
                 setcolor(MAGENTA); // MAGENTA is purple-like
                rectangle(brickX, brickY, brickX + BRICK_WIDTH, brickY + BRICK_HEIGHT);
            }
        }
    }
}

// Function to draw the saw-tooth ground
void drawSawSurface() {
    setcolor(RED);
    int numTeeth = 50; // Increased number of teeth for finer saw effect
    int toothWidth = SCREEN_WIDTH / numTeeth;
    int toothHeight = 10; // Shorter teeth for a finer look
    for (int i = 0; i < numTeeth; i++) {
        int x1 = i * toothWidth;
        int y1 = SCREEN_HEIGHT - 30;
        int x2 = x1 + toothWidth / 2;
        int y2 = y1 - toothHeight;
        int x3 = x1 + toothWidth;
        int y3 = y1;
        line(x1, y1, x2, y2);
        line(x2, y2, x3, y3);
    }
}

// Function to move the paddle
void movePaddle() {
    if (kbhit()) {
        char key = getch();
        if (key == 75 && paddleX > 0) { // Left arrow key
            paddleX -= 20;
        }
        if (key == 77 && paddleX + PADDLE_WIDTH < SCREEN_WIDTH) { // Right arrow key
            paddleX += 20;
        }
        if (key == 32 && !ballLaunched) { // Spacebar to launch
            ballLaunched = true;
        }
    }

    // If ball is not launched, move it with the paddle
    if (!ballLaunched) {
        ballX = paddleX + PADDLE_WIDTH / 2;
        ballY = paddleY - BALL_SIZE / 2;
    }
}

// Function to draw the score
void drawScore() {
    char scoreText[20];
    sprintf(scoreText, "Score: %d", score);
    setcolor(WHITE);
    outtextxy(20, SCREEN_HEIGHT / 2 - 10, scoreText); // Moved score display to middle-left
}

// Function to move the ball
void moveBall() {
    if (!ballLaunched) return; // Don't move if not launched

    ballX += ballDirX;
    ballY += ballDirY;

    // Ball collision with walls
    if (ballX <= 0 || ballX >= SCREEN_WIDTH) {
        ballDirX = -ballDirX;
    }
    if (ballY <= 0) {
        ballDirY = -ballDirY;
    }

    // Ball collision with paddle
    if (ballY + BALL_SIZE / 2 >= paddleY && ballX >= paddleX && ballX <= paddleX + PADDLE_WIDTH) {
        ballDirY = -ballDirY;
    }

   // Ball collision with bricks
for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 20; j++) {
        if (bricks[i][j] == 1) {
            int brickX = j * BRICK_WIDTH;
            int brickY = i * BRICK_HEIGHT;
            if (ballX >= brickX && ballX <= brickX + BRICK_WIDTH &&
                ballY >= brickY && ballY <= brickY + BRICK_HEIGHT) {
                bricks[i][j] = 0; // Destroy the brick
                ballDirY = -ballDirY; // Reverse ball direction
                score++; // Increase score when a brick is broken
                
                // Play sound when the ball hits a brick
                PlaySound(TEXT("hit.wav"), NULL, SND_FILENAME | SND_ASYNC);
                if (ballSpeedX > 0) ballSpeedX += 0.2;
                else ballSpeedX -= 0.2;
    
                if (ballSpeedY > 0) ballSpeedY += 0.2;
                else ballSpeedY -= 0.2;
    
                // Limit max speed
                if (ballSpeedX > 10) ballSpeedX = 10;
                if (ballSpeedY > 10) ballSpeedY = 10;
    
                // Reverse ball direction after collision
                ballSpeedY = -ballSpeedY;
    
                break;
            }
        }
    }
}


    if (ballY >= SCREEN_HEIGHT) {
        PlaySound(TEXT("gameover.wav"), NULL, SND_FILENAME | SND_ASYNC); // Play game over sound
        gameOver = true;
    }
}

// Function to check if all bricks are destroyed (win condition)
void checkWin() {
    bool allBricksDestroyed = true;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {
            if (bricks[i][j] == 1) {
                allBricksDestroyed = false;
                break;
            }
        }
    }
    if (allBricksDestroyed) {
        gameOver = true;
        outtextxy(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2, (char*)"You Win!");

    }
}

// Function to draw the main menu
void drawMainMenu() {
    cleardevice();
    setcolor(WHITE);
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 4);
    outtextxy(SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 - 100, (char*)"Main Menu");
    outtextxy(SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 - 50, (char*)"1. Start New Game");
    outtextxy(SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2, (char*)"2. View High Score");
    outtextxy(SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 + 50, (char*)"3. I AI'NT PLAYING");
}

// Function to handle main menu input
int handleMainMenuInput() {
    while (true) {
        if (kbhit()) {
            char key = getch();
            if (key == '1') {
                return 1; // Start New Game
            } else if (key == '2') {
                return 2; // View High Score
            } else if (key == '3') {
                return 3; // Exit
            }
        }
    }
}

// Function to show the Game Over window
void showGameOverWindow() {
    // Close current window and open a new one for game over
    closegraph();
    
    // Create a new window for game over screen - same size as main window
    int gameOverWindow = initwindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game Over");
    
    // Fill the entire window with a dark gray background
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    // Draw a red border around the entire window
    setcolor(RED);
    rectangle(10, 10, SCREEN_WIDTH-10, SCREEN_HEIGHT-10);
    
    // Create black background for "GAME OVER!" text
    setfillstyle(SOLID_FILL, BLACK);
    bar(SCREEN_WIDTH/2-150, SCREEN_HEIGHT/3-20, SCREEN_WIDTH/2+150, SCREEN_HEIGHT/3+20);
    
    // Display "GAME OVER!" text
    setcolor(RED);
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 4);
    outtextxy(SCREEN_WIDTH/2-140, SCREEN_HEIGHT/3-15, (char*)"GAME OVER!");
    
    // Create black background for score text
    setfillstyle(SOLID_FILL, BLACK);
    bar(SCREEN_WIDTH/2-150, SCREEN_HEIGHT/2-30, SCREEN_WIDTH/2+150, SCREEN_HEIGHT/2);
    
    // Display the player's score
    char scoreText[50];
    sprintf(scoreText, "Your Score: %d", score);
    setcolor(WHITE);
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 3);
    outtextxy(SCREEN_WIDTH/2-140, SCREEN_HEIGHT/2-25, scoreText);
    
    // Create black background for high score text
    setfillstyle(SOLID_FILL, BLACK);
    bar(SCREEN_WIDTH/2-150, SCREEN_HEIGHT/2+10, SCREEN_WIDTH/2+150, SCREEN_HEIGHT/2+40);
    
    // Display the high score
    char highScoreText[50];
    sprintf(highScoreText, "High Score: %d", highScore);
    outtextxy(SCREEN_WIDTH/2-140, SCREEN_HEIGHT/2+15, highScoreText);
    
    // Create button-like area for "Play Again?" text
    setcolor(WHITE);
    rectangle(SCREEN_WIDTH/2-150, SCREEN_HEIGHT*2/3-20, 
              SCREEN_WIDTH/2+150, SCREEN_HEIGHT*2/3+20);
    
    // Create black background for "Play Again?" text
    setfillstyle(SOLID_FILL, BLACK);
    bar(SCREEN_WIDTH/2-148, SCREEN_HEIGHT*2/3-18, 
        SCREEN_WIDTH/2+148, SCREEN_HEIGHT*2/3+18);
    
    // Display "Play Again? (Y/N)" text
    outtextxy(SCREEN_WIDTH/2-300, SCREEN_HEIGHT*2/3-15, (char*)"LETS HIT THE BRICK AGAIN(Y/N)");
    
    // Wait for player's response
    while (true) {
        if (kbhit()) {
            char key = getch();
            if (key == 'Y' || key == 'y') {
                // Close the game over window
                closegraph();
                
                // Re-initialize the main game window
                initwindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Bricks Breaker");
                
                // Start a new game
                initialize();
                gameOver = false;
                ballLaunched = false;
                score = 0;
                break;
            } else if (key == 'N' || key == 'n') {
                // Exit the game
                closegraph();
                exit(0);
            }
        }
    }
}

// Main function
int main() {
    // Initialize the graphics window at the start
    initwindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Bricks Breaker");

    loadHighScore(); // Load the high score at the start

    int page = 0; // Used for double buffering

    while (true) {
        drawMainMenu();
        int choice = handleMainMenuInput();

        if (choice == 1) {
            // Start New Game
            initialize();
            gameOver = false;
            ballLaunched = false;
            score = 0;

            while (!gameOver) {
                setactivepage(page);
                setvisualpage(1 - page);
                cleardevice();

                drawSawSurface(); // Draw saw-tooth ground
                drawPaddle();     // Draw normal paddle
                drawBall();
                drawBricks();
                drawScore();

                movePaddle();
                moveBall();
                checkWin();

                delay(5); // Reduced from 10 to 5 milliseconds
                page = 1 - page;
            }

            if (gameOver) {
                // Update high score if necessary
                if (score > highScore) {
                    highScore = score;
                    saveHighScore();
                }

                // Show the Game Over window
                showGameOverWindow();
            }

        } else if (choice == 2) {
            // View High Score
            cleardevice();
            setcolor(WHITE);
            settextstyle(GOTHIC_FONT, HORIZ_DIR, 4);
            char highScoreText[50];
            sprintf(highScoreText, "High Score: %d", highScore);
            outtextxy(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, highScoreText);
            getch();
        } else if (choice == 3) {
            // Exit
            break;
        }
    }

    closegraph();
    return 0;
}