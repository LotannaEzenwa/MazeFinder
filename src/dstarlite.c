#include "../util/src/dstarlite2.h"
#include <stdlib.h>




static int k = 0;
static float C1 = 1;

static void initializeMazeNode(MazeNode *mn);
static unsigned long int minCSG(MazeNode *q, Graph *gr);
static unsigned long int minimum(unsigned long int f1, unsigned long int f2);
static int testin(Queue *queue, MazeNode *mn1);
static QueueNode* same(Queue *queue, MazeNode *mn1);


void dstarmain(mz_dat *init_data){
	unsigned long int vn=0,vw=0,ve=0,vs=0,v1=0,v2=0,v3 = 0; //Values corresponding to c + g
	unsigned long int c;
	MazeNode *s_last = init_data->end;
	MazeNode *s_start = init_data->end;
	MazeNode *s_goal = init_data->begin;
	Graph *gr = init_data->gr;
	int *dir = init_data->dir;
	Queue *heap = createQueue();
	MazeNode *s_temp;
	dStarInit(s_start,s_goal,gr,heap);
	computeSP(s_start,s_goal,heap,gr);
	while (s_start != s_goal){
		
		if(s_start->rhs >= INT_MAX){
			fprintf(stderr,"\nNO ROUTE TO GOAL\n");

		}

		c = ((unsigned long int) s_start->south)*INT_MAX + !((unsigned long int)s_start->south)*1;
		if (s_start->position.y < gr->h-1) vs = gr->table[s_start->position.x][s_start->position.y+1].g + c;
			
		
		c = ((unsigned long int) s_start->west)*INT_MAX + !((unsigned long int)s_start->west)*1;
		if (s_start->position.x > 0) vw = gr->table[s_start->position.x-1][s_start->position.y].g + c;
		
		c = ((unsigned long int) s_start->east)*INT_MAX + !((unsigned long int)s_start->east)*1;
		if (s_start->position.x < gr->w-1) ve = gr->table[s_start->position.x+1][s_start->position.y].g + c;

		c = ((unsigned long int) s_start->north)*INT_MAX + !((unsigned long int)s_start->north)*1;
		if (s_start->position.y > 0) vn = gr->table[s_start->position.x][s_start->position.y-1].g + c;
		
		if (vs == minimum(vn,vs)) v1 = vs;
		else v1 = vn;
		if (vw == minimum(ve,vw)) v2 = vw;
		else v2 = ve;
		v3 = minimum(v1,v2);
		
		s_temp = s_start;
		if (v3 == vn && s_start->position.y > 0) {
			s_start = &(gr->table[s_start->position.x][s_start->position.y-1]);
			fprintf(stdout,"Move North\n");
		}
		else if (v3 == vs && s_start->position.y < gr-> h - 1){
			s_start = &(gr->table[s_start->position.x][s_start->position.y+1]);
			//Move South
			fprintf(stdout,"Move South\n");
			}
		else if (v3 == vw && s_start->position.x > 0){
			s_start = &(gr->table[s_start->position.x-1][s_start->position.y]);
			//Move West
			fprintf(stdout,"Move West\n");
			}
		else if (v3 == ve && s_start->position.x < gr->w - 1){
			s_start = &(gr->table[s_start->position.x+1][s_start->position.y]);
			//Move East
			fprintf(stdout,"Move East\n");
			}
		fprintf(stdout,"%u,%u\n",s_start->position.x,s_start->position.y);
		/*
		while(*dir >= 0);
		//Wait for response
		if (*dir == -2){
			//Change update the wall position
			//Update the costs
			k += heuristic(&(s_last->position),&(s_start->position));
			//Because we know the position didn't change, test between expected position
			if (s_last->position.x != s_start->position.x){
				if (s_last->position.x < s_start->position.x){
					s_last->east = WALL;
					s_start->west = WALL;
				}
				else{
					s_last->west = WALL;
					s_start->east = WALL;
				}
			}
			else{
				if (s_last->position.y < s_start->position.y){
					s_last->north = WALL;
					s_start->south = WALL;
				}
				else{
					s_last->south = WALL;
					s_start->north = WALL;
				}
			}
			
			if(s_last->rhs == 1 + s_start->g){
				if (s_last != s_goal) s_last->rhs = minimum(s_last->rhs,1 + s_start->g);
				
			}
			if (s_start->rhs == 1 + s_last->g){
				if (s_start != s_goal)
				{
					s_start->rhs = minimum(s_last->rhs,0);
				}
			}
			updateVertex(s_last, heap, gr);
			updateVertex(s_start, heap, gr);
			s_start = s_temp;
			s_last = s_start;
			computeSP(s_start,s_goal,heap,gr);
			
			
			
			
			
		
		
		}*/
		
			
		
	}

	
}

void dStarInit(MazeNode *s_start, MazeNode *s_goal, Graph *gr, Queue *heap)
{
	NodeKey nk;
	nk.val1 = heuristic(&(s_start->position),&(s_goal->position));
	nk.val2 = 0;
	s_goal->rhs = 0;
	s_goal->key = nk;
	
	PriorityAdd(heap,s_start,compareKey);
}

void updateVertex(MazeNode *u,MazeNode* start, Queue *heap, Graph *gr)
{
	QueueNode *tmp;
	if ((((u->rhs < INT_MAX) && (u->g >= INT_MAX) && testin(heap,u)) || ((u->g != u->rhs) && testin(heap,u)))) {
		tmp = same(heap,u);
		removeNode(tmp, heap);
		u->key = calculateKey(u, start, gr);
		PriorityAdd(heap,u,compareKey);
	}
	
	else if ((u->g != u->rhs) && !testin(heap,u)){
		u->key = calculateKey(u, start, gr);
		PriorityAdd(heap,u,compareKey);
	}
	
	else if(u->g == u->rhs && testin(heap,u)){
		tmp = same(heap,u);
		removeNode(tmp, heap);
	}
	
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
	new->table = calloc(width,sizeof(int*));
	new->h = height;
	new->w = width;

	if(new->table == NULL){
		fprintf(stderr, "out of memory\n");
		return NULL;
	}
	
	for(i = 0; i < height; i++){
	
		new->table[i] = calloc(height, sizeof(MazeNode));
		MALLOC_CHECK(stderr, new->table[i]); 
	}
	
	for (i = 0; i< width; i++){
		for(j = 0; j < height; j++){
			
			
			b = &(new->table[i][j]);
			xy.x = i;
			xy.y = j;
			b->position = xy;
			initializeMazeNode(b);
			if (i == 0){
				b->west = WALL;
			}
			else if (i==width-1){
				b->east = WALL;
			}
			
			if (j == 0){
				b->north = WALL;
			}
			else if (j == width-1){
				b->south = WALL;
			}
			
		}
	}
	return new;
}

NodeKey calculateKey(MazeNode *s,MazeNode *start, Graph *gr)
{
	NodeKey nk = s->key;
	nk.val1 = minimum(s->g,s->rhs) + heuristic(&(s->position),&(start->position)) + k;
	nk.val2 = minimum(s->g,s->rhs);
	return nk;
}

static unsigned long int minimum(unsigned long int f1, unsigned long int f2){
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

int compareKey(void *p1, void *p2){
	if(!p1 || !p2) return 0;
	MazeNode *mn1 = (MazeNode *) p1;
	MazeNode *mn2 = (MazeNode *) p2;
	return ((mn1->key.val1 > mn2->key.val1) || ((mn1->key.val1 == mn2->key.val1) && (mn1->key.val2 > mn2->key.val2)));
}

void computeSP(MazeNode *s_start, MazeNode* s_goal, Queue *heap, Graph *gr){
	MazeNode mn2;
	unsigned long int g_old = 0;
	g_old = g_old;
	XYPos ft;
	NodeKey k_old;
	NodeKey k_new;
	
	while( (heap->head) && ((!compareKey(top(heap)->data,s_start) || (s_start->rhs > s_start->g)))){
		QueueNode *nos;
		QueueNode *tmp = top(heap);
		MazeNode *u = (MazeNode *) tmp->data;
		k_old = u->key;
		k_new = calculateKey(u, s_start, gr);
		if (compareKey(&k_old, &k_new)){
			nos = pop(heap);
			u->key = k_new;
			PriorityAdd(heap,u,compareKey);
			free(nos);
			}
		else if (u->g > u->rhs){
			u->g = u->rhs;
			removeNode(tmp, heap);
			ft = u->position;
			
			if (ft.y > 0){
				mn2 = gr->table[ft.x][ft.y-1];
				if(mn2.position.x != ft.x || mn2.position.y != ft.y){
					unsigned long int c;
					c = ((unsigned long int) mn2.north)*INT_MAX + !((unsigned long int)mn2.north)*1;
					mn2.rhs = minimum(mn2.rhs,c + u->g);
					updateVertex(&mn2,s_start,heap,gr);
					gr->table[ft.x][ft.y-1] = mn2;
				}
			}
			
			if (ft.y < gr->h - 1){
				mn2 = gr->table[ft.x][ft.y+1];
				if(mn2.position.x != ft.x || mn2.position.y != ft.y){
					unsigned long int c;
					c = ((unsigned long int) mn2.south)*INT_MAX + !((unsigned long int)mn2.south)*1;
					mn2.rhs = minimum(mn2.rhs,c + u->g);
					updateVertex(&mn2,s_start,heap,gr);
					gr->table[ft.x][ft.y+1] = mn2;
				}
			}
			
			if (ft.x > 0){
				mn2 = gr->table[ft.x - 1][ft.y];
				if(mn2.position.x != ft.x || mn2.position.y != ft.y){
					unsigned long int c;
					c = ((unsigned long int) mn2.west)*INT_MAX + !((unsigned long int)mn2.west)*1;
					mn2.rhs = minimum(mn2.rhs,c + u->g);
					updateVertex(&mn2,s_start,heap,gr);
					gr->table[ft.x-1][ft.y] = mn2;
				}
			}
			
			if (ft.x < gr->w -1){
				mn2 = gr->table[ft.x+1][ft.y];
				if(mn2.position.x != ft.x || mn2.position.y != ft.y){
					unsigned long int c;
					c = ((unsigned long int) mn2.east)*INT_MAX + !((unsigned long int)mn2.east)*1;
					mn2.rhs = minimum(mn2.rhs,c + u->g);
					updateVertex(&mn2, s_start, heap,gr);
					gr->table[ft.x+1][ft.y] = mn2;
				}
			}
		
		
			
		}
		else{
			g_old = u->g;
			u->g = INT_MAX;
			MazeNode *tmp2;
			
			//For all predecessors of u and u
			/*
			*/
			if (u->rhs == 1 + g_old || (g_old +1 >= INT_MAX && u->rhs >= INT_MAX))
			{
				if (u != s_goal) u->rhs = minCSG(u,gr);
			
			}
			
			if (u->position.x > 0)
			{
				tmp2 = &(gr->table[u->position.x-1][u->position.y]);
				if (tmp2 != s_goal) tmp2->rhs = minCSG(tmp2,gr);
				updateVertex(tmp2,s_start,heap,gr);
			}
			
			if (u->position.x < gr->w - 1)
			{
				tmp2 = &(gr->table[u->position.x+1][u->position.y]);
				if (tmp2 != s_goal) tmp2->rhs = minCSG(tmp2,gr);
				updateVertex(tmp2,s_start,heap,gr);
			}
			
			if (u->position.y > 0)
			{
				tmp2 = &(gr->table[u->position.x][u->position.y - 1]);
				if (tmp2 != s_goal) tmp2->rhs = minCSG(tmp2,gr);
				updateVertex(tmp2,s_start,heap,gr);
			}
			
			if (u->position.x < gr->h - 1)
			{
				tmp2 = &(gr->table[u->position.x][u->position.y+1]);
				if (tmp2 != s_goal) tmp2->rhs = minCSG(tmp2,gr);
				updateVertex(tmp2,s_start,heap,gr);
			}
			
		}
	}
}

static unsigned long int minCSG(MazeNode *q, Graph *gr)
{
		unsigned long int c;
		unsigned long int vs,vw,ve,vn, v1,v2,v3;
		
		c = ((unsigned long int) q->south)*INT_MAX + !((unsigned long int)q->south)*1;
		if (q->position.y < gr->h-1) vs = gr->table[q->position.x][q->position.y+1].g + c;
			
		
		c = ((unsigned long int) q->west)*INT_MAX + !((unsigned long int)q->west)*1;
		if (q->position.x > 0) vw = gr->table[q->position.x-1][q->position.y].g + c;
		
		c = ((unsigned long int) q->east)*INT_MAX + !((unsigned long int)q->east)*1;
		if (q->position.x < gr->w-1) ve = gr->table[q->position.x+1][q->position.y].g + c;

		c = ((unsigned long int) q->north)*INT_MAX + !((unsigned long int)q->north)*1;
		if (q->position.y > 0 ) vn = gr->table[q->position.x][q->position.y-1].g + c;
		
		if (vs == minimum(vn,vs)) v1 = vs;
		else v1 = vn;
		if (vw == minimum(ve,vw)) v2 = vw;
		else v2 = ve;
		v3 = minimum(v1,v2);
		return v3;
		
		
}


static int testin(Queue *queue, MazeNode *mn1){
	if (queue->head == NULL){
		return 0;
	}
	QueueNode *tmp = queue->head;
	MazeNode *tmn = (MazeNode *) tmp->data;
	
	while (tmp != NULL)
	{
		if (tmn->position.x == mn1->position.x){
			if (tmn->position.y == mn1->position.y){
				return 1;
			}
		}
		tmp = tmp->next;
	}
	
	return 0;
	
	
}

static QueueNode* same(Queue *queue, MazeNode *mn1){
	if (queue->head == NULL){
		return NULL;
	}
	QueueNode *tmp = queue->head;
	MazeNode *tmn = (MazeNode *) tmp->data;
	
	while (tmp != NULL)
	{
		if (tmn->position.x == mn1->position.x){
			if (tmn->position.y == mn1->position.y){
				return tmp;
			}
		}
		tmp = tmp->next;
	}
	
	return NULL;
}



