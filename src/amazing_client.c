/* ========================================================================== */
/* File: amazing_client.c - Final Project 
 *
 * Author: Oseleta
 * Date: 05/16/2014
 *
 * Description: validates the arguments, constructs and sends AM_INIT message to server.
 * 		When server responds with AM_INIT_OK, AMStartup recovers MazePort from the reply 
 * Commandline input: AMStartup [AVATARID] [NAVATARS] [DIFFICULTY] [IPADDRESS] [MAZEPORT] [FILENAME] 
 *
 * 
 * Example command input
 * $ ./amazing_client 0 3 2 129.170.212.235 10829 Amazing_3_2.log 
 * 
 * [AVATARID] -> 0
 * Requirement: 
 * Usage: an integer generated by AMStartup, starting at 0 and incremented 
 * 		by one for each subsequent Avatar started  
 * 
 * [NAVATARS] -> 3
 * Requirement: Must be positive number
 * Usage: total number of Avatars generated for the maze 
 * 
 * [DIFFICULTY] -> 2
 * Requirement: Must be an integer between 0 and 9 inclusive 
 * Usage: the difficulty level, on the scale 0 (easy) to 9 (excruciatingly difficult)  
 * 
 * [IPADDRESS] -> 129.170.212.235
 * Requirement: Must be a number
 * Usage: IP address of the server  
 *
 * [MAZEPORT] -> 10829
 * Requirement:  
 * Usage: MazePort returned in the AM_INIT_OK message  
 * 
 * [FILENAME] -> Amazing_3_2.log
 * Requirement: 
 * Usage: Filename of the log the Avatar should open for writing in append mode.  
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



	/**************************** create log file ****************************/


}

/*

The startup script, AMStartup, can be a shell script or C program. It should be able to handle these parameters at a minimum:

-n nAvatars 
(int) the number of Avatars in the maze
-d Difficulty
(int) the difficulty level, on the scale 0 (easy) to 9 (excruciatingly difficult)
-h Hostname
(char *) the hostname of the server. Our server will be running on pierce.cs.dartmouth.edu .
Once AMStartup has validated the arguments, it will construct and send the AM_INIT message to the server. 
When the server responds with AM_INIT_OK, AMStartup recovers the MazePort from that reply.

Then AMStartup creates a new log file with the name Amazing_$USER_N_D.log, where $USER is the current userid, 
N is the value of nAvatars and D is the value of Difficulty. The first line of the file should contain $USER, 
the MazePort, and the date & time. For example, the following shell command would produce this first line:

echo $USER, 10829, `date` > $filename
where $filename is a variable containing the name of the log file. Alternatively, if your AMStartup is a C program, 
then you may find the time() and ctime() functions in time.h helpful.
Then AMStartup will start the N copies of the Avatar client (as threads or processes) with the parameters they will need:

AvatarId (an integer generated by AMStartup, starting at 0 and incremented by one for each subsequent Avatar started)
nAvatars (total number of Avatars)
Difficulty (difficulty of the maze)
IP address of the server
MazePort (as returned in the AM_INIT_OK message).
Filename of the log the Avatar should open for writing in append mode.
While validity checking of the parameters in the startup script (program) is appropriate (defensive programming!), the client program is not really meant to be run by people, so the start parameters can be simply positional and required. For example, if you use a shell script to start the main client program the line might look like this:

./amazing_client 0 3 2 129.170.212.235 10829 Amazing_3_2.log
Of course, if you would prefer to setup these parameters with "getopt" style switches, that's ok too.

If you wish to add additional parameters, please speak with your customers (the TA and/or the instructor) first.


*/



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