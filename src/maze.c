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
// ---------------- Local includes  e.g., "file.h"

// ---------------- Constant/Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes

int main(int argc,char *argv[]) {
	FILE *logfile;
	char *filename = "/var/tmp/%s/log.out";
	char buffer[24];

	if (argc != 2) {
		printf("Error, improper number of arguments");
		exit(1);
	}
	
	char *portnum = argv[1];	

	sprintf(buffer,filename,portnum);
	
	char *start = "scp $USER@pierce.cs.dartmouth.edu:";
	char *end = " maze.log";
	char *command = calloc((strlen(start) + strlen(buffer) + strlen(end) + 1),sizeof(char));
	strcat(command,start);
	strcat(command,buffer);
	strcat(command,end);
	

	system(command);

	if (logfile = fopen("maze.log", "r") == NULL) {
		printf("Could not open file\n");
		exit(1);
	}

	
	return 0;
}
