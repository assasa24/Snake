#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>                                                                                //ASAF KOENIGSBERG 318654118
                                                                                                  //this program is a game of snake.enjoy!!!
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <stdbool.h>
#define UP 72  // Up Arrow 
#define DOWN 80 // Down Arrow 
#define LEFT 75  // Left Arrow 
#define RIGHT 77  // Right Arrow 
#define ESC 27  // Escape Key
#define ROWS 25//number of rows
#define COLS 75//number of cols
#define SLEEP 200//amount of time to sleep
#define SNAKE_SIZE 10//snake size
#define WIN_LENGTH 10//how many times you have to eat food to win
#define STAM 5//indefinite value at the start of the game 
#define HASTAGLENGTH 55//number of hashtags in the menu message
typedef struct gameBoard {//represents a game board
	int x;
	int y;
	char c;
}Board;
typedef struct snakeCell {//represents a snake cell
	int x;
	int y;
	char c;
}SingleCell;
void buildBoard(Board board[][COLS], int rows, int cols);//builds the entire board
void printBoard(Board board[][COLS], int rows, int cols);//prints the entire board
void placeFood(Board board[][COLS], int rows, int cols);//places food in random places on the screen
void printSnake(SingleCell* snake, int snakeSize);//prints the snake
void initSnake(SingleCell* snake, int snakeSize);//creates the snake
void moveSnake(SingleCell* snake, int snakeSize, char c);//moves the snake around the screen
bool eatFood(Board board[][COLS], SingleCell* snake, int snakeSize);//checks if the snake ate a food.yes-true,no-false
SingleCell* changeSize(SingleCell* snake, int snakeSize);//increases the snake size by 1
void getPress(char* c);//checks what key was pressed in the keyboard
void gotoxy(int x, int y);
int getkey();
void playGame(Board b[][COLS]);//function that plays the game
void printMenu();//prints the menu
void checkAllocation(SingleCell* snake);//checks if snake creation succeeded(malloc,realoc),if not-exits the program
bool didLose(SingleCell* snake, Board board[][COLS], int snakeSize);//checks if the player lost
void printMessage(bool lost, bool won);//prints a message if the player won or lost.
bool gameWon(int* counter);//checks if pklayer won the game
bool snakeAteItself(SingleCell* snake, int size);//checks if the snake ate itself
void startAgain(SingleCell** snake, int* snakeSize, char* c, int* countTimesEaten, char* lastValidC, Board board[][COLS]);//after player loses/wins if he wants to start again,restarts all
void main()
{
	int rows = ROWS, cols = COLS;
	Board board[ROWS][COLS];
	int choice;
	printMenu();
	scanf("%d", &choice);
	while (choice != 1 && choice != 0)//checks if the choice is valid
	{
		if (choice != 2 && choice != 3)
			printf("Invalid choice\n");

		printMenu();
		scanf("%d", &choice);

	}
	system("cls");

	switch (choice)
	{
	case 1://time to start the game
		buildBoard(board, rows, cols);
		printBoard(board, rows, cols);
		placeFood(board, rows, cols);
		playGame(board);
		break;
	case 0://time to say goodbye
		printf("                                  BYE BYE\n\n");
		break;
	}
}
void playGame(Board board[][COLS])
{
	int snakeSize = SNAKE_SIZE;
	SingleCell* snake = (SingleCell*)malloc(snakeSize * sizeof(SingleCell));
	checkAllocation(snake);
	initSnake(snake, snakeSize);//creates a snake with 10 cells
	char c = STAM;//indefinite char
	char lastValidC = STAM;//indefinite char
	int countTimesEaten = 0;//counts how many times a food was eaten
	int choice;//player chooses what to do after game over
	bool lost = false, exitChoice = false, won = false;
	while (!exitChoice)//while the player hasnt chosen to exit
	{
		while (!won && !lost && c != ESC)//while the game isnt over
		{
			printSnake(snake, snakeSize);//prints the snake on the screen
			Sleep(SLEEP);//sleeps
			getPress(&c);//c gets the key pressed

			if (c == LEFT || c == RIGHT || c == UP || c == DOWN)//if key pressed is valid
			{
				lastValidC = c;//saves the last c value
				moveSnake(snake, snakeSize, c);//moves the snake according to c value
			}
			else//key pressed is not valid
			{
				if (lastValidC != STAM)//if last value pressed is a valid key
					moveSnake(snake, snakeSize, lastValidC);//moves according to the last valid value
				else//is not a valid key,then we continue
					continue;
			}

			if (c == ESC)//player pressed esc in the middle of the game
			{
				system("cls");
				lost = true;
				printMessage(won, lost);
				break;
			}

			if (eatFood(board, snake, snakeSize))
			{
				snake = changeSize(snake, &snakeSize);//eats the food and changes snake size if snake met food
				countTimesEaten++;
				continue;
			}

			if (didLose(snake, board, snakeSize))//player lost
			{
				system("cls");
				lost = true;
				printMessage(won, lost);
				free(snake);
				break;
			}
			if (gameWon(&countTimesEaten))//player won
			{
				won = true;
				system("cls");
				printMessage(won, lost);
				free(snake);
				break;
			}
		}
		if (!lost && !won)//player pressed esc before the game even begun
			system("cls");
		printMenu();//game is over,wether lost won or pressed esc
		scanf("%d", &choice);
		switch (choice)
		{
		case 0://time to say goodbye
			system("cls");
			printf("                                  BYE BYE\n\n");
			exitChoice = true;
			break;
		case 1://start again
			system("cls");
			exitChoice = lost = won = false;
			startAgain(&snake, &snakeSize, &c, &countTimesEaten, &lastValidC, board);
			break;
		}
	}
}
void startAgain(SingleCell** snake, int* snakeSize, char* c, int* countTimesEaten, char* lastVaildC, Board board[][COLS])
{
	//gives all the important game values the initial values in the start
	*c = STAM;
	*lastVaildC = STAM;
	*countTimesEaten = 0;
	int size = *snakeSize;
	
	buildBoard(board, ROWS, COLS);
	printBoard(board, ROWS, COLS);
	placeFood(board, ROWS, COLS);

	*snakeSize = SNAKE_SIZE;
	*snake = (SingleCell*)malloc(size * sizeof(SingleCell));
	initSnake(*snake, size);
	checkAllocation(*snake);
}
bool gameWon(int* counter)
{
	if (*counter == WIN_LENGTH)//snake ate food 10 times
		return true;
	return false;
}
void printMessage(bool won, bool lost)
{
	//prints won or lost message
	if (lost == true)
	{
		printf("##################################################\n");
		printf("#                     GAME OVER\n");
		printf("##################################################\n");
	}
	else if (won == true)
	{
		printf("##################################################\n");
		printf("#                     you win\n");
		printf("##################################################\n");
	}
}
bool snakeAteItself(SingleCell* snake, int size)
{
	int i = 0;
	for (i = 0;i < size - 1;i++)//checks each cell if equals to the head(snake[size-1])
	{
		if (snake[size - 1].x == snake[i].x && snake[size - 1].y == snake[i].y)
			return true;
	}
	return false;

}
bool didLose(SingleCell* snake, Board board[][COLS], int snakeSize)
{
	if (snakeAteItself(snake, snakeSize))//snake eats itself,game over
		return true;
	int row = snake[snakeSize - 1].x;
	int column = snake[snakeSize - 1].y;
	if (board[row][column].c == '#')//snake hit wall,game over
		return true;
	return false;
}
SingleCell* changeSize(SingleCell* snake, int* snakeSize)
{
	//we create a new snake with snakeSize+1 and free to the last snake...realloc
	int i, newSize = (*snakeSize) + 1;
	SingleCell* newSnake = (SingleCell*)malloc(newSize * sizeof(SingleCell));
	checkAllocation(newSnake);
	for (i = 0;i < *snakeSize;i++)
	{
		newSnake[i] = snake[i];
	}
	(*snakeSize)++;
	newSnake[(*snakeSize - 1)] = newSnake[(*snakeSize - 2)];//the new head gets the previous head
	free(snake);
	return newSnake;
}
bool eatFood(Board board[][COLS], SingleCell* snake, int snakeSize)
{
	//checks if ate food
	int row = snake[snakeSize - 1].x;
	int column = snake[snakeSize - 1].y;
	char c = board[row][column].c;
	if (c == '$')
	{
		board[row][column].c = ' ';//cells is ' ' now and not '$',need to delete the '$'
		gotoxy(row, column);
		printf(" ");
		placeFood(board, ROWS, COLS);//places a new food
		return true;
	}
	return false;
}
void getPress(char* c)
{
	if (_kbhit())
		*c = getkey();
}
void moveSnake(SingleCell* snake, int snakeSize, char c)
{
	int i;
	gotoxy(snake[0].x, snake[0].y);
	printf(" ");
	for (i = 0;i < snakeSize - 1;i++)
	{
		snake[i] = snake[i + 1];//each cell gets the value infront
	}
	switch (c)
	{
	case RIGHT:
		snake[snakeSize - 1].y++;//goes right
		break;
	case LEFT:
		snake[snakeSize - 1].y--;//goes left
		break;
	case UP:
		snake[snakeSize - 1].x--;//goes up
		break;
	case DOWN:
		snake[snakeSize - 1].x++;//goes down
		break;
	default:
		break;
	}
}
void printSnake(SingleCell* snake, int snakeSize)
{
	int i;
	for (i = 0;i < snakeSize;i++)
	{
		gotoxy(snake[i].x, snake[i].y);//prints each cell according to the Coordinates
		printf("%c", snake[i].c);
	}

}
void initSnake(SingleCell* snake, int snakeSize)
{
	//creates a snake
	int i, row = 12, col = 32;//puts the snake in an indefinite place
	for (i = 0;i < snakeSize;i++)
	{
		snake[i].x = row;
		snake[i].c = '@';
		snake[i].y = col;
		col++;
	}
}
void gotoxy(int x, int y) {
	printf("\x1b[%d;%df", x + 1, y + 1);
}
int getkey()
{
	char KeyStroke = _getch();
	if (KeyStroke == 0 || KeyStroke == -32)
		KeyStroke = _getch();
	return (KeyStroke);
}
void printMenu()
{
	int i, j;
	for (i = 0;i < 7;i++)
	{
		if (i == 1)
			printf("\n* Pick Your Option, to exit press 0\n");
		if (i == 2)
			printf(" 0 -Exit\n");
		if (i == 3)
			printf(" 1 -Start Game\n");
		for (j = 0;j < HASTAGLENGTH;j++)
		{
			if (i == 0 || i == 6)
				printf("*");
			if (j == 0)
				printf("*");
		}
	}
}
void placeFood(Board board[][COLS], int rows, int cols)
{
	//places a food in a random place
	int randomX, randomY;
	srand(time(NULL));
	randomX = rand() % rows;
	randomY = rand() % cols;
	while (randomX == 0 || randomX == rows - 1 || randomY == 0 || randomY == cols - 1)//while the food hasnt reached the boundries
	{
		randomX = rand() % rows;
		randomY = rand() % cols;
	}
	board[randomX][randomY].c = '$';
	gotoxy(randomX, randomY);
	printf("$");
}
void buildBoard(Board board[][COLS], int rows, int cols)
{
	//creates a whole board
	int i, j;
	for (i = 0;i < rows;i++)
	{
		for (j = 0;j < cols;j++)
		{
			board[i][j].x = i;
			board[i][j].y = j;
			if (i == 0 || i == rows - 1)
				board[i][j].c = '#';
			else
				board[i][j].c = ' ';
			if (j == 0 || j == cols - 1)
				board[i][j].c = '#';
		}
	}
}
void printBoard(Board board[][COLS], int rows, int cols)
{
	//prints the entire board
	int i, j;
	for (i = 0;i < rows;i++)
	{
		for (j = 0;j < cols;j++)
		{
			printf("%c", board[i][j].c);
		}
		printf("\n");
	}
}

void checkAllocation(SingleCell* snake)
{
	//checks if malloc succeded
	if (snake == NULL)
	{
		printf("Memory allocation failed!");
		exit(1);
	}
}