/* ========================================================================== */
/* File: amazing_client.c - Final Project 
 *
 * Author: Oseleta
 * Date: 05/16/2014
 *
 * Description: validates the arguments, constructs and sends AM_INIT message to server.
 *      When server responds with AM_INIT_OK, AMStartup recovers MazePort from the reply 
 * Commandline input: amazing_client [AVATARID] [NAVATARS] [DIFFICULTY] [IPADDRESS] 
 *          [MAZEPORT] [FILENAME] [SHMID]
 *
 * 
 * Example command input
 * $ ./amazing_client 0 3 2 129.170.212.235 10829 Amazing_3_2.log 
 * 
 * [AVATARID] -> 0
 * Requirement: 
 * Usage: an integer generated by AMStartup, starting at 0 and incremented 
 *      by one for each subsequent Avatar started  
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
 * [SHMID] -> 1234
 * Requirement: 4 digit number 
 * Usage: Key for accessing shared memory  
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
 *     7.  
 *     8.  
 *     9.   
 */

/* ========================================================================== */
#define _GNU_SOURCE
// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <stdio.h>                           // printf
#include <sys/types.h> 
#include <sys/stat.h> 
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h> 
#include <semaphore.h>


// ---------------- Local includes  e.g., "file.h"
#include "../util/src/amazing.h"
#include "../util/src/utils.h"
#include "../util/src/shm_com.h"
#include "../util/src/dstarlite2.h"
#include "maze.h"

// ---------------- Constant definitions

// ---------------- Macro definitions
#define MAX_IP_LEN 100
#define MAX_FILE_NAME 100

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes
int IsNotNumeric(char *input);


static void del_semvalue(void);
static int semaphore_p(void);
static int semaphore_v(void);


static int sem_id;


/* =========================================================================== */
/*                                   main                                      */
/* =========================================================================== */

int main(int argc, char* argv[])
{
	int avatarId; 
	int nAvatars; 
	int Difficulty; 
    char ipAddress[MAX_IP_LEN]; 
	char filename[MAX_FILE_NAME]; 
    float xAvg; 
    float yAvg; 
    int MazeWidth; 
    int MazeHeight; 
    int xpos; 
    int ypos; 
    // for shared memory 
    MazeMemory *shared_mem; 
    int shmid;
    
    // for sockets 
	int sockfd; 
	struct sockaddr_in servaddr;
	int MazePort; 
    time_t cur;
    FILE *fp; 
    int first = 1; 


	/******************************* args check *******************************/
	if (argc != 8) {
		perror("AC: Incorrect number of arguments. Exiting now.\n"); 
		exit(1); 
	} else {
        printf("getting called\n"); 
    }

	// check that the avatarid 
    if (IsNotNumeric(argv[1])) {
		fprintf(stderr, "Number of avatars must be a number. Exiting now.\n"); 
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

    // copy ipAddress  
    if (strlen(argv[4]) >= MAX_IP_LEN) {
        perror("Ip address too long. Exiting now.\n"); 
        exit(1); 
    } else {
        strcpy(ipAddress, argv[4]);  
    }

    // MazePort 
    if (IsNotNumeric(argv[5])) {
        perror("MazePort wrong. Exiting now.\n"); 
        exit(1); 
    } else {
        MazePort = atoi(argv[5]); 
    }

    // get filename 
    if (strlen(argv[6]) >= MAX_FILE_NAME) {
        perror("Filename too long. Exiting now.\n"); 
        exit(1); 
    } else {
        strcpy(filename,argv[6]); 
    }

    // check key passed in  
    if (IsNotNumeric(argv[7])) {
        perror("Key for shared memory wrong. Exiting now.\n"); 
        exit(1); 
    } else {
        shmid = atoi(argv[7]); 
    }    
    
    /************************ attach to shared memory ************************/

    // make the shared memory accessible to the program 
    shared_mem = shmat(shmid, (void *)0, 0);
    if (shared_mem == (void *)-1) {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Memory attached at %p\n", (void *)shared_mem);

    // initialize semaphore 
    sem_id = semget((key_t)2345, 1, 0666); 

	/************************ tell server avatar ready ************************/
    // create a socket for the client
    if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Problem creating the socket.\n");
        exit(2);
    }

    // creation of the socket
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr= inet_addr(ipAddress);
    servaddr.sin_port =  htons(MazePort); //convert to big-endian order

    // connection of the client to the socket 
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("Problem connecting to the server.\n");
        exit(3);
    } 

    
    // create and send message 
    AM_Message msg;

    msg.type = htonl(AM_AVATAR_READY); 
    msg.avatar_ready.AvatarId = htonl(avatarId); 
    
    send(sockfd, &msg, sizeof(msg), 0);

    printf("sent\n"); 

    /******* this z variable is so you can limit the number of steps for testing *******/ 
    /******* used in the while loop below ********/ 
    int z = 0; 


    int dir = 0; 
    /************************** listen for avatarID **************************/
    while (( recv(sockfd, &msg, sizeof(msg) , 0) >= 0 ) && z<1000) {
        printf("received: %d\n", avatarId); 

        // check if error 
        if (IS_AM_ERROR(ntohl(msg.type))) {
            perror("Something went wrong.\n");
            // detach the memory 
            if (shmdt(shared_mem) == -1) {
                perror("shmdt failed\n");
                exit(EXIT_FAILURE);
            }

            // one avatar should delete the memory 
            if (avatarId == 0) {
                if (shmctl(shmid, IPC_RMID, 0) == -1) {
                    perror("shmctl(IPC_RMID) failed\n");
                    exit(EXIT_FAILURE);
                }
            }
            exit(4);
        } 

        // check message type 
        if (ntohl(msg.type) == AM_AVATAR_TURN) {
	    // update the maze
	    update(array,MazeWidth,MazeHeight,msg.avatar_turn,nAvatars);

            // the first time it receives a message, find central point 
            if (first) {
                int i; 
                for ( i = 0; i < nAvatars; i++ ) {
                    xAvg += ntohl(msg.avatar_turn.Pos[i].x);  
                    yAvg += ntohl(msg.avatar_turn.Pos[i].y);  
                }

                xAvg = xAvg / nAvatars; 
                yAvg = yAvg / nAvatars; 

                xAvg = (int) xAvg; 
                yAvg = (int) yAvg; 

                first = 0; 
            }

            // if the avatar is the one to move, move 
            if (avatarId == ntohl(msg.avatar_turn.TurnId)) {

                // grab the key 
                if (!semaphore_p()) exit(EXIT_FAILURE);

                // get the avatar's current location
                xpos = ntohl(msg.avatar_turn.Pos[avatarId].x); 
                ypos = ntohl(msg.avatar_turn.Pos[avatarId].y); 

                // make a move 
                printf("make a move\n"); 
                msg.type = htonl(AM_AVATAR_MOVE); 
                msg.avatar_move.AvatarId = htonl(avatarId); 

            
                /* This is a random algorithm for the sake of testing. */ 

                /******** comment out when you want to test dstarlite ********/ 
                srand ( time(NULL) );
                int random_number = rand();

                msg.avatar_move.Direction = htonl(random_number%4);  

/*                switch (dir) {
                    case 0: 
                        msg.avatar_move.Direction = htonl(0);                
                        break; 
                    case 1: 
                        msg.avatar_move.Direction = htonl(1);                
                        break; 
                    case 2: 
                        msg.avatar_move.Direction = htonl(3);                
                        break; 
                    case 3: 
                        msg.avatar_move.Direction = htonl(2);   
                        dir = 0;              
                        break; 
                    default: 
                        break; 
                }
*/
                
                /********* The algorithm should go here. **********/  
                /* shared_mem is a pointer that points to the shared memory, 
                        which contains a MazeMemory (see util/src/dstarlite2.h) */
                
                /* dstarlite */ 








                // log the avatar's move to a file 
                fp = fopen(filename, "a"); 
                ASSERT_FAIL(stderr, fp); 

                fprintf(fp, "Id: %d, Move: %d\n", avatarId, ntohl(msg.avatar_move.Direction)); 
//                fclose(fp); 


                send(sockfd, &msg, sizeof(msg), 0);

                // listen for reply 
                if (recv(sockfd, &msg, sizeof(msg) , 0) >= 0) {
/*                    fprintf(fp,"prevx: %d\n", xpos); 
                    fprintf(fp,"prevy: %d\n", ypos); 
                    
                    fprintf(fp,"movex: %d\n", (ntohl(msg.avatar_turn.Pos[avatarId].x))); 
                    fprintf(fp,"movey: %d\n", (ntohl(msg.avatar_turn.Pos[avatarId].y))); 
*/ 

                    // if current position is same as last
                    if ( (ntohl(msg.avatar_turn.Pos[avatarId].x) == xpos) 
                        && (ntohl(msg.avatar_turn.Pos[avatarId].y) == ypos) ) {

                        // hit a wall 
                        printf("hit a wall\n"); 
                        fprintf(fp, "Hit a wall\n"); 

                        /******* update the shared memory to reflect the wall *******/






                        //dir++;      // change direction (for a random algorithm, just ignore)
                    }
                    printf("received message\n"); 
                }
                fclose(fp);  

                // release the key 
                if (!semaphore_v()) exit(EXIT_FAILURE);

            } else {
                continue; 
                printf("not my turn\n"); 
            }
        } 
        if (ntohl(msg.type) == AM_MAZE_SOLVED) {
            // make sure only one avatar writes to the file 
            if (avatarId == 0) {
                // write to file that program finished 
                fp = fopen(filename, "a"); 
                ASSERT_FAIL(stderr, fp); 

                time (&cur);
                

                fprintf(fp, "Solved the maze at %s!\n", ctime(&cur)); 
                fclose(fp); 
            }
//            printf("Solved the maze\n"); 
            exit(EXIT_SUCCESS); 
        }
        z++; 
    } 

    // detach the memory 
    if (shmdt(shared_mem) == -1) {
        perror("shmdt failed\n");
        exit(EXIT_FAILURE);
    }

    sleep(2); 

    // one avatar should delete the memory 
    if (avatarId == 0) {
        if (shmctl(shmid, IPC_RMID, 0) == -1) {
            perror("shmctl(IPC_RMID) failed\n");
            exit(EXIT_FAILURE);
        }
        del_semvalue();
    }

    printf("ended\n"); 

    exit(EXIT_SUCCESS);
}

/* =========================================================================== */
/*                              IsNotNumeric                                   */
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
/*            Semaphore Functions, taken from Dartmouth CS Resources           */
/* =========================================================================== */
/* The del_semvalue function has almost the same form, except the call to semctl uses
 the command IPC_RMID to remove the semaphore's ID. */

static void del_semvalue(void)
{
    union semun sem_union;
    
    if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
        fprintf(stderr, "Failed to delete semaphore\n");
}

/* semaphore_p changes the semaphore by -1 (waiting). */

static int semaphore_p(void)
{
    struct sembuf sem_b;
    
    sem_b.sem_num = 0;
    sem_b.sem_op = -1; /* P() */
    sem_b.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_b, 1) == -1) {
        fprintf(stderr, "semaphore_p failed\n");
        return(0);
    }
    return(1);
}

/* semaphore_v is similar except for setting the sem_op part of the sembuf structure to 1,
 so that the semaphore becomes available. */

static int semaphore_v(void)
{
    struct sembuf sem_b;
    
    sem_b.sem_num = 0;
    sem_b.sem_op = 1; /* V() */
    sem_b.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_b, 1) == -1) {
        fprintf(stderr, "semaphore_v failed\n");
        return(0);
    }
    return(1);
}
