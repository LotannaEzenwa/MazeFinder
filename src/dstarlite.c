
#include "dstarlite.h"
#include <stdlib.h>





int heuristic(XYPos *p1, XYPos *p2){
	uint32_t dx = p1->x - p2->x;
	uint32_t dy = p1->y - p2->y;
	return abs(dx) + abs(dy);
}

