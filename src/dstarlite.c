
#include "dstarlite.h"
#include <stdlib.h>



static float minimum(float f1, float f2);



int heuristic(XYPos *p1, XYPos *p2){
	uint32_t dx = p1->x - p2->x;
	uint32_t dy = p1->y - p2->y;
	return abs(dx) + abs(dy);
}


int compareCosts(void *p1, void *p2){
	return (((MazeNode*) p1)->costToGoal > ((MazeNode*) p2)->costToGoal);
}


Graph* constructGraph(uint32_t height, uint32_t width){
	int i;
	Graph *new = malloc(sizeof(Graph));
	new->table = malloc(height*sizeof(int*));
	if(new->table == NULL){
		fprintf(stderr, "out of memory\n");
		return NULL;
	}
	
	for(i = 0; i < height; i++){
	
		new->table[i] = malloc(width * sizeof(MazeNode));
		if(new->table[i] == NULL){
			fprintf(stderr, "out of memory\n");
			return NULL;
		}
	}
	return new;
}

NodeKey calculateKey(MazeNode *s){
	NodeKey nk;
}

static float minimum(float f1, float f2){
	if (f1 < f2) return f1;
	else if (f2 < f1) return f2;
	return NULL;9--===
}




