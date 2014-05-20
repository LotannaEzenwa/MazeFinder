/* ========================================================================== */
/* File: maze.c - Final Project 
 *
 * Author: Emily Greene
 * Date: 05/19/2014
 *
 * Description: Graphically depicts the maze and the avatar's motion through the maze
 */
/* ======================================================================================*/
// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <stdio.h>                           // printf
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

// ---------------- Local includes  e.g., "file.h"
#include "dstarlite.h"

// ---------------- Constant/Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes

void maze(uint32_t mazeport){
	FILE *logfile;
	char *filename = "/var/tmp/%zu/log.out";
	char buffer[24];
	char *block;
	char *cell;
	char *readin = calloc(strlen("MazeCell") +1, sizeof(char));
	int linelen = 45;
	MazeNode *newnode;
	uint32_t xcord;
	uint32_t ycord;
	
	sprintf(buffer,filename,mazeport);
	
	char *start = "scp $USER@pierce.cs.dartmouth.edu:";
	char *end = " maze.log";
	char *command = calloc((strlen(start) + strlen(buffer) + strlen(end) + 1),sizeof(char));
	strcat(command,start);
	strcat(command,buffer);
	strcat(command,end);
	

	system(command);

	if ((logfile = fopen("maze.log", "r")) == NULL) {
		printf("Could not open file\n");
		exit(1);
	}

	block = calloc(linelen+1,sizeof(char));
	while ((cell = fgets(block,linelen,logfile)) != NULL) {
		sscanf(cell,"%s",readin);
		if (strcmp(readin,"MazeCell") == 0) {
			newnode = calloc(1,sizeof(MazeNode));
			cell += strlen(readin) + 2;
			sscanf(cell,"%zu",&xcord);
			newnode.position.x = xcord;
			cell += 4;
			sscanf(cell,"%zu",&ycord);
			newnode.position.y = ycord;
			printf("x:%zu y:%zu ",xcord,ycord);
			cell += 5 + strlen("walls: ");
			printf("next: %c",cell[0]);
			if (cell[0] == "W") {
				newnode.west = WALL;
				printf("WALL");
			}
			else {
				newnode.west = PATH;
			}
			cell += 1;
			if (cell[0] == "N") {
				newnode.north = WALL;
			}
			else {
				newnode.north = PATH;
			}
			cell += 1;
			if (cell[0] == "S") {
				newnode.south = WALL;
			}
			else {
				newnode.south = PATH;
			}
			cell += 1:
			if (cell[0] == "E") {
				newnode.east = WALL;
			}
			else {
				newnode.east = PATH;
			}
			
		}
	}
	
}
