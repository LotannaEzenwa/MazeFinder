#ifndef DSTARL_H
#define DSTARL_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../util/src/amazing.h"
#include "../util/src/queue.h"


typedef enum {WALL, OPEN, BLOCK} status;


typedef struct MazeNode{
	
	int key;
	float costToGoal;
	XYPos position;
	status north;
	status south;
	status east;
	status west;
		
}MazeNode;

typedef struct PathNode{

		XYPos position;
		struct PathNode *next;
		struct PathNode *prev;
} PathNode;

typedef struct OpenQueue{
	PathNode *head;
	PathNode *tail;
} OpenQueue;



typedef struct Path{
	PathNode *head;
	PathNode *tail;
}Path;

int MH(MazeNode n1, MazeNode n2);





#endif
