#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// OLED屏幕尺寸
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define i2c_Address 0x3c

// 按键引脚定义
#define BUTTON_UP 38
#define BUTTON_DOWN 35
#define BUTTON_LEFT 36
#define BUTTON_RIGHT 37
#define BUTTON_A 19
#define BUTTON_B 20

// 蜂鸣器引脚定义
const int buzzerPin1 = 5;
const int buzzerPin2 = 40;

// 游戏参数
#define GRID_SIZE 8
#define SNAKE_MAX_LENGTH 20

// 方向定义
enum Direction { UP, DOWN, LEFT, RIGHT };
Direction currentDirection = RIGHT;

// 蛇的结构
struct SnakeSegment {
  int x, y;
};

SnakeSegment snake[SNAKE_MAX_LENGTH];
int snakeLength = 3;

// 食物位置
int foodX, foodY;

// 游戏状态
bool gameOver = false;

// 初始化OLED屏幕

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// 初始化按键状态
int buttonStates[6] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};

void setup() {
   Wire.begin(41,42);
  // 初始化串口
  Serial.begin(115200);
  
  // 初始化OLED屏幕
  if (!display.begin(i2c_Address, true)) {
    Serial.println(F("SH1106 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.display();

  // 初始化按键引脚
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);

  // 初始化蜂鸣器引脚
  pinMode(buzzerPin1, OUTPUT);
  pinMode(buzzerPin2, OUTPUT);


  // 初始化蛇的位置
  snake[0].x = SCREEN_WIDTH / 2;
  snake[0].y = SCREEN_HEIGHT / 2;
  snake[1].x = snake[0].x - GRID_SIZE;
  snake[1].y = snake[0].y;
  snake[2].x = snake[1].x - GRID_SIZE;
  snake[2].y = snake[1].y;

  // 初始化食物位置
  spawnFood();

  // 显示初始画面
  display.clearDisplay();
  drawSnake();
  drawFood();
  display.display();
}

void loop() {
  
  // 读取按键状态
  readButtons();

  // 如果游戏结束，显示游戏结束画面
  if (gameOver) {
    displayGameOver();
    delay(1000);
    resetGame();
    return;
  }

  // 移动蛇
  moveSnake();

  // 检查是否吃到食物
  if (snake[0].x == foodX && snake[0].y == foodY) {
    // 增加蛇的长度
    if (snakeLength < SNAKE_MAX_LENGTH) {
      snake[snakeLength].x = snake[snakeLength - 1].x;
      snake[snakeLength].y = snake[snakeLength - 1].y;
      snakeLength++;
    }
    // 生成新的食物
    spawnFood();
    // 播放音效
    playSound();
  }

  // 显示蛇和食物
  display.clearDisplay();
  drawSnake();
  drawFood();
  display.display();

  // 延迟一段时间
  delay(100);
}

void readButtons() {
  buttonStates[0] = digitalRead(BUTTON_UP);
  buttonStates[1] = digitalRead(BUTTON_DOWN);
  buttonStates[2] = digitalRead(BUTTON_LEFT);
  buttonStates[3] = digitalRead(BUTTON_RIGHT);
  buttonStates[4] = digitalRead(BUTTON_A);
  buttonStates[5] = digitalRead(BUTTON_B);

  // 更新方向
  if (buttonStates[0] == LOW && currentDirection != DOWN) currentDirection = UP;
  if (buttonStates[1] == LOW && currentDirection != UP) currentDirection = DOWN;
  if (buttonStates[2] == LOW && currentDirection != RIGHT) currentDirection = LEFT;
  if (buttonStates[3] == LOW && currentDirection != LEFT) currentDirection = RIGHT;
}

void moveSnake() {
  // 移动蛇的身体
  for (int i = snakeLength - 1; i > 0; i--) {
    snake[i] = snake[i - 1];
  }

  // 移动蛇头
  switch (currentDirection) {
    case UP:
      snake[0].y -= GRID_SIZE;
      break;
    case DOWN:
      snake[0].y += GRID_SIZE;
      break;
    case LEFT:
      snake[0].x -= GRID_SIZE;
      break;
    case RIGHT:
      snake[0].x += GRID_SIZE;
      break;
  }

  // 检查是否撞墙或撞到自己
  if (snake[0].x < 0 || snake[0].x >= SCREEN_WIDTH || snake[0].y < 0 || snake[0].y >= SCREEN_HEIGHT) {
    gameOver = true;
  }
  for (int i = 1; i < snakeLength; i++) {
    if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
      gameOver = true;
    }
  }
}

void spawnFood() {
  foodX = random(0, SCREEN_WIDTH / GRID_SIZE) * GRID_SIZE;
  foodY = random(0, SCREEN_HEIGHT / GRID_SIZE) * GRID_SIZE;
}

void drawSnake() {
  for (int i = 0; i < snakeLength; i++) {
    display.fillRect(snake[i].x, snake[i].y, GRID_SIZE, GRID_SIZE, SH110X_WHITE);
  }
}

void drawFood() {
  display.fillRect(foodX, foodY, GRID_SIZE, GRID_SIZE, SH110X_WHITE);
}

void displayGameOver() {
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(SCREEN_WIDTH / 2 - 20, SCREEN_HEIGHT / 2 - 10);
  display.println("GAME OVER");
}

void resetGame() {
  snakeLength = 3;
  snake[0].x = SCREEN_WIDTH / 2;
  snake[0].y = SCREEN_HEIGHT / 2;
  snake[1].x = snake[0].x - GRID_SIZE;
  snake[1].y = snake[0].y;
  snake[2].x = snake[1].x - GRID_SIZE;
  snake[2].y = snake[1].y;
  currentDirection = RIGHT;
  gameOver = false;
  spawnFood();
}

void playSound() {
   tone(buzzerPin1, 1000);
   tone(buzzerPin2, 1000);
   delay(100);
   noTone(buzzerPin1);
   noTone(buzzerPin2); 
    delay(100);
}
