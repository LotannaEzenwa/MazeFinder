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
#include "../util/src/amazing.h"
#include "../util/src/utils.h"

// ---------------- Constant/Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes
void parselog(uint32_t mazeport, uint32_t mazewidth, uint32_t mazeheight){
	uint32_t port = mazeport;
	uint32_t width = mazewidth;
	uint32_t height = mazeheight;
	FILE *logfile;
	char *filename = "/var/tmp/%zu/log.out";
	char buffer[24];
	char *block;
	char *cell;
	char *readin = calloc(strlen("MazeCell") +1, sizeof(char));
	int linelen = 55;
	MazeNode *newnode;
	MazeNode *array[width+1][height + 1];
	uint32_t xcord;
	uint32_t ycord;
	
	sprintf(buffer,filename,port);
	
	char *start = "scp $USER@pierce.cs.dartmouth.edu:";
	char *end = " maze.log";
	char *command = calloc((strlen(start) + strlen(buffer) + strlen(end) + 1),sizeof(char));
	strcat(command,start);
	strcat(command,buffer);
	strcat(command,end);
	
	
	printf("presystem ");

	system(command);

	if ((logfile = fopen("maze.log", "r")) == NULL) {
		printf("Could not open file\n");
		exit(1);
	}
	printf("open logfile ");
	int counter = 0;
	block = calloc(linelen+1,sizeof(char));
	while ((cell = fgets(block,linelen,logfile)) != NULL) {
		printf("loop ");
		sscanf(cell,"%s",readin);
		if (strcmp(readin,"MazeCell") == 0) {
			newnode = calloc(1,sizeof(MazeNode));
			cell += strlen(readin) + 2;
			sscanf(cell,"%u",&ycord);
			newnode->position.y = ycord;
			cell += 4;
			sscanf(cell,"%u",&xcord);
			newnode->position.x = xcord;
			cell += 5 + strlen("walls: ");
			if (cell[0] == 'W') {
				newnode->west = WALL;
			}
			else {
				newnode->west = PATH;
			}
			cell += 1;
			if (cell[0] == 'N') {
				newnode->north = WALL;
			}
			else {
				newnode->north = PATH;
			}
			cell += 1;
			if (cell[0] == 'S') {
				newnode->south = WALL;
			}
			else {
				newnode->south = PATH;
			}
			cell += 1;
			if (cell[0] == 'E') {
				newnode->east = WALL;
			}
			else {
				newnode->east = PATH;
			}
			array[xcord][ycord] = newnode;
			counter += 1;
			cell += strlen("borders: WNSE");
		}
	}
	printf("\n");
	MazeNode *node = calloc(1,sizeof(MazeNode));
	int i;
	int j;
	for (j=0;j<height;j++) {
		for (i=0;i<width;i++) {
			node = array[i][j];
			if (node->north == WALL) {
				printf("OOOO");
			}
			else {
				if (node->west == WALL) {
					printf("O    ");
				}
				else {
					printf("    ");
				}
			}
		}
		printf("\n");
		int k;
		for (k=0; k<2; k++) {
		int n;
		for (n=0;n<width;n++) {
			node = array[n][j];
			if (node->west == WALL && node->east == WALL) {
				printf("O  O");
			}
			else if (node->west == WALL) {
				printf("O   ");
			}
			else if (node->east == WALL) {
				printf("   O");
			}
			else {
				printf("    ");
			}
		}
		printf("\n");
		}
		int m;
		for (m=0;m<width;m++) {
			node = array[m][j];
			if (node->south == WALL) {
				printf("OOOO");
			}
			else {
				printf("    ");
			}
		}
		printf("\n");
	}
}






	/*	if (node->north == WALL && node->south == WALL) {
			printf("****");
			if (node->west == WALL) {
				if (node->east == WALL) {
					printf("*  *");
					printf("*  *");
				}
				else {
					printf("*   ");
					printf("*   ");
				}
			}
			else {
				if (node->east == WALL) {
					printf("   *");
					printf("   *");
				}
				else {
					printf("    ");
					printf("    ");
				}
			}
			printf("****");
		}
		else if (node->north == WALL) {
			printf("****");
			if (node->west == WALL) {
				if (node->east == WALL) {
					printf("*  *");
					printf("*  *");
					printf("*  *");
				}
				else {
					printf("*   ");
					printf("*   ");
					printf("*   ");
				}
			}
			else {
				if (node->east == WALL) {
					printf("   *");
					printf("   *");
					printf("   *");
				}
				else {
					printf("    ");
					printf("    ");
					printf("    ");
				}
			}
		
		}	
		else if (node ->south == WALL) {
			if (node->west == WALL) {
				if (node->east == WALL) {
					printf("*  *");
					printf("*  *");
					printf("*  *");
				}
				else {
					printf("*   ");
					printf("*   ");
					printf("*   ");
				}
			}
			else {
				if (node->east == WALL) {
					printf("   *");
					printf("   *");
					printf("   *");
				}
				else {
					printf("    ");
					printf("    ");
					printf("    ");
				}
			}
			printf("****");
		}
		else {
			if (node->west == WALL) {
				if (node->east == WALL) {
					printf("*  *");
					printf("*  *");
					printf("*  *");
					printf("*  *");
				}
				else {
					printf("*   ");
					printf("*   ");
					printf("*   ");
					printf("*   ");
				}
			}
			else {
				if (node->east == WALL) {
					printf("   *");
					printf("   *");
					printf("   *");
					printf("   *");
				}
				else {
					printf("    ");
					printf("    ");
					printf("    ");
					printf("    ");
				}
			}
		}
	}
}*/
