# SnakeGameArduino
Components used:  
* Arduino UNO
* 8x8 LED Dot Matrix Display
* 16x2 LCD
* Joystick
* Connecting wires (mother-mother, father-mother)
* 2 X Bread Board
* Resistance 220 Ohm


<img src="https://user-images.githubusercontent.com/32790344/49936656-35e18600-fedd-11e8-963b-c094e1c5f5a1.jpg" width="400" height="500">
How to play:

The game begins by pressing the button on the bread board. The goal is to reach a score as high as possile. In order to do that, you have to eat(touch) the food (the dots that are lightened along your way on the matrix). You can choose to go up, down, left or right, using the joystick. If you don't change the snake's direction, it will continue to move in the last direction you've chosen. Don't forget that with every bite taken, the snake's speed increases, making your moves sometimes tricky. The game ends when the snake bites its body. Don't worry, you are entitled for a revenge, so press again the button on the bread board to replay. :)

Scoring & Difficulty:
* the score increseas by 10 when the snakes bites the food
* the difficulty increases at every bite; the delay decreases between lightening and switching off the snake's body on the matrix

Implementation:
<p> The snake's body as well as the food and the snake's head, are all using 2 vectors, one for X coordinate (length) and one for Y coodinate(height). (the hole algorithm uses the visualisation of a xOy system). FoodX and foodY are random generated values, the head suffers modifications either on X coordinate (left/right move), either on Y coordinate (up/down move) and also can pass at the beginning/end of the current line when the snake has reached the edge of the matrix. Then, the body suffers modifications from back to the  head.</p>

At every move (if gameOn == true) , there will be calls for the following functions: 
1. checkFood()-to see if the snake has reached the food
2. (yes)-foodFound()- to generate randomly new food -> difficuly increases -> growSnake()
3. snakeDies() (snake bites itself) -> gameLost() & gameReset()-> gameOn = false -> need the button to be pressed to REPLAY
