#include "../../src/dstarlite.c"

static int TEST1(); //Manhattan Heuristic Test
static int TEST2(); //Compare Function Test
//static int TEST3(); //Priority Queue Test

int main(int argc, char *argv[]){
	
	
	 
	if(TEST1()){fprintf(stderr, "ERROR DURING TEST 1"); return 1;}
	if(TEST2()){fprintf(stderr, "ERROR DURING TEST 2"); return 2;}
	
	
	
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
