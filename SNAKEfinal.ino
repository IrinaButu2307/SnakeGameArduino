#include "LedControl.h"
#include <LiquidCrystal.h>

// MATRIX DRIVER
#define DIN 12
#define CLK 11
#define CS 10 

LedControl matrix = LedControl(DIN, CLK, CS, 1);

// JOYSTICK
#define joyX A0
#define joyY A1

const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7; //LCD 
const int buttonPin = 13;                            //button to start/replay
const int lcdPin = 9;                               // pin for lcd luminousity


LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//used for mapping joystick's values
int newRowMap, newColMap;
int rowValue, colValue;

int snakeHeadCol = 0;   // head on [0][0]
int snakeHeadRow = 0;
int snakeBodyCol[64] = {0,1,2}; // start position will be in left corner up
int snakeBodyRow[64] = {0,0,0}; // start state of the body [0][0] , [0][1], [0][2]
int snakeLength = 3;
int snakeDirection = 0;

int foodRow = 0;
int foodCol = 0;
bool foodFound = false;

int gameScore = 0;
int gameSpeed = 250;
bool gameOn = false;
int buttonStatus = 0;

void setFood()                          /// set the food randomly, exept on the snake's body
{
  int foodAccepted = 0;
  int positionAccepted = 0;
  int i;
  while(foodAccepted == 0)
  {
    foodRow = random(8);
    foodCol = random(8);
    positionAccepted = 1;
    for(i = 0 ; i < snakeLength ; i++)
    {
      if(snakeBodyRow[i] == foodRow && snakeBodyCol[i] == foodCol)
        positionAccepted = 0; 
    }
    if(positionAccepted)
      foodAccepted = 1;
  }
  
}

void displaySnakeOn()                                       /// show snake on matrix
{
  for(int i = 0 ; i < snakeLength ; i++)
  {
    matrix.setLed(0, snakeBodyCol[i], snakeBodyRow[i], true);           //Anod Matrix
  }
}
void displaySnakeOff()                                          // erase the snake form matrix
{                                   
  for(int i = 0 ; i < snakeLength ; i++)
  {
    matrix.setLed(0, snakeBodyCol[i], snakeBodyRow[i], false);
  }
}


void snakeMovement()
{
  
  int ok = 0;                                             // mandatory to execute only one if per loop
  ///          1 = right 
  ///          2 = left
  ///          3 = up
  ///          4 = down
  rowValue = analogRead(joyX);
  colValue = analogRead(joyY);
                                                         // mapping the joystick values
  newRowMap = map(rowValue, 0, 1023, 0,9); // 0-3 =  left, 3-6 = no move detected, 6-9 = right;
  newColMap = map(colValue, 0, 1023, 0,9); // 0-3 = down, 3-6 = no move detected, 6-9= up;

  if(newRowMap > 6 && ok == 0)                               //going to the right
    {
      ok = 1;
      snakeDirection = 1;
      snakeHeadCol ++;
      if(snakeHeadCol > 7)
         snakeHeadCol = 0;                                 // i can overpass the matrix edge and go to the beginning of the current line
      for(int i = snakeLength - 1 ; i>0 ; i--)
        {
            snakeBodyRow[i] = snakeBodyRow[i - 1];           //the rest of the body will get the value of the segment ahead
            snakeBodyCol[i] = snakeBodyCol[i - 1];
        }
      snakeBodyRow[0] = snakeHeadRow;                        // the head takes the new value read from joystick
      snakeBodyCol[0] = snakeHeadCol;
    }

  if(newRowMap < 3 && ok == 0)                                //going to the left
   {
     ok = 1;
     snakeDirection = 2;
     snakeHeadCol--;
     if(snakeHeadCol < 0)                                  // i can overpass the matrix edge and get to the end of the current column 
        snakeHeadCol = 7;
     for(int i = snakeLength - 1 ; i > 0 ; i--)
       {
          snakeBodyRow[i] = snakeBodyRow[i - 1];
          snakeBodyCol[i] = snakeBodyCol[i - 1];
       }
     snakeBodyRow[0] = snakeHeadRow;
     snakeBodyCol[0] = snakeHeadCol;
   }
  if(newColMap < 3 && ok == 0)                                //going up
    {
      ok = 1;
      snakeDirection = 3;
      snakeHeadRow--;
      if(snakeHeadRow < 0)
         snakeHeadRow = 7;
      for(int i = snakeLength - 1; i > 0 ; i--)
        {
           snakeBodyRow[i] = snakeBodyRow[i - 1];
           snakeBodyCol[i] = snakeBodyCol[i - 1];
        }
      snakeBodyRow[0] = snakeHeadRow;
      snakeBodyCol[0] = snakeHeadCol;
    } 
  if(newColMap > 6 && ok == 0)                              //going down
    {
      ok = 1;
      snakeDirection = 4;
      snakeHeadRow++;
      if(snakeHeadRow > 7)
         snakeHeadRow = 0;
      for(int i = snakeLength - 1 ; i > 0 ; i--)
        {
           snakeBodyRow[i] = snakeBodyRow[i - 1];
           snakeBodyCol[i] = snakeBodyCol[i - 1];
        }
      snakeBodyRow[0] = snakeHeadRow;
      snakeBodyCol[0] = snakeHeadCol;
     }

  if(newRowMap  > 3 && newRowMap < 6 && newColMap > 3 && newColMap < 6)             // if there is no move detected the snake will continue to go into the last direction fixed
    {
       if(snakeDirection == 1)
       {
             snakeHeadCol ++;
             if(snakeHeadCol > 7)
                snakeHeadCol = 0;
             for(int i = snakeLength - 1 ; i > 0 ; i--)
               {
                 snakeBodyRow[i] = snakeBodyRow[i - 1];
                 snakeBodyCol[i] = snakeBodyCol[i - 1];
               }
             snakeBodyRow[0] = snakeHeadRow;
             snakeBodyCol[0] = snakeHeadCol;
             ok = 1;
             snakeDirection = 1;
       }
       else if(snakeDirection == 2)
        {
            
               snakeHeadCol--;
               if(snakeHeadCol < 0)
                  snakeHeadCol = 7;
               for(int i = snakeLength - 1 ; i > 0 ; i--)
                 {
                    snakeBodyRow[i] = snakeBodyRow[i - 1];
                    snakeBodyCol[i] = snakeBodyCol[i - 1];
                 }
               snakeBodyRow[0] = snakeHeadRow;
               snakeBodyCol[0] = snakeHeadCol;
               ok = 1;
               snakeDirection = 2;
        }
       else if(snakeDirection == 3)
       {
           
                snakeHeadRow--;
                if(snakeHeadRow < 0)
                    snakeHeadRow = 7;
                for(int i = snakeLength - 1; i > 0 ; i--)
                  {
                      snakeBodyRow[i] = snakeBodyRow[i - 1];
                      snakeBodyCol[i] = snakeBodyCol[i - 1];
                  }
               snakeBodyRow[0] = snakeHeadRow;
               snakeBodyCol[0] = snakeHeadCol;
               ok = 1;
               snakeDirection = 3;
       }
      else if(snakeDirection == 4)
      {
              snakeHeadRow++;
              if(snakeHeadRow > 7)
                 snakeHeadRow = 0;
              for(int i = snakeLength - 1; i > 0; i--)
                {
                    snakeBodyRow[i] = snakeBodyRow[i - 1];
                    snakeBodyCol[i] = snakeBodyCol[i - 1];
                }
              snakeBodyRow[0] = snakeHeadRow;
              snakeBodyCol[0] = snakeHeadCol;
              ok = 1;
              snakeDirection = 4;
      }
    }
 
  displaySnakeOn();
  delay(gameSpeed);            // decreasing delay by 10 at each bite of the food == increasing snake's speed
  displaySnakeOff();           //otherwise, all the moves will be displayed on the matrix
  
}

void displayScore()                                 // show the score on screen
{
  lcd.clear();
  lcd.begin(16, 2);
  lcd.setCursor(2, 0);
  lcd.print("SCORE");
  lcd.setCursor(11, 0);
  lcd.print(gameScore);
}


void checkFood()                                    //check if the snake has found the food
{
  foodFound = false;
  if(snakeBodyRow[0] == foodRow && snakeBodyCol[0] == foodCol)
  {
    foodFound = true;
    matrix.setLed(0, foodCol, foodRow, false);              //food is eaten
    setFood();                                      // generate new food
    gameScore += 10;                                // increase the score by 10
    gameSpeed -= 10;                                // decrease delay == gamespeed
    displayScore();
  }
  matrix.setLed(0, foodCol, foodRow, true);                 //show the food on matrx
}

bool growSnake()                                    // increase snake's dimensions -- food position passes at the end of the snake
{
  snakeLength++;
  snakeBodyRow[snakeLength - 1] = foodRow; 
  snakeBodyCol[snakeLength - 1] = foodCol;
}

void gameReset()                                      // reset - snake goes back to the original length, speed, direction; score resets
{ 
  gameOn = false;
  gameSpeed = 250;
  snakeHeadRow = 0;
  snakeHeadCol = 0;
  // start position will be in left corner up
  // start state of the body [0][0] , [0][1], [0][2]

  /*
for(int i = 0 ; i < snakeLength ; i++)
  {
    snakeBodyCol[i]=0; 
    snakeBodyRow[i]=0;  
  }
*/  
  snakeBodyRow[0] = 0;
  snakeBodyRow[1] = 0;
  snakeBodyRow[2] = 0;
  snakeBodyCol[0] = 0;
  snakeBodyCol[1] = 1;
  snakeBodyCol[2] = 2;
  snakeDirection = 0;
  snakeLength = 3;
  gameSpeed = 250;
  gameScore = 0;  

  displaySnakeOn();                                                       //after returning to the original state, show the snake on matrix
}


void gameLost()                                                           // messages for the player with the score + the message to replay
{
  gameOn = false;
  lcd.clear();
  lcd.begin(16, 2);
  lcd.setCursor(4, 0);
  lcd.print("YOU LOST");
  lcd.setCursor(0, 1);
  lcd.print("Final Score");
  lcd.setCursor(13, 1);
  lcd.print(gameScore);
  delay(3000);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("PRESS TO REPLAY");
  lcd.setCursor(2, 1);
  lcd.print("Go! Go! Go!");
  matrix.clearDisplay(0);
  
}

bool snakeDies()                                    // check if the snake bites itself, head touches the body
{
  int x = snakeBodyRow[0];
  int y = snakeBodyCol[0];
  for(int i = 1; i < snakeLength; i++)
    if(x == snakeBodyRow[i] && y == snakeBodyCol[i])
   {
       gameLost();
       gameReset();
       return true;
   }
  return false;   
}


void setup()
{
  matrix.shutdown(0, false);    //to set the property of lightning
  matrix.setIntensity(0, 8);    
  lcd.begin(16, 2);
  lcd.setCursor(3, 0);
  lcd.print("SNAKE GAME");
  lcd.setCursor(0, 1);
  lcd.print("PRESS THE BUTTON");
  pinMode(buttonPin, INPUT);
  analogWrite(lcdPin, 90);
  setFood();
}
void loop()
{ 
  buttonStatus = digitalRead(buttonPin);
  if(buttonStatus == HIGH)                        // game starts/resets when pressing the button
      gameOn = true;
    
  if(gameOn)
  {
    snakeMovement();
    checkFood();
    if(foodFound)
      growSnake();
    if(snakeDies())
      gameOn = false;
  }
}
