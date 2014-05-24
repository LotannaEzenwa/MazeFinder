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
#include "dstarlite.h"
// ---------------- Constants

// ---------------- Structures/Types

// ---------------- Public Variables

// ---------------- Prototypes/Macros
MazeNode *** parselog(uint32_t mazeport,uint32_t mazewidth, uint32_t mazeheight);
void update(MazeNode ***array,uint32_t mazewidth, uint32_t mazeheight, AM_Message msg, int nAvatars);

#endif // MAZE.H

