
#include "dstarlite.h"
#include <stdlib.h>




int k = 0;
float C1 = 1;
XYPos goal;

static void initializeMazeNode(MazeNode *mn);
//static float minimum(float f1, float f2);



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
			
			XYPos *xy = calloc(1,sizeof(XYPos));
			MazeNode *b = &(new->table[i][j]);
			xy->x = i;
			xy->y = j;
			b->position = *xy;
			free(xy);
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

NodeKey calculateKey(MazeNode *s){
	NodeKey nk;
	return nk;
}

/*static float minimum(float f1, float f2){
	if (f1 < f2) return f1;
	else if (f2 < f1) return f2;
	return 0.0;
}*/

static void initializeMazeNode(MazeNode *mn){
	if (!mn) return;
	mn->g = UINT_MAX;
	mn->rhs = UINT_MAX;
	
	return;
}



