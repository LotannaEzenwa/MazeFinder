/* ========================================================================== */
/* File: dstarlite.h
 *
 * Project name: CS50 Amazing Project
 * Component name: dstarlite header
 *
 * Primary Author: Oseleta
 * Date Created: Mon May 20 2014
 *
 * This file contains the common defines for the dstarlite algorithm.
 *
 */
/* ========================================================================== */
#ifndef DSTARL_H
#define DSTARL_H

// ---------------- Prerequisites e.g., Requires "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
// ---------------- Local includes  e.g., "file.h"
#include "../util/src/amazing.h"
#include "../util/src/queue.h"

// ---------------- Structures/Types
typedef enum {PATH, WALL} status;
typedef enum {TRAVELED, UNVISITED} pathstatus;
typedef enum {NEW, OPEN, CLOSED,RAISE, LOWER} mark;

typedef struct NodeKey
{
	float val1;
	float val2;
} NodeKey;



typedef struct MazeNode
{

	NodeKey key;
	uint32_t g;
	uint32_t rhs;
	int costToGoal;
	XYPos position;
	status north;
	status south;
	status east;
	status west;
	pathstatus ps;
	mark mk;
	int maze_boolean;
	
} MazeNode;


typedef struct Graph
{
	MazeNode **table;
} Graph;

typedef struct PathNode
{
	XYPos position;
	struct PathNode *next;
	struct PathNode *prev;
} PathNode;

typedef struct OpenQueue
{
	PathNode *head;
	PathNode *tail;
} OpenQueue;



typedef struct Path
{
	PathNode *head;
	PathNode *tail;
} Path;

// ---------------- Private prototypes
int MH(MazeNode n1, MazeNode n2);
int calculateRHS(MazeNode *mn, Graph *gr);
int heuristic(XYPos *p1, XYPos *p2);
int compareCosts(void *p1,void *p2);
int compareKey(void *p1, void *p2);
int testin(Queue *queue, MazeNode *mn1);

Graph* constructGraph(uint32_t height, uint32_t width);
void dStarInit(MazeNode *s_start,MazeNode *s_goal,Graph *gr, Queue *heap);
MazeNode* getGoalNode(XYPos *xy,Graph *grid);
void dstarmain(mz_data *init_data);
NodeKey calculateKey(MazeNode *s, Graph *gr);
void computeSP(MazeNode *s_start, MazeNode *s_goal,Queue *heap, Graph *gr);
void updateVertex(MazeNode *u, MazeNode *start, Queue *heap, Graph *gr);




#endif
