#include "SPI.h"
#include <Adafruit_GFX.h>
#include <ILI9488.h>

#define TFT_CS         8
#define TFT_DC         10
#define TFT_RST        9

#define TFT_CS2        4
#define TFT_RST2       7

ILI9488 tft = ILI9488(TFT_CS, TFT_DC, TFT_RST);
ILI9488 tft2 = ILI9488(TFT_CS2, TFT_DC, TFT_RST2);

int board[10][10] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //the set playing board for player 1
                     {0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
                     {0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
                     {0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
                     {0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
                     {0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
                     {0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
                     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                     {0, 0, 0, 0, 0, 1, 1, 0, 0, 0}};

int board2[10][10] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //the set playing board for player 2
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
                      {0, 0, 0, 1, 1, 0, 0, 0, 1, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

int oBoard[10][10] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //what player 2 will shoot at (note how all of the squares are obfuscated)
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

int oBoard2[10][10] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //what player 1 will shoot at
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

int i = 0; //this represents the turn: 0 for player 1 and 1 for player 2

int c1[2] = {1,1}; //the two vectors indicating the location of the cursors
int c2[2] = {1,1};

int j1x = 0; // the following 6 variables store the data from the joysticks
int j1y = 0; // x,y and mB
int j1m = 0;

int j2x = 0;
int j2y = 0;
int j2m = 0;

int hitsOne = 0; //these count the hits each player has done
int hitsTwo = 0;

//movement function: takes input from joysticks and returns 1 or -1 based on up or down
int m(int x){
  if (x > 650){
    return(1);
  }
  else if (x < 450){
    return(-1);
  }
  else{
    return(0);
  }
}

//translate function for the cursor (player 1)
void t(int x, int y, int b[10][10], int c[2], ILI9488 B){
  drawUnit(c[0]*30, c[1]*30, -1, B); //overwrites with black
  drawUnit(c[0]*30, c[1]*30, b[c[0]][c[1]], B); //redraws square with potentially new value
  if(x == 1 and c[0] <= 8){
    c[0] = c[0] + 1;
  }else if(y == 1 and c[1] <= 8){
    c[1] = c[1] + 1;
  }else if(x == -1 and c[0] >= 2){
    c[0] = c[0] - 1;
  }else if(y == -1 and c[1] >= 2){
    c[1] = c[1] - 1;
  }else if(x and y == 0){
    c[0] = c[0];
  }
  drawUnit(c[0]*30, c[1]*30, 3, B); //this actually draws the cursor
}

//translate function for the cursor (player 2)

//shoot the ships (Midway moment)
int shoot(int b[10][10], int b2[10][10], int c[2]){ // b is the board being shot, b2 is the obfuscated version of b
  if(b[c[0]][c[1]] == 1){
     b[c[0]][c[1]] = 2;
     b2[c[0]][c[1]] = 2;
     return(1);
  }
  else if(b[c[0]][c[1]] == 0){
    //b[c[0]][c[1]] = b[c[0]][c[1]];
    //b[c[0]][c[1]] = 4;
    b2[c[0]][c[1]] = 4;
    return(0);
  }
  else{
    return(2);
  }
}

//rendering:
void drawUnit(int x, int y, int t,ILI9488 s) { //the function to draw a single unit 
  if (t == 0) { 
    s.fillRect(x, y, 15, 15, 10); //blue square: water
  }
  else if (t == 1){
    s.fillCircle(x+7, y+7, 7, 0x55CF); //blue circle: ship
  }
  else if (t == 2){
    s.fillTriangle(x+15, y+8.66, x, y+17.33, x, y, 6666666); //red triangle: hit ship
  }
  else if (t == 3){
    s.drawCircle(x+7, y+7, 7, 4550455); //cyan circle: cursor
  }
  else if (t == 4){
    s.fillCircle(x+7, y+7, 7, 0xFFFF); //white circle: missed shot
  }
  else if (t == -1){
    s.fillRect(x, y, 20, 20, 0); //(black square)empty space
  }
}

//draws the board
void drawBoard(int B[10][10], ILI9488 s){
  for (int i=1; i < 10; i++){
    for (int j=1; j < 10; j++){
      drawUnit(i*30, j*30, -1, s);
      drawUnit(i*30, j*30, B[i][j], s);
    }
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("LET THE GAMES BEGIN");

  tft.begin(); //initializes both screens
  tft2.begin();

  pinMode(2, INPUT_PULLUP); //prepares the middle buttons for the joysticks
  pinMode(12, INPUT_PULLUP);

  tft.setRotation(3); //rotates screens 90 degrees
  tft2.setRotation(3);
  
  tft.fillScreen(ILI9488_BLACK); //starting off the game with a black background
  tft2.fillScreen(ILI9488_BLACK);
  
  drawBoard(oBoard2, tft); //drawing the intial game boards
  drawBoard(board2, tft2);

  //tft.fillScreen(ILI9488_WHITE);
  //tft2.fillScreen(ILI9488_WHITE);

}


void loop(void) {

  //Player 1 move and shoot
  //check if win
  //Player 2 move and shoot
  //check if win
  j1x = analogRead(A0);
  j1y = analogRead(A1);
  j1m = digitalRead(2);

  j2x = analogRead(A2);
  j2y = analogRead(A3);
  j2m = digitalRead(12);

  if(i == 0){ //the integer i is read. '0' indicates player 1's turn, '1' indicates player 2's turn.
    t(m(j1x), m(j1y), oBoard2, c1, tft);
    t(m(j2x), m(j2y), board2, c2, tft2);

    if (j1m == 0){
      if(shoot(board2, oBoard2, c1) == 1){
        hitsOne++;
        drawUnit(c1[0]*30, c1[1]*30, oBoard2[c1[0]][c1[1]], tft);
        drawUnit(c1[0]*30, c1[1]*30, board2[c1[0]][c1[1]], tft2);
      }
      else if(shoot(board2, oBoard2, c1) == 0){

        drawUnit(c1[0]*30, c1[1]*30, oBoard2[c1[0]][c1[1]], tft);
        drawUnit(c1[0]*30, c1[1]*30, board2[c1[0]][c1[1]], tft2);
        
        i++;        
        delay(2000);
        drawBoard(board, tft);
        drawBoard(oBoard, tft2); 
      }
      else{
        i = i;
      }
    }
  }
  if(i == 1){
    t(m(j1x), m(j1y), board, c1, tft);
    t(m(j2x), m(j2y), oBoard, c2, tft2);

    if (j2m == 0){
      if(shoot(board, oBoard, c2) == 1){
        hitsTwo++;
        drawUnit(c2[0]*30, c2[1]*30, board[c2[0]][c2[1]], tft);
        drawUnit(c2[0]*30, c2[1]*30, oBoard[c2[0]][c2[1]], tft2);
      }
      else if(shoot(board, oBoard, c2) == 0){
        i--;
        drawUnit(c2[0]*30, c2[1]*30, board[c2[0]][c2[1]], tft);
        drawUnit(c2[0]*30, c2[1]*30, oBoard[c2[0]][c2[1]], tft2);
        
        delay(2000);
        drawBoard(oBoard2, tft);
        drawBoard(board2, tft2);
      }
      else{
        i = i;
      }
    }
  }

  if (hitsOne == 10){
     drawBoard(board2, tft);
     drawBoard(board, tft2);
     tft.setTextSize(3); tft2.setTextSize(3); tft.setTextColor(ILI9488_RED); tft2.setTextColor(ILI9488_RED);
     tft.println("PLAYER 1 WINS");
     tft2.println("PLAYER 1 WINS");
     delay(1000000);
  }
  else if (hitsTwo == 10){
     drawBoard(board2, tft);
     drawBoard(board, tft2);
     tft.setTextSize(3); tft2.setTextSize(3); tft.setTextColor(ILI9488_RED); tft2.setTextColor(ILI9488_RED);
     tft.println("PLAYER 2 WINS");
     tft2.println("PLAYER 2 WINS");
     delay(1000000);
  }
  delay(100); //"master delay"
}
