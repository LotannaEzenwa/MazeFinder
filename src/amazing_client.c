/* ========================================================================== */
/* File: amazing_client.c - Final Project 
 *
 * Author: Oseleta
 * Date: 05/16/2014
 *
 * Description: validates the arguments, constructs and sends AM_INIT message to server.
 *      When server responds with AM_INIT_OK, AMStartup recovers MazePort from the reply 
 * Commandline input: amazing_client [AVATARID] [NAVATARS] [DIFFICULTY] [IPADDRESS] 
 *          [MAZEPORT] [FILENAME] [SHMID] [MAZEWIDTH] [MAZEHEIGHT]
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
 * [MAZEWIDTH] -> 20
 * Requirement: Positive number 
 * Usage: Maze's width, given by server   
 * 
 * [MAZEHEIGHT] -> 20
 * Requirement: Positive number 
 * Usage: Maze's height, given by server  
 * 
 * Output: All the avatars finding each other in a maze  
 * 
 * Error conditions: See requirements for each commandline input above
 * 
 * Pseudocode:
 *     1.  Check commandline arguments 
 *     2.  Attach to shared memory and send an AM_AVATAR_READY message to the server 
 *     3.  Receive server's AM_AVATAR_TURN message and if first turn, calculate central point
 *     4.  If it's the avatar's turn, check to see if it hit a wall last time 
 *     5.  If so, update wall within shared memory
 *     6.  If not, recalculate priorities based on "Follow Right Wall" algorithm 
 *     7.  If already where avatar needs to be, stay still 
 *     8.  Otherwise, move in direction dictated by priority array  
 *     9.  Repeat until server sends AM_MAZE_SOLVED message 
 *     10. Detach memory and sleep for a second 
 *     11. One avatar deletes memory and writes end to file 
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


// ---------------- Local includes  e.g., "file.h"
#include "../util/src/amazing.h"
#include "../util/src/utils.h"
#include "../util/src/maze.h"

// ---------------- Constant definitions

// ---------------- Macro definitions
#define MAX_IP_LEN 100
#define MAX_FILE_NAME 100
#define W_WALL 1
#define N_WALL 2
#define S_WALL 4
#define E_WALL 8

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes
int IsNotNumeric(char *input);

int HasNorthWall(int index); 
int HasWestWall(int index); 
int HasSouthWall(int index); 
int HasEastWall(int index); 

static int *shared_mem; 


/* =========================================================================== */
/*                                   main                                      */
/* =========================================================================== */

int main(int argc, char* argv[])
{
    int avatarId; 
    int nAvatars; 
    char ipAddress[MAX_IP_LEN]; 
    char filename[MAX_FILE_NAME]; 
    float xAvg; 
    float yAvg; 
    int xlast; 
    int ylast; 
    int xcurr; 
    int ycurr; 
    int MazeWidth; 
    int MazeHeight; 

    // for shared memory 
    int shmid;
    
    // for sockets 
    int sockfd; 
    struct sockaddr_in servaddr;
    int MazePort; 
    time_t cur;
    FILE *fp; 
    int first = 1; 
    int arrived = 0;
    int index; 

    int direction[4]; 


    /******************************* args check *******************************/
    if (argc != 10) {
        perror("AC: Incorrect number of arguments. Exiting now.\n"); 
        exit(1); 
    }

    // check that the avatarid 
    if (IsNotNumeric(argv[1])) {
        fprintf(stderr, "Number of avatars must be a number. Exiting now.\n"); 
        exit(1); 
    } else {
        avatarId = atoi(argv[1]); 
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
    }

    // check input for difficulty of maze 
    if (IsNotNumeric(argv[3])) {
        fprintf(stderr, "Difficulty must be a number. Exiting now.\n"); 
        exit(1); 
    } else if ((atoi(argv[3]) < 0) || (atoi(argv[3]) > 9)) {
        fprintf(stderr, "Difficulty level must be between 0 and 9. Exiting now.\n"); 
        exit(1); 
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

    // check width and height passed in  
    if (IsNotNumeric(argv[8]) || IsNotNumeric(argv[9])) {
        perror("MazeWidth or MazeHeight wrong. Exiting now.\n"); 
        exit(1); 
    } else {
        MazeWidth = atoi(argv[8]); 
        MazeHeight = atoi(argv[9]);
    }     
    
    /************************ attach to shared memory ************************/

    // make the shared memory accessible to the program 
    shared_mem = shmat(shmid, (void *)0, 0);
    if (shared_mem == (void *)-1) {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }

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

    int dir = 0; 

    /************************** listen for avatarID **************************/

    // initialize the two dimensional array and draw its initial state
    MazeCell ***maze;
    maze = parselog(MazeWidth,MazeHeight);
    update(maze,MazeWidth,MazeHeight,msg,nAvatars);
  
    while (( recv(sockfd, &msg, sizeof(msg) , 0) >= 0 )) {

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

            // the first time it receives a message, find central point 
            if (first) {
  
              // get the central point
                int i; 
                for ( i = 0; i < nAvatars; i++ ) {
                    xAvg += ntohl(msg.avatar_turn.Pos[i].x);  
                    yAvg += ntohl(msg.avatar_turn.Pos[i].y);  
                }

                xAvg = xAvg / nAvatars; 
                yAvg = yAvg / nAvatars; 

                xAvg = (int) xAvg; 
                yAvg = (int) yAvg; 

                // set "first" flag to false 
                first = 0; 

                // set the array direction to go right 
                direction[0] = M_NORTH;

                xlast = -1; 
                ylast = -1;  
            }

     	    // update the graphics after all the avatars move once 
     	    // (when the turn message is directed towards the first avatar again)
    	    if (ntohl(msg.avatar_turn.TurnId == 0)) {
    	        update(maze,MazeWidth,MazeHeight,msg,nAvatars);
            }

	        // if the avatar is the one to move, move 
            if (avatarId == ntohl(msg.avatar_turn.TurnId)) {

                // set current position 
                xcurr = ntohl(msg.avatar_turn.Pos[avatarId].x); 
                ycurr = ntohl(msg.avatar_turn.Pos[avatarId].y); 
                
                // log the avatar's move to a file 
                fp = fopen(filename, "a"); 
                ASSERT_FAIL(stderr, fp); 

                // if current position is same as last
                if ( (xcurr == xlast) && (ycurr == ylast) ) {

                    if(!arrived) {
                        // hit a wall 
                        index = (ylast * MazeWidth) + xlast; 
                        
                        /******* update the shared memory to reflect the wall *******/
                        switch(direction[dir]){
                            case M_NORTH: 
                                shared_mem[index] += N_WALL; 
                                if ( ((index - MazeWidth) >= 0) && (!HasSouthWall(index - MazeWidth)) ) {
                                    shared_mem[index - MazeWidth] += S_WALL; 
                                }
                                break;
                            case M_EAST: 
                                shared_mem[index] += E_WALL; 
                                if ( ((index + 1) < (MazeHeight * MazeWidth)) && (!HasWestWall(index + 1)) ) {
                                    shared_mem[index + 1] += W_WALL; 
                                }
                                break;
                            case M_SOUTH: 
                                shared_mem[index] += S_WALL; 
                                if ( ((index + MazeWidth) < (MazeHeight * MazeWidth)) && (!HasNorthWall(index + MazeWidth)) ) {
                                    shared_mem[index + MazeWidth] += N_WALL; 
                                }
                                break;
                            case M_WEST: 
                                shared_mem[index] += W_WALL; 
                                if ( ((index - 1) >= 0) && (!HasEastWall(index - 1)) ) {
                                    shared_mem[index - 1] += E_WALL; 
                                }
                                break;
                        }
                        dir++;
                    }
                    
                } else {
                    // figure out which way avatar moved 
                    switch(direction[dir]){
                        case M_NORTH:
                            direction[0] = M_EAST;
                            direction[1] = M_NORTH;
                            direction[2] = M_WEST;
                            direction[3] = M_SOUTH;
                            break;
                        case M_EAST:
                            direction[0] = M_SOUTH;
                            direction[1] = M_EAST;
                            direction[2] = M_NORTH;
                            direction[3] = M_WEST;
                            break;
                        case M_SOUTH:
                            direction[0] = M_WEST;
                            direction[1] = M_SOUTH;
                            direction[2] = M_EAST;
                            direction[3] = M_NORTH;
                            break;
                        case M_WEST:
                            direction[0] = M_NORTH;
                            direction[1] = M_WEST;
                            direction[2] = M_SOUTH;
                            direction[3] = M_EAST;
                            break;
                    }

                    // reset the direction priority
                    dir = 0;
                }


                /************************ Send the move ************************/ 
                // time to make a move 
                msg.type = htonl(AM_AVATAR_MOVE); 
                msg.avatar_move.AvatarId = htonl(avatarId); 
                
                if ( (ntohl(msg.avatar_turn.Pos[avatarId].x) == xAvg) 
                    && (ntohl(msg.avatar_turn.Pos[avatarId].y) == yAvg) ) {
                    arrived = 1;
                    // don't move
                    msg.avatar_move.Direction = htonl(M_NULL_MOVE);
                } else {
                    // move in next direction dictated by direction array
                    msg.avatar_move.Direction = htonl(direction[dir]);
                }

                send(sockfd, &msg, sizeof(msg), 0);

                fprintf(fp, "Avatar Id: %d, Move: %d\n", avatarId, direction[dir]); 
                fprintf(fp, "xlast: %d, ylast: %d, xcurr: %d, ycurr: %d\n", xlast, ylast, xcurr, ycurr); 
                fclose(fp);  

                // assign the avatar's last location
                xlast = xcurr; 
                ylast = ycurr; 

            } else {
                continue; 
            }
        } 
        if (ntohl(msg.type) == AM_MAZE_SOLVED) {

            // detach the memory 
            if (shmdt(shared_mem) == -1) {
                perror("shmdt failed\n");
                exit(EXIT_FAILURE);
            }

            sleep(2); 

            // make sure only one avatar writes to the file and deletes the memory 
            if (avatarId == 0) {
                // write to file that program finished 
                fp = fopen(filename, "a"); 
                ASSERT_FAIL(stderr, fp); 

                time (&cur);
                
                fprintf(fp, "Solved the maze in %d moves at %s!\n", ntohl(msg.maze_solved.nMoves), ctime(&cur)); 
                fclose(fp); 

                // one avatar should delete the memory 
                if (shmctl(shmid, IPC_RMID, 0) == -1) {
                    perror("shmctl(IPC_RMID) failed\n");
                    exit(EXIT_FAILURE);
                }
                
            }
            printf("Solved the maze\n");
     	    freeMaze(maze,MazeWidth,MazeHeight);
            exit(EXIT_SUCCESS); 
        }
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
    }

    exit(EXIT_FAILURE);
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
/*                              HasNorthWall                                   */
/* =========================================================================== */
/*  Checks whether the current maze cell has a north wall 
 *  
 *  @index: cell of the maze to check
 *
 *  Returns 1 if it has the wall; otherwise, 0 (as true and false booleans)
 */
int HasNorthWall(int index) 
{
    switch(shared_mem[index]){
        case 2: 
            return 1; 
        case 3: 
            return 1; 
        case 6: 
            return 1; 
        case 10: 
            return 1;
        case 7: 
            return 1;
        case 11: 
            return 1;
        case 14: 
            return 1;
        case 15: 
            return 1;
    }
    return 0; 
}


/* =========================================================================== */
/*                               HasWestWall                                   */
/* =========================================================================== */
/*  Checks whether the current maze cell has a west wall 
 *  
 *  @index: cell of the maze to check
 *
 *  Returns 1 if it has the wall; otherwise, 0 (as true and false booleans)
 */
int HasWestWall(int index) 
{
    switch(shared_mem[index]){
        case 1: 
            return 1; 
        case 3: 
            return 1; 
        case 5: 
            return 1; 
        case 7: 
            return 1;
        case 9: 
            return 1;
        case 11: 
            return 1;
        case 13: 
            return 1;
        case 15: 
            return 1;
    }
    return 0; 
}


/* =========================================================================== */
/*                              HasSouthWall                                   */
/* =========================================================================== */
/*  Checks whether the current maze cell has a south wall 
 *  
 *  @index: cell of the maze to check
 *
 *  Returns 1 if it has the wall; otherwise, 0 (as true and false booleans)
 */

int HasSouthWall(int index) 
{
    switch(shared_mem[index]){
        case 4: 
            return 1; 
        case 5: 
            return 1; 
        case 6: 
            return 1; 
        case 12: 
            return 1;
        case 7: 
            return 1;
        case 13: 
            return 1;
        case 14: 
            return 1;
        case 15: 
            return 1;
    }
    return 0; 
}

/* =========================================================================== */
/*                               HasEastWall                                   */
/* =========================================================================== */
/*  Checks whether the current maze cell has a east wall 
 *  
 *  @index: cell of the maze to check
 *
 *  Returns 1 if it has the wall; otherwise, 0 (as true and false booleans)
 */

int HasEastWall(int index) 
{
    switch(shared_mem[index]){
        case 8: 
            return 1; 
        case 12: 
            return 1; 
        case 10: 
            return 1; 
        case 9: 
            return 1;
        case 13: 
            return 1;
        case 14: 
            return 1;
        case 11: 
            return 1;
        case 15: 
            return 1;
    }
    return 0; 
}


