#include "../../src/dstarlite.c"

int main(int argc, char *argv[]){
	
	XYPos pos1;
	XYPos pos2;
	pos1.x = 20;
	pos1.y = -4;
	pos2.x = -23;
	pos2.y = -3;
	printf("\nThe Heuristic Returned %d\n",heuristic(&pos1,&pos2));
	
	
	return 0;
}
