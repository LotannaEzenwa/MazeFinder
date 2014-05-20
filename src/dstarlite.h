#ifndef DSTARL_H
#define DSTARL_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include 
#include "../util/src/amazing.h"
#include "../util/src/queue.h"


typedef enum {WALL, PATH} status;
typedef enum {TRAVELED, UNVISITED} pathstatus;
typedef enum {NEW, OPEN, CLOSED,RAISE, LOWER} mark;

typedef struct NodeKey{
	float val1 = INFINITY;
	float val2;
}NodeKey;



typedef struct MazeNode{

	NodeKey key;
	float costToGoal;
	XYPos position;
	status north;
	status south;
	status east;
	status west;
	pathstatus ps;
	mark mk;
	

}MazeNode;

typedef struct Graph{

	MazeNode **table;
	
}Graph;

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
int heuristic(XYPos *p1, XYPos *p2);
int compareCosts(void *p1,void *p2);

Graph* constructGraph(uint32_t height, uint32_t width);




#endif
