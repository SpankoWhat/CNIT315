/* Lab 2 Graded - CIT315
 * Name: Waleed Nasr
 * Email: wnasr@purdue.edu
 * Lab time: Online
 *
 * General Documentation: replicate the movie Inception to solve a maze.
 */
#include <stdio.h>
#include <stdbool.h>

//Width of maze
#define _WIDTH 6

//Height of maze
#define _HEIGHT 6

//maze array
#ifndef _MAZE
//83 = S | 35 = # | 71 = G | 43 = + | 46 = .
int _MAZE[_WIDTH][_HEIGHT] = {
        {83,35,35,35,35,35},
        {46,46,46,46,46,35},
        {35,46,35,35,35,35},
        {35,46,35,35,35,35},
        {46,46,46,35,46,71},
        {35,35,46,46,46,35}
};
#endif

//creating a struct to store locations
struct Location{
    int x;
    int y;
};


//function initializations
void solveMaze();
bool looptyLoop();
void printMaze();
void inputFromUser();
struct Location go(int loc, struct Location position);

int main() {
    bool flag = true;
    int input;

    //basic menu...
    while(flag){
        printf("Start program; 1 for yes, 2 for no:  ");
        if(scanf("%i", &input) != 0){
            if(input == 2){
                printf("K, bye");
                return 0;
            }
            flag = false;
        }
    }
    printf("------------------Current Maze to solve:------------------\n");
    printMaze(*_MAZE);
    solveMaze();
    return 0;
}

void solveMaze(){
    int x, y;
    int solvedMaze[_WIDTH][_HEIGHT];
    struct Location startingPos;

    //copying from the original maze
    for(y = 0; y < _HEIGHT; y++){
        for(x = 0 ; x < _WIDTH; x++){
            solvedMaze[y][x] = _MAZE[y][x];
        }
    }

    //looking for S to begin maze and storing output in a Location struct
    for(y = 0; y < _HEIGHT; y++){
        for(x = 0 ; x < _WIDTH; x++){
            if(solvedMaze[y][x] == 83){
                startingPos.x = x;
                startingPos.y = y;
            }
        }
    }

    printf("\nStarting position is: (%i,%i)...\n", startingPos.y, startingPos.x);

    //The movie Inception begins
    if(looptyLoop(&solvedMaze, startingPos)){
        solvedMaze[startingPos.y][startingPos.x] = 83; //Cleaning output
        printf("\nSolution is:\n");
        printMaze(*solvedMaze);
    }
    else{
        printf("No Solution");
    }
}

//the recursive function
bool looptyLoop(int maze[_WIDTH][_HEIGHT], struct Location position){
    //memory efficient? im not sure but looks cleaner doing it this way
    int north = maze[position.y-1][position.x];
    int east = maze[position.y][position.x+1];
    int south = maze[position.y+1][position.x];
    int west = maze[position.y][position.x-1];

    //checks if current position is G(or 71)
    if (maze[position.y][position.x] == 71){
        return true;
    }

    //leaves breadcrumb and prints 43 = +
    maze[position.y][position.x] = 43;
    printMaze(maze);

    //if statements that check direction and check output of function
    if ((east == 46 || east == 71) && (position.x+1) < _WIDTH){
        if(looptyLoop(maze, go(1, position))){
            return true;
        }
    }

    if ((west == 46|| west == 71) && (position.x+1) >= 0){
        if(looptyLoop(maze, go(3, position))) {
            return true;
        }
    }

    if ((south == 46|| south == 71) && (position.x+1) < _HEIGHT){
        if(looptyLoop(maze, go(2, position))) {
            return true;
        }
    }

    if ((north == 46|| north == 71) && (position.x+1) >= 0){
        if(looptyLoop(maze, go(0, position))) {
            return true;
        }
    }
    //setting path back to . = 46
    maze[position.y][position.x] = 46;
    return false;
}

//simple function that returns a direction of a position
struct Location go(int loc, struct Location position){
    struct Location nextLocation;

    if (loc == 0){
        nextLocation.y = position.y-1;
        nextLocation.x = position.x;
    }
    if (loc == 1){
        nextLocation.y = position.y;
        nextLocation.x = position.x+1;
    }
    if (loc == 2){
        nextLocation.y = position.y+1;
        nextLocation.x = position.x;
    }
    if (loc == 3){
        nextLocation.y = position.y;
        nextLocation.x = position.x-1;
    }

    return nextLocation;
}

//prints maze
void printMaze(int maze[_WIDTH][_HEIGHT]){
    int x,y;

    for(y = 0; y < _HEIGHT; y++){
        for(x = 0 ; x < _WIDTH; x++){
            printf("%c ", maze[y][x]);
            if(x == _WIDTH-1){
                printf("\n");
            }
        }
    }

    printf("______\n");
}
