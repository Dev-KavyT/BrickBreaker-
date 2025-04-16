#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <fstream>
#include <windows.h>
#include <mmsystem.h> // Include for PlaySound function
#include <time.h>
#pragma comment(lib, "winmm.lib")  // Include Windows API for Beep function

// Screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Paddle dimensions
 int PADDLE_WIDTH = 100;
 int PADDLE_HEIGHT = 20;

// Ball dimensions
const int BALL_SIZE = 20;

// Brick dimensions (reduced to half)
const int BRICK_WIDTH = 40;  // Reduced from 80
const int BRICK_HEIGHT = 15; // Reduced from 30

// Game variables
int paddleX, paddleY; // Paddle position
int ballX, ballY;     // Ball position
float ballSpeedMultiplier = 1.0;
int ballDirX, ballDirY; // Ball direction
bool gameOver;        // Game over flag
int lives = 3; // Number of lives
int brickHits[10][20];   // Brick grid (1 = exists, 0 = destroyed) - Increased to 10x20   
bool ballLaunched = false; // Tracks if the ball is launched
int highScore = 0;
float ballSpeedX = 2.5;  // Initial speed in X direction
float ballSpeedY = 2.5;  // Initial speed in Y direction
int score = 0;           // Initialize score
bool powerupActive = false;
int powerupTimer = 0;
const int POWERUP_DURATION = 15000; // in milliseconds
int originalPaddleWidth = PADDLE_WIDTH; 
struct PowerUp {
    int x, y;
    bool active;
};
PowerUp sizePowerUp = {0, 0, false};
int powerUpTimer = 0;     // Timer to track how long the paddle stays big
bool powerUpActive = false; 
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
            brickHits[i][j] = 2;
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
void pauseGame();
// Function to move the paddle
void movePaddle() {
    if (GetAsyncKeyState(VK_LEFT) & 0x8000 && paddleX > 0) {
        paddleX -= 10;
    }
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && paddleX + PADDLE_WIDTH < SCREEN_WIDTH) {
        paddleX += 10;
    }
    if (GetAsyncKeyState('P') & 0x8000) {
        pauseGame();
        return;
    }
    if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && !ballLaunched) {
        ballLaunched = true;
    }

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
void displayLives() {
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    setcolor(WHITE);
    char lifeText[20];
    sprintf(lifeText, "Lives: %d", lives);
    outtextxy(20, 20, lifeText);
}
void resetBallAndPaddle() {
    paddleX = (SCREEN_WIDTH - PADDLE_WIDTH) / 2;
    paddleY = SCREEN_HEIGHT - 50;

    ballX = paddleX + PADDLE_WIDTH / 2;
    ballY = paddleY - BALL_SIZE / 2;

    ballDirX = 4;
    ballDirY = -4;
    ballLaunched = false;
}

void updatePowerUp() {
    if (sizePowerUp.active) {
        sizePowerUp.y += 5; // Falling speed

        // Check if it hits the paddle
        if (sizePowerUp.y + 10 >= paddleY && sizePowerUp.y <= paddleY + PADDLE_HEIGHT &&
            sizePowerUp.x >= paddleX && sizePowerUp.x <= paddleX + PADDLE_WIDTH) {

                PADDLE_WIDTH += 40;
                powerUpTimer = 0;
                powerUpActive = true;
                sizePowerUp.active = false;
        }

        // If it falls off the screen
        if (sizePowerUp.y > SCREEN_HEIGHT) {
            sizePowerUp.active = false;
        }
    }
}
void updatePowerUpTimer() {
    if (powerUpActive) {
        powerUpTimer++;

        // Assuming your game loop runs ~60 times per second
        if (powerUpTimer > 60 *2) { // After 7 seconds
            PADDLE_WIDTH -= 40;      // Reset paddle size
            powerUpActive = false;
            powerUpTimer = 0;
        }
    }
}
void drawBricks() {
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 20; ++j) {
            if (brickHits[i][j] > 0) {
                int brickX = j * BRICK_WIDTH;
                int brickY = i * BRICK_HEIGHT;

                // Fill color based on hit count
                if (brickHits[i][j] == 2)
                    setfillstyle(SOLID_FILL, YELLOW);
                else if (brickHits[i][j] == 1)
                    setfillstyle(SOLID_FILL, RED);

                // Draw filled brick
                bar(brickX, brickY, brickX + BRICK_WIDTH, brickY + BRICK_HEIGHT);

                // Draw border (white or any other color)
                setcolor(BLACK);
                rectangle(brickX, brickY, brickX + BRICK_WIDTH, brickY + BRICK_HEIGHT);
            }
        }
    }
}

// Function to move the ball
void moveBall() {
    if (!ballLaunched || gameOver) return;

    ballX += ballDirX * ballSpeedX;
    ballY += ballDirY * ballSpeedY;

    // Wall collisions
    if (ballX <= BALL_SIZE || ballX >= SCREEN_WIDTH - BALL_SIZE)
        ballDirX = -ballDirX;
    if (ballY <= BALL_SIZE)
        ballDirY = -ballDirY;

    // Paddle collision
    if (ballY + BALL_SIZE >= paddleY &&
        ballX >= paddleX && ballX <= paddleX + PADDLE_WIDTH) {
        ballDirY = -ballDirY;
        ballY = paddleY - BALL_SIZE;
    }

    // Brick collision and power-up spawn logic
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 20; ++j) {
            if (brickHits[i][j] > 0) {
                int brickX = j * BRICK_WIDTH;
                int brickY = i * BRICK_HEIGHT;

                if (ballX + BALL_SIZE > brickX && ballX - BALL_SIZE < brickX + BRICK_WIDTH &&
                    ballY + BALL_SIZE > brickY && ballY - BALL_SIZE < brickY + BRICK_HEIGHT) {

                    brickHits[i][j]--;
                    ballDirY = -ballDirY;

                    if (brickHits[i][j] == 1) {
                        score += 1; // First hit (turns red)
                    } else if (brickHits[i][j] == 0) {
                        score += 2; // Second hit (breaks brick)

                        // 20% chance to drop power-up
                        if (!sizePowerUp.active && !powerUpActive && rand() % 5 == 0) {
                            sizePowerUp.x = brickX + BRICK_WIDTH / 2;
                            sizePowerUp.y = brickY + BRICK_HEIGHT;
                            sizePowerUp.active = true;
                        }
                    }

                    return; // Only handle one brick collision per frame
                }
            }
        }
    }

    // Power-up falling and catching
    if (sizePowerUp.active) {
        sizePowerUp.y += 5;

        if (sizePowerUp.y + 10 >= paddleY &&
            sizePowerUp.x >= paddleX && sizePowerUp.x <= paddleX + PADDLE_WIDTH) {

            PADDLE_WIDTH += 40;
            powerUpTimer = clock();
            powerUpActive = true;
            sizePowerUp.active = false;
        } else if (sizePowerUp.y > SCREEN_HEIGHT) {
            sizePowerUp.active = false;
        }
    }

    // Power-up timeout logic
    if (powerUpActive && (clock() - powerUpTimer >= POWERUP_DURATION)) {
        PADDLE_WIDTH = originalPaddleWidth;
        powerUpActive = false;
    }

    // Ball missed
    if (ballY >= SCREEN_HEIGHT) {
        lives--;
        if (lives <= 0) {
            gameOver = true;
            if (score > highScore) highScore = score;
        } else {
            ballLaunched = false;
            ballX = paddleX + PADDLE_WIDTH / 2;
            ballY = paddleY - BALL_SIZE;
        }
    }
}

// Function to check if all bricks are destroyed (win condition)
void checkWin() {
    bool allBricksDestroyed = true;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {
            if (brickHits[i][j] >0) {
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
    outtextxy(SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 + 50, (char*)"3. Mai Nai Khelraa");
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
    outtextxy(SCREEN_WIDTH/2-300, SCREEN_HEIGHT*2/3-15, (char*)"Ek Baap Ki Aulaad ho toh Firse aaja(Y/N)");
    
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
void pauseGame() {
    // Show "PAUSED" text in the center of the screen
    setactivepage(0);
    setvisualpage(0);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 5);
    setcolor(WHITE);
    outtextxy(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, (char*)"PAUSED");

    // Wait for 'P' key again to resume
    while (true) {
        if (kbhit()) {
            char key = getch();
            if (key == 'P' || key == 'p') {
                break; // Resume game
            }
        }
    }
   
}
void drawPowerUp() {
    if (sizePowerUp.active) {
        setfillstyle(SOLID_FILL, YELLOW);
        fillellipse(sizePowerUp.x, sizePowerUp.y, 10, 10); // Draw a small circle
    }
}


// Main function
int main() {
    PlaySoundA("back.wav", NULL, SND_ASYNC | SND_LOOP);

    // Initialize the graphics window at the start
    initwindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Bricks Breaker");
    
    loadHighScore(); // Load the high score at the start

    int page = 0; // Used for double buffering

    while (true) {
        // Check for the Esc key to exit the game at the start of the loop
        if (kbhit()) {
            char key = getch();
            if (key == 27) { // 27 is the ASCII value for the Esc key
                // Display "Game Over" message
                cleardevice();
                setcolor(WHITE);
                settextstyle(GOTHIC_FONT, HORIZ_DIR, 4);
                outtextxy(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, const_cast<char*>("Game Over"));
    
                delay(1000); // Give the player 1 second to see the message before exiting
    
                closegraph();  // Close the graphics window
                exit(0);       // Exit the program
            }
        }
    
        // Continue with the regular game process
        drawMainMenu();
        int choice = handleMainMenuInput();
    
        if (choice == 1) {
            // Start New Game
            initialize();
            gameOver = false;
            ballLaunched = false;
            score = 0;
    
            while (!gameOver) {
                // Check for the Esc key to exit during the game
                if (kbhit()) {
                    char key = getch();
                    if (key == 27) { // 27 is the ASCII value for the Esc key
                        // Display "Game Over" message
                        cleardevice();
                        setcolor(WHITE);
                        settextstyle(GOTHIC_FONT, HORIZ_DIR, 4);
                        outtextxy(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, const_cast<char*>("Game Over"));
    
                        delay(1000); // Give the player 1 second to see the message before exiting
    
                        closegraph();  // Close the graphics window
                        exit(0);       // Exit the game
                    }
                }
    
                setactivepage(page);
                setvisualpage(1 - page);
                cleardevice();
                readimagefile("bg.bmp", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                drawSawSurface(); // Draw saw-tooth ground
                drawPaddle();     // Draw normal paddle
                drawBall();
                drawBricks();
                drawScore();
                displayLives();
    
                movePaddle();
                moveBall();
                checkWin();
                drawPowerUp();
                updatePowerUp();
                updatePowerUpTimer();
                delay(5); // Reduced from 10 to 5 milliseconds for faster game
                page = 1 - page;
            }
    
            if (gameOver) {
                // Update high score if necessary
                if (score > highScore) {
                    highScore = score;
                    saveHighScore();
                }
    
                // Show the Game Over window after the game ends
                cleardevice();
                setcolor(WHITE);
                settextstyle(GOTHIC_FONT, HORIZ_DIR, 4);
                outtextxy(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, const_cast<char*>("Game Over"));
                delay(1000); // Display Game Over for 1 second
    
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
            // Exit the program
            break;
        }
    }
    closegraph();
    return 0;
}
