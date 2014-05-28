#include <time.h>
#include "../../src/maze.h"
#include "../../src/maze.c"


static int TEST1(); //Parsing Log file
static int TEST2(); //Freeing of Maze
static int TEST3(); 
static int TEST4(); 
static int TEST5(); 
static int TEST6(); 


int main(int argc, char *argv[]){
	
	
	 
	if(TEST1()){fprintf(stderr, "ERROR DURING TEST 1"); return 1;}
	if(TEST2()){fprintf(stderr, "ERROR DURING TEST 2"); return 2;}
	if(TEST3()){fprintf(stderr, "ERROR DURING TEST 3"); return 3;}
	if(TEST4()){fprintf(stderr, "ERROR DURING TEST 4"); return 4;}
	if(TEST5()){fprintf(stderr, "ERROR DURING TEST 5"); return 5;}
	
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
	int b = 1;
	MazeCell ***test_array;
	test_array = parselog(25,25);
	if (!test_array) return 1;
	freeMaze(test_array,25,25); //Freeing function;
	return b;
}

static int TEST3(){
	int c = 1;
	
	
	
	
	
	return c;
	

}

static int TEST4(){
	return 0;
}

static int TEST5(){
	return 0;
}


