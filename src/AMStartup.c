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
 * Output: Starts N processes of avatars  
 * 
 * Error conditions: See requirements for each commandline input above
 * 
 * Pseudocode:
 *     1.  Checks commandline arguments 
 * 	   2.  Runs and sends an AM_INIT message to the server  
 *     3.  Server responds with AM_INIT_OK message and listens to client 
 *     4.  Extracts the MazePort from message 
 *     5.  Writes info to file 
 *     6.  Creates and initializes shared memory 
 *     7.  Starts N (number of avatars) processes (amazing_client.c) 
 * 	   8.  Exits successfully 
 */

/* ========================================================================== */
#define _GNU_SOURCE
// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <stdio.h>                           // printf
#include <sys/types.h> 
#include <sys/stat.h> 
#include <sys/shm.h>		// for shared memory
#include <sys/sem.h>		// for semaphores 
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <pthread.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h> //For errno - the error number
#include <netdb.h> //hostent
#include <sys/wait.h>

// ---------------- Local includes  e.g., "file.h"
#include "../util/src/amazing.h"
#include "../util/src/utils.h"
#include "maze.h"

// ---------------- Constant definitions

// ---------------- Macro definitions
#define MAX_FILE_NAME 100
#define MAX_CMD_LEN 100 
#define MAX_ID_LEN 5
#define MAX_PORT_LEN 10 
#define MAX_KEY_LEN 10
#define MAX_WIDTH 102
#define MAX_HEIGHT 102 

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes
int IsNotNumeric(char *input);

void InitializeMaze(int MazeHeight, int MazeWidth, int * maze); 

/* =========================================================================== */
/*                                   main                                      */
/* =========================================================================== */

int main(int argc, char* argv[])
{
	int nAvatars; 
	int Difficulty; 
	char filename[MAX_FILE_NAME]; 
	int sockinit; 
	struct sockaddr_in servaddr;
	uint32_t MazePort; 
	uint32_t MazeWidth; 
	uint32_t MazeHeight; 
	time_t cur;
	FILE *fp; 
	int shmid;

	char key[MAX_KEY_LEN]; 
	char width[MAX_KEY_LEN]; 
	char height[MAX_KEY_LEN]; 
	int *shared_mem; 
	int maze[MAX_HEIGHT *  MAX_WIDTH]; 


	/******************************* args check *******************************/
	if (argc < 4) {
		fprintf(stderr, "Incorrect number of arguments. Exiting now.\n"); 
		exit(1); 
	}

	// check the input for number of avatars 
    if (IsNotNumeric(argv[1])) {
		fprintf(stderr, "Number of avatars must be a number. Exiting now.\n"); 
    	exit(1); 
    } else if ((atoi(argv[1]) < 0)) {
    	fprintf(stderr, "Number of avatars must be greater than 0. Exiting now.\n"); 
    	exit(1); 
    } else {
    	nAvatars = atoi(argv[1]); 
    	printf("Number avatars: %d\n", nAvatars); 
    }

    // check input for difficulty of maze 
    if (IsNotNumeric(argv[2])) {
		fprintf(stderr, "Difficulty must be a number. Exiting now.\n"); 
    	exit(1); 
    } else if ((atoi(argv[2]) < 0) || (atoi(argv[2]) > 9)) {
    	fprintf(stderr, "Difficulty level must be between 0 and 9. Exiting now.\n"); 
    	exit(1); 
    } else {
    	Difficulty = atoi(argv[2]); 
    	printf("Difficulty %d\n", Difficulty); 
    }

	// check hostname 
	struct hostent *he;
	struct in_addr ipadd;

	he = gethostbyname(argv[3]); 
	ipadd = *(struct in_addr *)he->h_addr_list[0]; 

	/************************** send AM_INIT message **************************/
	// create a socket for the client
	if ((sockinit = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Problem creating the socket.\n");
		exit(2);
	}

	// creation of the socket
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr= inet_addr(inet_ntoa(ipadd));
	servaddr.sin_port =  htons(atoi(AM_SERVER_PORT)); //convert to big-endian order

	// connection of the client to the socket 
	if (connect(sockinit, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
		perror("Problem connecting to the server.\n");
		exit(3);
	} 

	// send the message
	AM_Message msg;

	msg.type = htonl(AM_INIT); 
	msg.init.nAvatars = htonl(nAvatars); 
	msg.init.Difficulty = htonl(Difficulty); 

	send(sockinit, &msg, sizeof(msg), 0);

	/*********************** receive AM_INIT_OK message ***********************/
	// receive a reply   
	if( recv(sockinit, &msg, sizeof(msg) , 0) < 0) {
		perror("The server terminated prematurely.\n");
		exit(4); 
	}

	// catch errors
	if (IS_AM_ERROR(msg.type)) {
	    perror("Something went wrong.\n");
	    exit(5);
	} else {
		if (ntohl(msg.type) != AM_INIT_OK) {
			perror("Message received was not AM_INIT_OK.\n"); 
			exit(6); 
		} else {
			// set the variables based on the reply 
			MazePort = ntohl(msg.init_ok.MazePort);  
			MazeWidth = ntohl(msg.init_ok.MazeWidth); 
			MazeHeight = ntohl(msg.init_ok.MazeHeight); 
		} 
	}

	// create log file for avatars 
	time (&cur);
	char * id = getenv("USER"); 
	sprintf(filename,"AMAZING_%s_%d_%d.log", id, nAvatars, Difficulty); 

	// get log for graphics
	getlog(MazePort);

	// first line of file should contain $USER, the MazePort, and the date & time
	fp = fopen(filename, "w"); 
	printf("%s, %d, %s\n", id, MazePort, ctime(&cur)); 
	fprintf(fp, "%s, %d, %s\n", id, MazePort, ctime(&cur)); 
	fclose(fp); 

	/************************** open shared memory **************************/
    shmid = shmget((key_t)1129, sizeof(maze), 0666 | IPC_CREAT);

    if (shmid == -1) {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    } else {
    	sprintf(key, "%d", shmid); 
    	// initialize the memory 
    	shared_mem = shmat(shmid, (void *)0, 0);

    	// initialize the maze in shared memory 
    	InitializeMaze(MazeHeight, MazeWidth, shared_mem); 
    }

	/***************************** start Avatars *****************************/
	// fork processes so that each avatar gets its own id 
	int i; 
	pid_t pid;

	for ( i=0; i < nAvatars; i++ ) {
		pid = fork();

		if (pid == -1) {
			perror("Failed to fork.\n"); 
		    exit(EXIT_FAILURE); 	// error, failed to fork()
		} else if (pid > 0) {
			
		} else {
		    char avId[MAX_ID_LEN]; 
			char port[MAX_PORT_LEN]; 
			sprintf(avId, "%d", i); 
			sprintf(port, "%d", MazePort); 
			sprintf(width, "%d", MazeWidth); 
			sprintf(height, "%d", MazeHeight); 

			// execute the amazing clients
		    char *args[11] = { "./amazing_client", avId, argv[1], argv[2], 
		    		inet_ntoa(ipadd), port, filename, key, width, height, NULL }; 
			execvp(args[0], args);

		    _exit(EXIT_FAILURE);   // exec never returns
		}
	}

	// end this program 
	return(0); 
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




/* =========================================================================== */
/*                              InitializeMaze	                               */
/* =========================================================================== */
/*  Assigns 0 to every cell of the maze, meaning walls are unknown
 *  
 *  @MazeHeight: height of maze returned from server
 * 	@MazeWidth: width of maze returned from server
 * 	@maze: pointer to the shared memory of the maze 
 */
void InitializeMaze(int MazeHeight, int MazeWidth, int * maze) 
{
	int i; 
	for (i=0; i<(MazeHeight * MazeWidth); i++) {
			maze[i] = 0; 
	} 
}
