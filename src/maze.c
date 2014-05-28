/* ========================================================================== */
 /* 
 * File: maze.c - Amazing Project
 *
 * Author: Emily Greene
 * Date: 05/28/2014
 *
 * Description: Graphically depicts the maze and the avatar's motion through the maze
 *
 */
/* ========================================================================== */
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
#include "../util/src/amazing.h"
#include "../util/src/utils.h"
#include "maze.h"

// ---------------- Constant/Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes

/*
 * Function to get the maze log from the server with all of the maze cells that server creates
 *
 * Input: the MazePort of this specific run
 *
 * Output: a system call to get the file off of the server
 */
void getlog(uint32_t mazeport) {
	uint32_t port = mazeport;
	
	// copy the specific MazePort name into the file path
	char *filename = "/var/tmp/%zu/log.out";
	char buffer[24];
	sprintf(buffer,filename,port);
	
	// create the beginning and the end of the system command
	char *start = "scp $USER@pierce.cs.dartmouth.edu:";
	char *end = " maze.log";
	
	// allocate memory
	char *command = calloc((strlen(start) + strlen(buffer) + strlen(end) + 1),sizeof(char));
	
	// concatinate the parts of the system command
	strcat(command,start);
	strcat(command,buffer);
	strcat(command,end);
	
	// call the command on a new BASH shell
	system(command);

	// free resources
	free(command);
}


/*
 * Function to parse the maze log from the server and create a two dimensional array of MazeCells for
 * graphic animation
 *
 * Input: the width and height of the maze
 *
 * Output: a two dimensional array containing an (x,y) position of the node and the presence of walls
 * in the node
 */
MazeCell *** parselog(uint32_t mazewidth, uint32_t mazeheight){
	// define constants
	uint32_t width = mazewidth;
	uint32_t height = mazeheight;
	int linelen = 55; // max length of a line of the log file
	int counter = 0;
	
	FILE *logfile; // log file from the server

	char *block; // one line in the file
	char *readin; // string to skip when reading the file
	char *cell; // pointer

	// coordinates of the MazeCell
	uint32_t xcord;
	uint32_t ycord;
	MazeCell *newnode;
	MazeCell ***array = calloc(width,sizeof(int*));
	int a;
	for (a=0;a<width;a++) {
		array[a] = calloc(height,sizeof(int*));
	}
	
	// attempt to open the log file
	if ((logfile = fopen("maze.log", "r")) == NULL) {
		printf("Could not open file\n");
		exit(1);
	}
	
	// allocting the strings
	block = calloc(linelen+1,sizeof(char));
	readin = calloc(strlen("MazeCell") +1, sizeof(char));
	
	// iterate through the file line by line
	while ((cell = fgets(block,linelen,logfile)) != NULL) {
	
		// read the line only if it starts with "MazeCell"
		sscanf(cell,"%s",readin);
		if (strcmp(readin,"MazeCell") == 0) {
			newnode = calloc(1,sizeof(MazeCell));		
			// update the pointer
			cell += strlen(readin) + 2;
	
			// assign the y coodinate of the cell
			sscanf(cell,"%u",&ycord);
			newnode->position.y = ycord;

			// update the pointer
			cell += 4;

			// assign the x coordinate of the cell
			sscanf(cell,"%u",&xcord);
			newnode->position.x = xcord;

			// update the pointer
			cell += 5 + strlen("walls: ");

			// assign the walls (W if there's a wall, P if there's a path)
			if (cell[0] == 'W') {
				newnode->west = W;
			}
			else {
				newnode->west = P;
			}
			cell += 1;
			if (cell[0] == 'N') {
				newnode->north = W;
			}
			else {
				newnode->north = P;
			}
			cell += 1;
			if (cell[0] == 'S') {
				newnode->south = W;
			}
			else {
				newnode->south = P;
			}
			cell += 1;
			if (cell[0] == 'E') {
				newnode->east = W;
			}
			else {
				newnode->east = P;
			}

			// add the node to the array
			array[xcord][ycord] = newnode;
			counter += 1;

			// update the pointer
			cell += strlen("borders: WNSE");
		}
	}
	free(readin);
	free(block);
	fclose(logfile);
	return array;
}

/*
 * Function to update the graphics based on the current locations of the avatars
 * 
 * Input: two dimensional array created by parselog, width and height of the maze, the AM_Message
 * and number of avatars
 *
 * Output: prints out an updated maze
 *
 */
void update(MazeCell ***array,uint32_t MazeWidth,uint32_t MazeHeight, AM_Message msg, int nAvatars) {
	// Constants
	uint32_t width = MazeWidth;
	uint32_t height = MazeHeight;

	MazeCell *node;
	int e;
	int f;
	for (e=0;e<height;e++) {
		for (f=0;f<width;f++) {

			node = array[f][e];
			node->maze_boolean = 0;

		} 
	}
	
	// if the server sends an avatar turn message
	if (ntohl(msg.type) == AM_AVATAR_TURN) {

		// iterate through all the avatars and add their respective (x,y) coordinates
		// as booleans (avatar present)
		int a;
		for (a=0;a<nAvatars;a++) {
			uint32_t x = ntohl(msg.avatar_turn.Pos[a].x);
			uint32_t y = ntohl(msg.avatar_turn.Pos[a].y);
			node = array[x][y];
			node->maze_boolean = 1;
		}
	}

	printf("\n");

	// iterate through the array and check for the presence of a north wall
	int i;
	int j;
	for (j=0;j<height;j++) {
		for (i=0;i<width;i++) {
			node = array[i][j];
			if (node->north == W) {
				if (node->west == W && node->east == W) {
					printf("Г¯Ⴈ");
				}
				else if (node->west == W) {
					printf("Г¯¯");
				}
				else if (node->east == W) {
					printf("¯¯Ⴈ");
				}
				else {
					printf("¯¯¯");
				}
			}
			else {
				if (node->west == W && node->east == W) {
					printf("| ⎟");
				}
				else if (node->west == W) {
					printf("|  ");
				}
				else if (node->east == W) {
					printf("  ⎟");
				}
				else {
					printf("   ");
				}
			}
		}
		printf("\n");

		// iterate through the array and check for the presence of east and west walls
		int n;
		for (n=0;n<width;n++) {
			node = array[n][j];

			// if the boolean is 1, there is an avatar in the node, and 
			// an "X" should be printed to mark the avatar
			if (node->maze_boolean == 1) {

				if (node->west == W && node->east == W) {
					printf("|X⎟");
				}
				else if (node->west == W) {
					printf("|X ");
				}
				else if (node->east == W) {
					printf(" X⎟");
				}
				else {
					printf(" X ");
				}
			}
			else {
				if (node->west == W && node->east == W) {
					printf("| ⎟");
				}
				else if (node->west == W) {
					printf("|  ");
				}
				else if (node->east == W) {
					printf("  ⎟");
				}
				else {
					printf("   ");
				}
			}
		}
		
		printf("\n");
		
		// iterate through the array to check for a south wall
		int m;
		for (m=0;m<width;m++) {
			node = array[m][j];
			if (node->south == W) {
				if (node->west == W && node->east == W) {
					printf("L_⎦");
				}
				else if (node->west == W) {
					printf("L__");
				}
				else if (node->east == W) {
					printf("__⎦");
				}
				else {
					printf("___");
				}
			}
			else {
				if (node->west == W && node->east == W) {
					printf("| ⎟");
				}
				else if (node->west == W) {
					printf("|  ");
				}
				else if (node->east == W) {
					printf("  ⎟");
				}
				else {
					printf("   ");
				}
			}
		}
		printf("\n");
	}
	
}

/*
 * Function to free the two dimensional array
 *
 * Input: two dimensional array to free, the width and height of the array
 *
 * Output: free resources
 *
 */
void freeMaze(MazeCell ***array, uint32_t width, uint32_t height){
	int i;
	int j;
	if (!array) return;
	for (i=0;i<width;i++){
		for(j=0;j<height;j++){
			free(array[i][j]);
		}
		free(array[i]);
	}
	free(array);
	return;
}
