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
//#include "dstarlite.h"

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
	int xcord;
	int ycord;
	
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
			sscanf(cell,"%d",&xcord);
			cell += 4;
			sscanf(cell,"%d",&ycord);
			printf("x:%d y:%d ",xcord,ycord);
			cell += 5 + strlen("walls: ");
			printf("next: %c",cell[0]);
			
		}
	}
	
}
