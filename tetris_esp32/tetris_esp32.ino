#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128   
#define SCREEN_HEIGHT 64   
#define OLED_RESET   -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const uint8_t COLS      = 10;
const uint8_t ROWS      = 20;
const uint8_t CELL_SIZE = 5;

const uint8_t TOP_OFFSET  = 27;  
const uint8_t SCREEN_W_V  = 64;  

uint8_t grid[ROWS][COLS];

uint8_t currentShape[4][4];
int currentX;
int currentY;
int currentPieceIndex;

long score = 0;
unsigned long lastFall = 0;
unsigned long fallInterval = 500;

bool gameOver = false;

const int BTN_LEFT   = 32;
const int BTN_RIGHT  = 33;
const int BTN_ROTATE = 25;


const uint8_t TETROMINOS[7][4][4] = {
  {
    {0,0,0,0},
    {1,1,1,1},
    {0,0,0,0},
    {0,0,0,0}
  },
  {
    {1,0,0,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0}
  },
  {
    {0,0,1,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0}
  },
  {
    {0,1,1,0},
    {0,1,1,0},
    {0,0,0,0},
    {0,0,0,0}
  },
  {
    {0,1,1,0},
    {1,1,0,0},
    {0,0,0,0},
    {0,0,0,0}
  },
  {
    {1,1,0,0},
    {0,1,1,0},
    {0,0,0,0},
    {0,0,0,0}
  },
  {
    {0,1,0,0},
    {1,1,1,0},
    {0,0,0,0},
    {0,0,0,0}
  }
};

void copyPiece(uint8_t dest[4][4], const uint8_t src[4][4]) {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      dest[y][x] = src[y][x];
    }
  }
}

void rotateShape(uint8_t shape[4][4]) {
  uint8_t tmp[4][4];
  for (int y = 0; y < 4; y++)
    for (int x = 0; x < 4; x++)
      tmp[x][3 - y] = shape[y][x];

  for (int y = 0; y < 4; y++)
    for (int x = 0; x < 4; x++)
      shape[y][x] = tmp[y][x];
}

bool checkCollision(int newX, int newY, uint8_t shape[4][4]) {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (shape[y][x] == 0) continue;

      int gx = newX + x;
      int gy = newY + y;

      if (gx < 0 || gx >= COLS || gy >= ROWS) return true;
      if (gy >= 0 && grid[gy][gx] != 0) return true;
    }
  }
  return false;
}

void fixPiece() {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (currentShape[y][x] == 0) continue;
      int gx = currentX + x;
      int gy = currentY + y;
      if (gy >= 0 && gy < ROWS && gx >= 0 && gx < COLS)
        grid[gy][gx] = 1;
    }
  }
}

int clearLines() {
  int linesCleared = 0;

  for (int y = ROWS - 1; y >= 0; y--) {
    bool full = true;
    for (int x = 0; x < COLS; x++) {
      if (grid[y][x] == 0) {
        full = false;
        break;
      }
    }

    if (full) {
      for (int yy = y; yy > 0; yy--)
        for (int x = 0; x < COLS; x++)
          grid[yy][x] = grid[yy - 1][x];

      for (int x = 0; x < COLS; x++) grid[0][x] = 0;
      linesCleared++;
      y++;
    }
  }

  return linesCleared;
}

void spawnNewPiece() {
  currentPieceIndex = random(0, 7);
  copyPiece(currentShape, TETROMINOS[currentPieceIndex]);
  currentX = (COLS / 2) - 2;
  currentY = 0;

  if (checkCollision(currentX, currentY, currentShape))
    gameOver = true;
}

void resetGame() {
  for (int y = 0; y < ROWS; y++)
    for (int x = 0; x < COLS; x++)
      grid[y][x] = 0;

  score = 0;
  fallInterval = 500;
  gameOver = false;
  spawnNewPiece();
}

void drawGame() {
  display.clearDisplay();

  display.drawRect(0, 0, 64, 128, SSD1306_WHITE);

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  String s = String(score);
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(s, 0, 0, &x1, &y1, &w, &h);
  int scoreX = (64 - w) / 2;
  display.setCursor(scoreX, 2);
  display.print(s);

  display.drawLine(1, 25, 62, 25, SSD1306_WHITE);

  int gridWidth = COLS * CELL_SIZE;
  int offsetX = (64 - gridWidth) / 2;
  int offsetY = TOP_OFFSET;

  for (int y = 0; y < ROWS; y++)
    for (int x = 0; x < COLS; x++)
      if (grid[y][x])
        display.fillRect(offsetX + x * CELL_SIZE, offsetY + y * CELL_SIZE, CELL_SIZE, CELL_SIZE, SSD1306_WHITE);

  // pièce en chute
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (currentShape[y][x] == 0) continue;
      int gx = currentX + x;
      int gy = currentY + y;
      if (gy < 0) continue;
      display.fillRect(offsetX + gx * CELL_SIZE, offsetY + gy * CELL_SIZE, CELL_SIZE, CELL_SIZE, SSD1306_WHITE);
    }
  }

  if (gameOver) {
    display.setTextSize(1);
    const char *msg = "GAME OVER";
    display.getTextBounds(msg, 0, 0, &x1, &y1, &w, &h);
    display.setCursor((64 - w) / 2, 60);
    display.print(msg);

    const char *msg2 = "Press ROT";
    display.getTextBounds(msg2, 0, 0, &x1, &y1, &w, &h);
    display.setCursor((64 - w) / 2, 72);
    display.print(msg2);
  }

  display.display();
}

void setup() {
  Serial.begin(115200);

  pinMode(BTN_LEFT,   INPUT_PULLUP);
  pinMode(BTN_RIGHT,  INPUT_PULLUP);
  pinMode(BTN_ROTATE, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    while (true) delay(100);

  display.setRotation(1);
  display.clearDisplay();
  display.display();

  randomSeed(analogRead(0));
  resetGame();
}

void loop() {
  static unsigned long lastButtonTime = 0;
  unsigned long now = millis();

  if (!gameOver) {
    if (digitalRead(BTN_LEFT) == LOW && now - lastButtonTime > 120) {
      if (!checkCollision(currentX - 1, currentY, currentShape))
        currentX--;
      lastButtonTime = now;
    }

    if (digitalRead(BTN_RIGHT) == LOW && now - lastButtonTime > 120) {
      if (!checkCollision(currentX + 1, currentY, currentShape))
        currentX++;
      lastButtonTime = now;
    }

    if (digitalRead(BTN_ROTATE) == LOW && now - lastButtonTime > 150) {
      uint8_t backup[4][4];
      copyPiece(backup, currentShape);
      rotateShape(currentShape);
      if (checkCollision(currentX, currentY, currentShape))
        copyPiece(currentShape, backup);
      lastButtonTime = now;
    }

    if (now - lastFall >= fallInterval) {
      lastFall = now;

      if (!checkCollision(currentX, currentY + 1, currentShape)) {
        currentY++;
      } else {
        fixPiece();

        int lines = clearLines();
        if (lines > 0) {
          score += lines * 100;
          if (fallInterval > 120)
            fallInterval -= 10 * lines;
        }

        spawnNewPiece();
      }
    }

  } else {
    if (digitalRead(BTN_ROTATE) == LOW && now - lastButtonTime > 300) {
      resetGame();
      lastButtonTime = now;
    }
  }

  drawGame();
}
