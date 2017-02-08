/* Jerry Chen
 * MP7 - Mazes
 *
 * This file will declare the SquareMaze class
 *		and declare the various member variables and
 *		methods associated with the class.
 */

#ifndef MAZE_H
#define MAZE_H

#include <vector>		// also used like a stack
#include <iostream>	// error messages
#include <stdlib.h>	// srand, rand
#include <time.h>		// time
#include <algorithm>	// random_shuffle
#include <utility>	// gonna use this for pairs of coordinates

using namespace std;

#include "png.h"
#include "dsets.h"	// DisjointSets class

// each cell is a square, the maze itself it's always square
class SquareMaze
{
	private:

		struct cell {

			bool North;	// North Wall
			bool South;	// South Wall
			bool West;	// West Wall
			bool East;  // East Wall

			// initialize cell to all be true
			cell(): North(true), South(true),
					West(true), East(true) {}
		};

	public:

		SquareMaze();

		void makeMaze(int width, int height);	// makes maze with w and h
		bool canTravel(int x, int y, int dir) const;

		void setWall(int x, int y, int dir, bool exists);

		vector<int> solveMaze();	// solves maze

		PNG* drawMaze() const;	// draw without solution
		PNG* drawMazeWithSolution();

	private:

		// using vectors 2d matrix
		// Walldir being bool takes up less memory, I only need 0 or 1
		vector< vector<cell> > mazeWall;	// whether or not to wall
		vector< vector<bool> > visited;	// whether or not it's traveled yet
										// if true then don't traverse
		vector<int> solution;	// initialized to false
										// used in solve maze only, and draw
		vector<int> possible;	// possible solution, find longest path

		vector< vector<int> > randDir;	// vector<int> will hold size 4 ways	
		vector< pair<int, int> > coords;	// treat like a queue I guess

		DisjointSets blocks;	// prevent cycles in maze

		int mWidth;	// width of maze
		int mHeight;	// height of maze
		int totalBlocks;	// width*height # of blocks in grid;
		int endCellX;	// end block, could use pairs, but I like this better
		int endCellY;

		// Helper Functions here
		void iterative(int StartX, int StartY, int &count);
		void Continue(int newx, int newy, int &curridx);
		bool carveHelper(int x, int y, int adjx, int adjy, int &size, int dir);
		void rearrange(vector<int> &ways);	// randomize given vector

		void solveMazeHelper();	// does the solving
		// add to "stack"
		void SolvePush(int newX, int newY, int dirs, int &vectidx);
		// remove from "stack"
		void popOffDirs(int &dirs, int &vectidx);
		void PickDirections(int x, int y);

		// makes code cleaner for coloring walls
		void paintRight(int x, int y, PNG *&canvas, RGBAPixel &Color) const;
		void paintDown(int x, int y, PNG *&canvas, RGBAPixel &Color) const;
};

#endif