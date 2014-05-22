
#include "dstarlite.h"
#include <stdlib.h>




static int k = 0;
static float C1 = 1;
static XYPos goal = {5,5};
static XYPos start = {3,3};

static void initializeMazeNode(MazeNode *mn);
static int minimum(int f1, int f2);


int calculateRHS(MazeNode *mn,Graph *gr){
	
	int tmp;
	tmp = INT_MAX;
	if (!mn || !gr) return INT_MAX;
	XYPos *xy = &(mn->position);
	int x = xy->x;
	int y = xy->y;
	
	if (!((int) mn->south)){
		tmp = minimum(gr->table[x][y+1].g + 1,tmp);
	}
	if (!((int) mn->west)){
		tmp = minimum(gr->table[x-1][y].g + 1,tmp);
	}
	if (!((int) mn->east)){
		tmp = minimum(gr->table[x+1][y].g + 1,tmp);
	}
	if (!((int) mn->north)){
		tmp = minimum(gr->table[x][y-1].g + 1,tmp);
	}
	return tmp;
}


int heuristic(XYPos *p1, XYPos *p2)
{
	uint32_t dx = p1->x - p2->x;
	uint32_t dy = p1->y - p2->y;
	return (abs(dx) + abs(dy))*C1;
}


int compareCosts(void *p1, void *p2)
{
	return (((MazeNode*) p1)->costToGoal > ((MazeNode*) p2)->costToGoal);
}


Graph* constructGraph(uint32_t height, uint32_t width)
{
	int i;
	int j;
	MazeNode *b;
	XYPos xy;
	Graph *new = calloc(1,sizeof(Graph));
	new->table = calloc(height,sizeof(int*));

	if(new->table == NULL){
		fprintf(stderr, "out of memory\n");
		return NULL;
	}
	
	for(i = 0; i < height; i++){
	
		new->table[i] = calloc(width, sizeof(MazeNode));
		MALLOC_CHECK(stderr, new->table[i]); 
	}
	
	for (i = 0; i< height; i++){
		for(j = 0; j < width; j++){
			
			
			b = &(new->table[i][j]);
			xy.x = j;
			xy.y = i;
			b->position = xy;
			initializeMazeNode(b);
			if (i == 0){
				b->north = WALL;
			}
			else if (i==height-1){
				b->south = WALL;
			}
			
			if (j == 0){
				b->west = WALL;
			}
			else if (j == width-1){
				b->east = WALL;
			}
		}
	}
	return new;
}

NodeKey* calculateKey(MazeNode *s, Graph *gr)
{
	NodeKey *nk = &(s->key);
	nk->val1 = minimum(heuristic(&(s->position), &goal),calculateRHS(s,gr)) + heuristic(&(s->position),&start) + k;
	nk->val2 = minimum(heuristic(&(s->position), &goal),calculateRHS(s,gr));
	return nk;
}

static int minimum(int f1, int f2){
	if (f1 <= f2) return f1;
	else return f2;
}

static void initializeMazeNode(MazeNode *mn){
	if (!mn) return;
	mn->g = INT_MAX;
	mn->rhs = INT_MAX;
	return;
}

MazeNode* getGoalNode(XYPos *xy,Graph *grid){
	if (!xy || !grid) return NULL;
	MazeNode *new = &(grid->table[xy->x][xy->y]);
	return new;
}



