/* ========================================================================== */
 /* Author: Emily Greene
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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// ---------------- Local includes  e.g., "file.h"
#include "dstarlite.h"
#include "../util/src/amazing.h"
#include "../util/src/utils.h"

// ---------------- Constant/Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes
MazeNode *** parselog(uint32_t mazeport, uint32_t mazewidth, uint32_t mazeheight){
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
	uint32_t xcord;
	uint32_t ycord;
	MazeNode *newnode;
	MazeNode ***array = calloc(width,sizeof(MazeNode));
	int a;
	for (a=0;a<width;a++) {
		array[a] = calloc(height,sizeof(MazeNode));
	}
	sprintf(buffer,filename,port);
	
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
	int counter = 0;
	block = calloc(linelen+1,sizeof(char));
	while ((cell = fgets(block,linelen,logfile)) != NULL) {
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
	return array;
}
void update(MazeNode ***array,uint32_t MazeWidth,uint32_t MazeHeight, AM_Message msg, int nAvatars) {
	printf("\n");
	uint32_t width = MazeWidth;
	uint32_t height = MazeHeight;
	MazeNode *node = calloc(1,sizeof(MazeNode));
	if (ntohl(msg.type) == AM_AVATAR_TURN) {
		int a;
		for (a=0;a<nAvatars;a++) {
			uint32_t x = ntohl(msg.avatar_turn.Pos[a].x);
			uint32_t y = ntohl(msg.avatar_turn.Pos[a].y);
			node = array[x][y];
			node->maze_boolean = 1;
		}
	}
	
	int i;
	int j;
	for (j=0;j<height;j++) {
		for (i=0;i<width;i++) {
			node = array[i][j];
			if (node->north == WALL) {
				printf("OOOOO");
			}
			else {
				if (node->west == WALL && node->east == WALL) {
					printf("O   O");
				}
				else if (node->west == WALL) {
					printf("O    ");
				}
				else if (node->east == WALL) {
					printf("    O");
				}
				else {
					printf("     ");
				}
			}
		}
		printf("\n");
		int n;
		for (n=0;n<width;n++) {
			node = array[n][j];
			if (node->maze_boolean == 1) {

				if (node->west == WALL && node->east == WALL) {
					printf("O X O");
				}
				else if (node->west == WALL) {
					printf("O X  ");
				}
				else if (node->east == WALL) {
					printf("  X O");
				}
				else {
					printf("  X  ");
				}
			}
			else {
				if (node->west == WALL && node->east == WALL) {
					printf("O   O");
				}
				else if (node->west == WALL) {
					printf("O    ");
				}
				else if (node->east == WALL) {
					printf("    O");
				}
				else {
					printf("     ");
				}
			}
		}
		printf("\n");
		int o;
		for (o=0;o<width;o++) {
			node = array[o][j];
			if (node->west == WALL && node->east == WALL) {
				printf("O   O");
			}
			else if (node->west == WALL) {
				printf("O    ");
			}
			else if (node->east == WALL) {
				printf("    O");
			}
			else {
				printf("     ");
			}
		}
		printf("\n");
		int m;
		for (m=0;m<width;m++) {
			node = array[m][j];
			if (node->south == WALL) {
				printf("OOOOO");
			}
			else {
				if (node->west == WALL && node->east == WALL) {
					printf("O   O");
				}
				else if (node->west == WALL) {
					printf("O    ");
				}
				else if (node->east == WALL) {
					printf("    O");
				}
				else {
					printf("     ");
				}
			}
		}
		printf("\n");
	}
}

