#include <time.h>
#include "../src/queue.h"
#include "../../src/dstarlite.c"


static int TEST1(); //Manhattan Heuristic Test
static int TEST2(); //Compare Function Test
static int TEST3(); //Priority Queue Test
static int TEST4(); //Graph Testing

int main(int argc, char *argv[]){
	
	
	 
	if(TEST1()){fprintf(stderr, "ERROR DURING TEST 1"); return 1;}
	if(TEST2()){fprintf(stderr, "ERROR DURING TEST 2"); return 2;}
	if(TEST3()){fprintf(stderr, "ERROR DURING TEST 3"); return 3;}
	if(TEST4()){fprintf(stderr, "ERROR DURING TEST 4"); return 4;}
	
	
	return 0;
}

static int TEST1(){
	int h;
	XYPos pos1;
	XYPos pos2;
	pos1.x = 20;
	pos1.y = -4;
	pos2.x = -23;
	pos2.y = -3;
	h = heuristic(&pos1,&pos2);
	printf("\nThe Heuristic Returned %d\n",h);
	

	return h != 44;
}

static int TEST2(){
	
	int b;
	int c;
	MazeNode mn1;
	MazeNode mn2;
	mn1.costToGoal = 1.0;
	mn2.costToGoal = 3.0;
	b = compareCosts(&mn1,&mn2);
	c = compareCosts(&mn2,&mn1);
	printf("\nThe Compare Function Returned %d and %d.\n",b,c);
	
	return 0;
}

static int TEST3(){
	float tmp = -21;
	int j = 0;
	srand(time(NULL));
	MazeNode mn1;
	MazeNode mn2;
	MazeNode mn3;
	MazeNode mn4;
	MazeNode mn5;
	MazeNode mn6;
	MazeNode mn7;
	Queue *Open = createQueue();
	
	mn1.costToGoal = rand()%41 - 20;
	mn2.costToGoal = rand()%41 - 20;
	mn3.costToGoal = rand()%41 - 20;
	mn4.costToGoal = rand()%41 - 20;
	mn5.costToGoal = rand()%41 - 20;
	mn6.costToGoal = rand()%41 - 20;
	mn7.costToGoal = rand()%41 - 20;

	PriorityAdd(Open, &mn1,compareCosts);
	PriorityAdd(Open, &mn2,compareCosts);
	PriorityAdd(Open, &mn3,compareCosts);
	PriorityAdd(Open, &mn4,compareCosts);
	PriorityAdd(Open, &mn5,compareCosts);
	PriorityAdd(Open, &mn6,compareCosts);
	PriorityAdd(Open, &mn7,compareCosts);
	MazeNode *mn0;
	QueueNode *qn0;
	
	for(;j<7;j++){
		
		qn0 = pop(Open);
		mn0	= qn0->data;
		
		if(mn0){
			
			if (tmp <= mn0->costToGoal){
				tmp = mn0->costToGoal;
				
			}
			else{
				return 1;
			}
		
		}
		free(qn0);
		
		
	}
	free(Open);
	fprintf(stdout, "\nThe Priority Queue is in correct order.\n");
	return 0;
	

}

static int TEST4(){
	int i = 0;
	Graph *tmp = constructGraph(10,10);
	tmp->table[2][9].costToGoal = 4;
	fprintf(stdout,"\nThe Created Node has a value of %.2f.\n",tmp->table[2][9].costToGoal);
	for (;i<10;i++){
		free(tmp->table[i]);
	}
	free(tmp->table);
	free(tmp);
	return 0;
}
