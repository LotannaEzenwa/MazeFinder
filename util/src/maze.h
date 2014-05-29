/* ========================================================================== */
/* File: maze.h
 *
 * Project name: Amazing Project
 * Component name: Graphics
 *
 * This file contains the definitions for maze graphics.
 *
 */
/* ========================================================================== */
#ifndef MAZE_H
#define MAZE_H

// ---------------- Prerequisites e.g., Requires "math.h"
#include "amazing.h"
// ---------------- Constants

// ---------------- Structures/Types
typedef enum {P, W} walls;

typedef struct MazeCell {
	XYPos position;
	walls north;
	walls south;
	walls east;
	walls west;
	int maze_boolean;
} MazeCell;

// ---------------- Public Variables

// ---------------- Prototypes/Macros
void getlog(uint32_t mazeport);
MazeCell *** parselog(uint32_t mazewidth, uint32_t mazeheight);
void update(MazeCell ***array,uint32_t mazewidth, uint32_t mazeheight, AM_Message msg, int nAvatars);
void freeMaze(MazeCell ***array, uint32_t width, uint32_t height);

#endif // MAZE.H

