// =============================================
// RETRO ARCADE GAME CONSOLE - ESP32 + U8G2
// Games: Snake, Pong, Space Invaders
// Components: OLED (128x64), Joystick, Buttons, Buzzer, LEDs
// =============================================

// --- Libraries ---
#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

// --- OLED Setup (U8G2) ---
// For SSD1106 128x64 I2C:
// U8G2_SSD1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=/ U8X8_PIN_NONE, / clock=/ 22, / data=*/ 21);
// For SH1106 128x64 I2C:
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=/ U8X8_PIN_NONE, / clock=/ 22, / data=*/ 21);

// --- Pin Definitions ---
#define JOY_VRX 34    // Joystick X
#define JOY_VRY 35    // Joystick Y
#define JOY_SW  13    // Joystick Button
#define BTN1    12    // Action Button 1
#define BTN2    14    // Action Button 2
#define BUZZER  25    // Passive Buzzer
#define LED1    26    // LED 1
#define LED2    27    // LED 2

// --- Game States ---
enum Game { MENU, SNAKE, PONG, INVADERS };
Game currentGame = MENU;

// --- Joystick & Buttons ---
int joyX = 0, joyY = 0;
bool joySW = false, btn1 = false, btn2 = false;

// --- Buzzer Tones ---
#define NOTE_C4 262
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_B4 494

// =============================================
// SETUP
// =============================================
void setup() {
  Serial.begin(115200);

  // --- Initialize OLED (U8G2) ---
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(10, 20, "RETRO");
  u8g2.drawStr(10, 40, "ARCADE");
  u8g2.sendBuffer();

  // --- Initialize Pins ---
  pinMode(JOY_SW, INPUT_PULLUP);
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  // --- Play Startup Sound ---
  tone(BUZZER, NOTE_C4, 100);
  delay(150);
  tone(BUZZER, NOTE_E4, 100);
  delay(150);
  tone(BUZZER, NOTE_G4, 100);
  delay(150);
  noTone(BUZZER);
  delay(500);
}

// =============================================
// MAIN LOOP
// =============================================
void loop() {
  readInputs();
  switch(currentGame) {
    case MENU:     menuScreen();     break;
    case SNAKE:    snakeGame();      break;
    case PONG:     pongGame();       break;
    case INVADERS: invadersGame();   break;
  }
}

// =============================================
// INPUT HANDLING
// =============================================
void readInputs() {
  joyX = analogRead(JOY_VRX);
  joyY = analogRead(JOY_VRY);
  joySW = !digitalRead(JOY_SW);
  btn1 = !digitalRead(BTN1);
  btn2 = !digitalRead(BTN2);
}

// =============================================
// MENU SCREEN
// =============================================
void menuScreen() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawStr(20, 10, "SELECT A GAME:");
  u8g2.drawStr(20, 30, "1. SNAKE");
  u8g2.drawStr(20, 45, "2. PONG");
  u8g2.drawStr(20, 60, "3. INVADERS");

  // --- Highlight selected game ---
  if (joyY < 1000) { // Joystick up
    u8g2.drawStr(10, 30, ">");
    if (joySW || btn1) {
      currentGame = SNAKE;
      delay(200);
    }
  } else if (joyY > 3000) { // Joystick down
    u8g2.drawStr(10, 60, ">");
    if (joySW || btn1) {
      currentGame = INVADERS;
      delay(200);
    }
  } else {
    u8g2.drawStr(10, 45, ">");
    if (joySW || btn1) {
      currentGame = PONG;
      delay(200);
    }
  }

  u8g2.sendBuffer();
}

// =============================================
// SNAKE GAME
// =============================================
struct SnakeSegment {
  int x, y;
};

#define GRID_SIZE 8
#define GRID_WIDTH (128 / GRID_SIZE)
#define GRID_HEIGHT (64 / GRID_SIZE)

SnakeSegment snake[100];
int snakeLength = 3;
int snakeDirection = 0; // 0: up, 1: right, 2: down, 3: left
int foodX, foodY;
bool gameOver = false;

void initSnake() {
  for (int i = 0; i < snakeLength; i++) {
    snake[i].x = GRID_WIDTH / 2 - i;
    snake[i].y = GRID_HEIGHT / 2;
  }
  spawnFood();
  gameOver = false;
}

void spawnFood() {
  foodX = random(0, GRID_WIDTH);
  foodY = random(0, GRID_HEIGHT);
}

void snakeGame() {
  if (gameOver) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.drawStr(20, 30, "GAME OVER!");
    u8g2.drawStr(10, 50, "Press BTN1 to restart");
    u8g2.sendBuffer();
    if (btn1) {
      initSnake();
    }
    return;
  }

  // --- Read Input ---
  if (joyX < 1000 && snakeDirection != 1) snakeDirection = 3; // Left
  if (joyX > 3000 && snakeDirection != 3) snakeDirection = 1; // Right
  if (joyY < 1000 && snakeDirection != 2) snakeDirection = 0; // Up
  if (joyY > 3000 && snakeDirection != 0) snakeDirection = 2; // Down

  // --- Move Snake ---
  for (int i = snakeLength - 1; i > 0; i--) {
    snake[i] = snake[i - 1];
  }
  switch(snakeDirection) {
    case 0: snake[0].y--; break; // Up
    case 1: snake[0].x++; break; // Right
    case 2: snake[0].y++; break; // Down
    case 3: snake[0].x--; break; // Left
  }

  // --- Check Collisions ---
  // Wall collision
  if (snake[0].x < 0 || snake[0].x >= GRID_WIDTH || snake[0].y < 0 || snake[0].y >= GRID_HEIGHT) {
    gameOver = true;
    tone(BUZZER, NOTE_C4, 200);
    return;
  }
  // Self collision
  for (int i = 1; i < snakeLength; i++) {
    if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
      gameOver = true;
      tone(BUZZER, NOTE_C4, 200);
      return;
    }
  }
  // Food collision
  if (snake[0].x == foodX && snake[0].y == foodY) {
    snakeLength++;
    spawnFood();
    tone(BUZZER, NOTE_E4, 100);
  }

  // --- Draw ---
  u8g2.clearBuffer();
  // Draw snake
  for (int i = 0; i < snakeLength; i++) {
    u8g2.drawBox(snake[i].x * GRID_SIZE, snake[i].y * GRID_SIZE, GRID_SIZE, GRID_SIZE);
  }
  // Draw food
  u8g2.drawFrame(foodX * GRID_SIZE, foodY * GRID_SIZE, GRID_SIZE, GRID_SIZE);
  u8g2.sendBuffer();

  // --- Back to Menu ---
  if (btn2) {
    currentGame = MENU;
    delay(200);
  }

  delay(150);
}

// =============================================
// PONG GAME
// =============================================
int paddleY = 64 / 2 - 10;
int ballX = 128 / 2;
int ballY = 64 / 2;
int ballDX = 2, ballDY = 2;
int score = 0;

void initPong() {
  paddleY = 64 / 2 - 10;
  ballX = 128 / 2;
  ballY = 64 / 2;
  ballDX = 2; ballDY = 2;
  score = 0;
}

void pongGame() {
  // --- Read Input ---
  if (joyY < 1000 && paddleY > 0) paddleY -= 3; // Up
  if (joyY > 3000 && paddleY < 64 - 20) paddleY += 3; // Down

  // --- Move Ball ---
  ballX += ballDX;
  ballY += ballDY;

  // --- Ball Collisions ---
  // Top and bottom
  if (ballY <= 0 || ballY >= 64 - 4) {
    ballDY = -ballDY;
    tone(BUZZER, NOTE_G4, 50);
  }
  // Paddle
  if (ballX <= 5 && ballY >= paddleY && ballY <= paddleY + 20) {
    ballDX = -ballDX;
    score++;
    tone(BUZZER, NOTE_A4, 50);
  }
  // Left wall (miss)
  if (ballX <= 0) {
    initPong();
    tone(BUZZER, NOTE_C4, 200);
  }
  // Right wall
  if (ballX >= 128 - 4) {
    ballDX = -ballDX;
    tone(BUZZER, NOTE_E4, 50);
  }

  // --- Draw ---
  u8g2.clearBuffer();
  // Paddle
  u8g2.drawBox(0, paddleY, 4, 20);
  // Ball
  u8g2.drawBox(ballX, ballY, 4, 4);
  // Score
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.setCursor(60, 10);
  u8g2.print("Score: ");
  u8g2.print(score);
  u8g2.sendBuffer();

  // --- Back to Menu ---
  if (btn2) {
    currentGame = MENU;
    delay(200);
  }

  delay(30);
}

// =============================================
// SPACE INVADERS GAME
// =============================================
#define INVADER_WIDTH 8
#define INVADER_HEIGHT 8
#define PLAYER_WIDTH 8
#define PLAYER_HEIGHT 4

int playerX = 128 / 2 - PLAYER_WIDTH / 2;
int invaderX[5], invaderY[5];
int invaderDX = 1;
int bulletX = -1, bulletY = -1;
int invaderCount = 5;
bool invaderGameOver = false;

void initInvaders() {
  for (int i = 0; i < 5; i++) {
    invaderX[i] = i * 20 + 10;
    invaderY[i] = 10;
  }
  playerX = 128 / 2 - PLAYER_WIDTH / 2;
  bulletX = -1;
  invaderGameOver = false;
  invaderCount = 5;
}

void invadersGame() {
  if (invaderGameOver) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.drawStr(20, 30, "GAME OVER!");
    u8g2.drawStr(10, 50, "Press BTN1 to restart");
    u8g2.sendBuffer();
    if (btn1) {
      initInvaders();
    }
    return;
  }

  // --- Read Input ---
  if (joyX < 1000 && playerX > 0) playerX -= 3; // Left
  if (joyX > 3000 && playerX < 128 - PLAYER_WIDTH) playerX += 3; // Right
  if (joySW || btn1) {
    if (bulletX == -1) {
      bulletX = playerX + PLAYER_WIDTH / 2;
      bulletY = 64 - PLAYER_HEIGHT - 2;
      tone(BUZZER, NOTE_F4, 50);
    }
  }

  // --- Move Bullet ---
  if (bulletX != -1) {
    bulletY -= 3;
    if (bulletY < 0) bulletX = -1;
  }

  // --- Move Invaders ---
  bool moveDown = false;
  for (int i = 0; i < invaderCount; i++) {
    invaderX[i] += invaderDX;
    if (invaderX[i] <= 0 || invaderX[i] >= 128 - INVADER_WIDTH) {
      moveDown = true;
    }
  }
  if (moveDown) {
    invaderDX = -invaderDX;
    for (int i = 0; i < invaderCount; i++) {
      invaderY[i] += 5;
      if (invaderY[i] >= 64 - INVADER_HEIGHT) {
        invaderGameOver = true;
        tone(BUZZER, NOTE_C4, 200);
      }
    }
  }

  // --- Check Bullet-Invader Collision ---
  if (bulletX != -1) {
    for (int i = 0; i < invaderCount; i++) {
      if (bulletX >= invaderX[i] && bulletX <= invaderX[i] + INVADER_WIDTH &&
          bulletY >= invaderY[i] && bulletY <= invaderY[i] + INVADER_HEIGHT) {
        bulletX = -1;
        invaderX[i] = invaderX[invaderCount - 1];
        invaderY[i] = invaderY[invaderCount - 1];
        invaderCount--;
        tone(BUZZER, NOTE_B4, 100);
        if (invaderCount == 0) {
          initInvaders();
        }
      }
    }
  }

  // --- Draw ---
  u8g2.clearBuffer();
  // Player
  u8g2.drawBox(playerX, 64 - PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT);
  // Invaders
  for (int i = 0; i < invaderCount; i++) {
    u8g2.drawBox(invaderX[i], invaderY[i], INVADER_WIDTH, INVADER_HEIGHT);
  }
  // Bullet
  if (bulletX != -1) {
    u8g2.drawBox(bulletX, bulletY, 2, 4);
  }
  u8g2.sendBuffer();

  // --- Back to Menu ---
  if (btn2) {
    currentGame = MENU;
    delay(200);
  }

  delay(50);
}
