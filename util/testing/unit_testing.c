#include <time.h>
#include "../src/maze.h"
#include "../../src/maze.c"


static int TEST1(); //Parsing Log file
static int TEST2(); //Freeing of Maze



int main(int argc, char *argv[]){
	
	
	 
	if(TEST1()){fprintf(stderr, "ERROR DURING TEST 1"); return 1;}
	if(TEST2()){fprintf(stderr, "ERROR DURING TEST 2"); return 2;}
	
	
	return 0;
}

static int TEST1(){
	int a = 1;
	MazeCell ***maze;
	maze = parselog(25,25);
	if(maze) a = 0;
	return a;
}

static int TEST2(){
	MazeCell ***test_array;
	test_array = parselog(25,25);
	if (!test_array) return 1;
	freeMaze(test_array,25,25); //Freeing function;
	
	return 0;
}



