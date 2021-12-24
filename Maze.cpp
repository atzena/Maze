// Tries to solve a maze using a recursive solution
// Shows path through the maze if one exists

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <string>
using namespace std;

const int FILENAMESIZE = 255;
const int MAXROWS = 22;
const int MAXCOLS = 81;

// ASCII characters to use for display
// https://theasciicode.com.ar/extended-ascii-code/block-graphic-character-ascii-code-219.html
const char WALL = (char)219;
const char WALLINFILE = '0';
const char START = 'S';
const char DEST = 'D';
const char EMPTY = ' ';
const char PATH = '*';
const char DEAD = 'X';

/*
* getFile will get a filename from the user. It then attmpts
* to open the file, read its contents, and store the maze
* structure into the maze array, based on the contents of the file.
* A WALLINFILE character in the file represents a maze wall, while
* an EMPTY character represents a free pathway. The file should also
* contain a single START character and a single DEST character.
* The function stops reading and exits the program if > MAXCOLS-1
* characters are found on one row, or if > MAXROWS rows are
* found in the file.  If the function is successful, then
* startX and startY will be set to the row and column number of
* where the START character is located in the maze.
*/
void getMaze(char maze[][MAXCOLS], int& startX, int& startY);

/*
* display is used to draw the maze on the screen.
*/
void display(const char maze[][MAXCOLS]);

/*
*TODO: Declare the remaining functions you need
*/
void initMaze(char maze[][MAXCOLS]);

/*
*mazeSolution is the recursion function.
*/
bool mazeSolution(char maze[][MAXCOLS], int row, int col, int& count);


int main()
{
	char maze[MAXROWS][MAXCOLS];
	int startX, startY;
	char input;

	initMaze(maze);
	getMaze(maze, startX, startY);
	display(maze);
	// Count is set to the recursive calls to count how many have been made
	int count = 0;
	cout << "Press ENTER to solve maze";
	cin.get();

	bool solutionMessage;


	solutionMessage = mazeSolution(maze, startX, startY,count);
	//Initializes startX and startY with START
	maze[startX][startY] = START;  

	//This FOR loop allows all the dead values to not appear in the final display output.
	for (int row = 0; row < MAXROWS; row++)
	{
		for (int col = 0; col < MAXCOLS; col++)
		{
			//If the position contains a DEAD value, then it becomes equal to EMPTY
			if (maze[row][col] == DEAD) { 
				maze[row][col] = EMPTY;
			}
		}

	}
	
	
	display(maze);
if (solutionMessage == true)
{
	printf(" %20c Solution found!", EMPTY);
	count++;
	printf(" -- %d recursive calls\n", count);
}
else{
	printf("%20c No solution found!", EMPTY);
	count++;
	printf(" -- %d recursive calls\n", count);
}
printf("Press ENTER to exit\n");
cin.get();
	return 0;
}

//This function is used to output the 2D Array to the screen.
void display(const char maze[][MAXCOLS])
{
	// system("cls") clears the screen; only works on Windows machines
	// You will probably need to comment this out if working on
	// a non-Windows machine
	system("cls");

	//This for loop goes through each position in the 2D array and outputs it to the screen.
	
	for (int row = 0; row < MAXROWS; row++) {
		for (int col = 0; maze[row][col] != '\0' && col < MAXCOLS; col++)
		{
				cout << maze[row][col];

		}
		cout << endl;
	}
}

//getMaze gets characters from a text file and fills the 2D Array with the values.
void getMaze(char maze[][MAXCOLS], int& startX, int& startY)
{
	ifstream inFile;

	// Prompts user to open a file
	cout << "Enter the name of a TXT file containing the maze (don't enter 'txt):" << endl;

	char fileName[FILENAMESIZE];
	
	// Gets the file name from the user.
	cin.getline(fileName, FILENAMESIZE - 4); 
	// Adds ".txt" to the end of the user's keyboard input
	strcat_s(fileName, ".txt");  

	// Opens the file
	inFile.open(fileName);  

	// Checks to see if the file works
	if (!inFile)
	{
		cerr << "ERROR: input file could not be opened.";
		// Exits the program if file fails to open
		exit(1);
	}
	char c = inFile.get();
	bool tooManyChar = false;
	int rowCounter = 0;
	// Loop that cycles through the rows.
	while (!inFile.eof() && rowCounter < MAXROWS && !tooManyChar)
	{
		int colCounter = 0;
		// Loop that cycles through the columns.
		while (!inFile.eof() && colCounter != MAXCOLS && colCounter < MAXCOLS && c != '\n')
		{
			// If the character is a value equivalent to WALLINFILE, it sets it to value WALL.
			if (c == WALLINFILE) 
			{
				maze[rowCounter][colCounter] = WALL;
			}
			// If the character is a value equivalent to START, it sets the position equal to START.
			else if (c == START)
			{
				maze[rowCounter][colCounter] = START;

				// Initializes the X and Y value to the rowCounter and colCounter.
				startX = rowCounter; 
				startY = colCounter; 
			}
			else
			{
				// Receives all other characters from the file.
				maze[rowCounter][colCounter] = c;
			}
			colCounter++;
			c = inFile.get();
		}
		
		// Finds the null character at the end of the row.
		maze[rowCounter][colCounter] = '\0';

		// So that the inner while loop doesn't end when it reaches an end of a line within the file
		if (c != '\n')
		{
			tooManyChar = true;

		}
		else {
			c = inFile.get();
		}
	
		rowCounter++;
		
		// If the end of file is reached, this FOR loop fills the rest of the rows with an end '\0' 

		
		for (rowCounter; inFile.eof() && rowCounter < MAXROWS; rowCounter++)
		{
			maze[rowCounter][0] = '\0';
		}
		
	}
	
	// These IF statements will check for errors
	if (!inFile.eof())
	{
		if (rowCounter <= MAXROWS)
		{
			cerr << "\nERROR: Line " << rowCounter << " in input file contains more than " << MAXCOLS - 1 << " chars\n";
		}
		else
		{
			cerr << "\nERROR: There are more than " << MAXROWS << " lines in the file\n";
		}
		inFile.close();
		exit(1);
	}
	inFile.close();
}

// MazeSolution is the recursive function that solves the maze.
bool mazeSolution(char maze[][MAXCOLS], int row, int col, int& count)
{
	count += 1;
	//Base cases.

	// Checks that array does not go out of bounds.
	if (row < 0 || row >= MAXROWS || col < 0 || col >= MAXCOLS)
	{
		return false;
	}
	//The search position is the destination, so the maze is solved.
	if (maze[row][col] == DEST)
	{
		return true;
	}
	// If the search position is a wall, null character, an X, or infinite loop
	// Will return false and go elsewhere.
	
	if (maze[row][col] == WALL || maze[row][col] == '\0' || maze[row][col] == DEAD || maze[row][col] == PATH)
	{
		return false;
	}

	// Recursive cases.
	// Chooses a path to follow based on non-base case positions.
	maze[row][col] = PATH;
	if (mazeSolution(maze, row, col + 1, count)) {
		return true;
	}

	if (mazeSolution(maze, row + 1, col, count)) {
		return true;
	}

	if (mazeSolution(maze, row - 1, col, count)) {
		return true;
	}

	if (mazeSolution(maze, row, col - 1, count)) {
		return true;
	}

	maze[row][col] = DEAD;
	return false;
}

// This function is meant to initialize the 2D array with a value.
void initMaze(char maze[][MAXCOLS])
{
	int row = 0;
	// This FOR loop fills the entire 2D Array with EMPTY values.
	for (row; row < MAXROWS; row++) {
		for (int col = 0; col <MAXCOLS; col++)
		{
			maze[row][col] = EMPTY;
		}
	}
}