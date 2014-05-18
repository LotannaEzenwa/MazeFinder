/* ========================================================================== */
/* File: AMStartup.c - Final Project 
 *
 * Author: Janice Yip
 * Date: 05/16/2014
 *
 * Description: validates the arguments, constructs and sends AM_INIT message to server.
 * 		When server responds with AM_INIT_OK, AMStartup recovers MazePort from the reply 
 * Commandline input: AMStartup [NAVATARS] [DIFFICULTY] [HOSTNAME] 
 *
 * 
 * Example command input
 * $ ./AMStartup 3 2 pierce.cs.dartmouth.edu  
 * 
 * [NAVATARS] -> 3
 * Requirement: Must be positive number
 * Usage: total number of Avatars generated for the maze 
 * 
 * [DIFFICULTY] -> 2
 * Requirement: Must be an integer between 0 and 9 inclusive 
 * Usage: the difficulty level, on the scale 0 (easy) to 9 (excruciatingly difficult)  
 * 
 * [HOSTNAME] -> pierce.cs.dartmouth.edu
 * Requirement: Must be a server
 * Usage: server name we want IP address of   
 *
 * Output:  
 * 
 * Error conditions: See requirements for each commandline input above
 * 
 * Pseudocode:
 *     1.   
 *     2.  
 *     3.  
 *     4.  
 *     5.    
 *     6.  
 * 	   7.  
 *     8.  
 *     9.   
 */

/* ========================================================================== */
// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <stdio.h>                           // printf
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


// ---------------- Local includes  e.g., "file.h"
#include "../util/src/amazing.h"
#include "../util/src/utils.h"


// ---------------- Constant definitions

// ---------------- Macro definitions


// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes
int IsNotNumeric(char *input);


/* =========================================================================== */
/*                                   main                                      */
/* =========================================================================== */

int main(int argc, char* argv[])
{
	int avatarId; 
	int nAvatars; 
	int Difficulty; 
//	char *filename; 
	int sockinit; 
	struct sockaddr_in servaddr;
	int MazePort; 
	int MazeWidth; 
	int MazeHeight; 

	/******************************* args check *******************************/
	if (argc < 4) {
		fprintf(stderr, "Incorrect number of arguments. Exiting now.\n"); 
		exit(1); 
	}

	// check that the numbering of avatars starts at 0 
    if (IsNotNumeric(argv[1])) {
		fprintf(stderr, "Number of avatars must be a number. Exiting now.\n"); 
    	exit(1); 
    } else if ((atoi(argv[1]) != 0)) {
    	fprintf(stderr, "ID of avatars must start at 0. Exiting now.\n"); 
    	exit(1); 
    } else {
    	avatarId = atoi(argv[1]); 
    	printf("avatarId: %d\n", avatarId); 
    }

	// check the input for number of avatars 
    if (IsNotNumeric(argv[2])) {
		fprintf(stderr, "Number of avatars must be a number. Exiting now.\n"); 
    	exit(1); 
    } else if ((atoi(argv[2]) < 0)) {
    	fprintf(stderr, "Number of avatars must be greater than 0. Exiting now.\n"); 
    	exit(1); 
    } else {
    	nAvatars = atoi(argv[2]); 
    	printf("Number avatars: %d\n", nAvatars); 
    }

    // check input for difficulty of maze 
    if (IsNotNumeric(argv[3])) {
		fprintf(stderr, "Difficulty must be a number. Exiting now.\n"); 
    	exit(1); 
    } else if ((atoi(argv[3]) < 0) || (atoi(argv[3]) > 9)) {
    	fprintf(stderr, "Difficulty level must be between 0 and 9. Exiting now.\n"); 
    	exit(1); 
    } else {
    	Difficulty = atoi(argv[3]); 
    	printf("Difficulty %d\n", Difficulty); 
    }

    // check hostname 



	/************************** send AM_INIT message **************************/
	// create a socket for the client
	if ((sockinit = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Problem creating the socket.\n");
		exit(2);
	}

	// creation of the socket
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr= inet_addr(argv[4]);
	servaddr.sin_port =  htons(atoi(AM_SERVER_PORT)); //convert to big-endian order

	// connection of the client to the socket 
	if (connect(sockinit, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
		perror("Problem connecting to the server.\n");
		exit(3);
	} 

	// send the message
	AM_Message *initialize;

	initialize = calloc(1, sizeof(AM_Message)); 
	MALLOC_CHECK(stderr,initialize);  

	initialize->type = htonl(AM_INIT); 
	initialize->init.nAvatars = htonl(nAvatars); 
	initialize->init.Difficulty = htonl(Difficulty); 

	send(sockinit, initialize, sizeof(initialize), 0);

	printf("sent\n"); 

	/************************** send AM_INIT message **************************/
	// receive a reply  
	AM_Message *initreply; 

	initreply = calloc(1, sizeof(AM_Message)); 
	MALLOC_CHECK(stderr,initreply); 

	if( recv(sockinit, initreply, sizeof(initreply) , 0) < 0)
	{
		perror("The server terminated prematurely.\n");
		exit(4); 
	}

	// catch errors? 
	if (IS_AM_ERROR(initreply->type))
	{
	    // something went wrong
	    perror("Something went wrong.\n");
	    exit(5);
	} else if (ntohl(initreply->type) != AM_INIT_OK) {
		printf("not ok\n"); 
	} else {
		printf("received\n"); 
		printf("init type: %d\n", ntohl(initreply->type)); 

		// set the variables based on the reply 
		MazePort = ntohl(initreply->init_ok.MazePort);  
		MazeWidth = ntohl(initreply->init_ok.MazeWidth); 
		MazeHeight = ntohl(initreply->init_ok.MazeHeight); 
		printf("originalwidth: %d\n", initreply->init_ok.MazeWidth); 
		printf("Port:%d\n", MazePort); 
		printf("Width:%d\n", MazeWidth); 
		printf("Height: %d\n", MazeHeight); 
	}

	/**************************** start Avatars ****************************/
	
/*	AMStartup extracts the MazePort from the message, 
	and then starts up N threads or processes (one for each Avatar) 
	running the main client software, each with the appropriate start 
	parameters (see Startup section below).






If the initialization succeeds, the server will respond with an AM_INIT_OK message.

If nAvatars is greater than AM_MAX_AVATAR or the Difficulty is greater than 
AM_MAX_DIFFICULTY, the server will respond with an AM_INIT_FAILED message.

*/
	/**************************** create log file ****************************/


}




/* =========================================================================== */
/*                              IsNotNumeric	                               */
/* =========================================================================== */
/*  Checks that the input is a number
 *  
 *  @input: input from commandline (should be a number)
 *
 *  Returns 0 on success; otherwise, -1, if the inputed string is not a number.
 */
int IsNotNumeric(char *input)
{
    int i;
    for ( i=0; i<strlen(input); i++ ) {
        if (!isdigit(input[i])) {
            fprintf(stderr, "Not a positive number.\n");
            return(-1); // failure
        }
    }
    return(0); // success
}