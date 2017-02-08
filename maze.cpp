/* Jerry Chen
 * MP7 - Mazes
 *
 * This file will define the SquareMaze class
 *	This class can create a random maze with no cycles
 * 	The maze can be solved in the solveMaze function, then
 *		there's two print functions that output the maze onto
 *		a PNG. One of them prints without the solution. The other
 *		prints with a solution.
 */

#include "maze.h"

// I'm going to create some macros so it's easier to understand code
#define RIGHT	0	// +1 in x coord 	// 0 = horizontal wall
#define DOWN	1	// +1 in y coord 	// 1 = vertical wall
#define LEFT	2	// -1 in x coord
#define UP		3	// -1 in y coord

/* Constructor creates an empty maze
 * Initialize all member variables
 * So there's no strange errors
 */
SquareMaze::SquareMaze() : blocks(DisjointSets()), mWidth(0), mHeight(0),
		totalBlocks(0)
{
	srand(time(NULL));	// generate random, "initialized"
}

/* makeMaze will make a new SquareMaze of given height and width
 *		This function will overwrite the current maze first.
 *		Start with a square grid, graph paper, Select random walls to
 *		delete that won't create a cycle. Don't delete walls on perimeter
 * param: (int)width = width of maze, (int)height = height of maze
 * return: void
 *
 *	I tried to do this recursively, this won't work for mazes bigger than
 *		280 or so, so I changed to iteratively
 */
void SquareMaze::makeMaze(int width, int height)
{
	if(width < 2 || height < 2)	// maze too small
	{
		cout << "Invalid maze dimensions." << endl;
		return;
	}

	// initialize members
	mWidth = width;
	mHeight = height;
	totalBlocks = width*height;

	// initialize all the vectors with associcated use
	mazeWall.resize(height, vector<cell>(width, cell()));	// [y][x]
	visited.resize(height, vector<bool>(width, false));	// not traveled
	solution.resize(0);	// empty the vector, new maze new solution
	possible.resize(0);	// same as solution

	blocks.addelements(totalBlocks);	// that many cells
	int currentidx = rand()%(totalBlocks);	// starting block

	// pick a random cell to start
	int Xidx, Yidx;
	Yidx = currentidx/width;	// y
	Xidx = (currentidx-(Yidx*width));	// x

	// carve grid
	int count = totalBlocks;	// down counter for forest

	iterative(Xidx, Yidx, count);	// create iteratively

	// create openings for start and end
	(mazeWall[0][0]).North = false;	// remove North wall @(0,0) to create an entrance
	// endCellX = (rand()%(mWidth-1))+1;
	// endCellY = mHeight-1;
}

void SquareMaze::iterative(int StartX, int StartY, int &count)
{
	//each time I push back increment
	int currentidx = -1; // increment and it will have last element

	// initialize the start cell, similar to stack/queue lab
	Continue(StartX, StartY, currentidx);
	int direction;	// ill pop from ways
	int x, y;
	// if randDir or coords is pushed, gotta push the other vector too
	// currentidx will need to be incremented
	while(count > 1)
	{	// empty direction vector
		// originally had if statement, realized I need loop
		// in case previous iterations are empty too
		while((randDir[currentidx]).empty())
		{
			randDir.pop_back();	// all directions done
			coords.pop_back();	// coordinate done
			currentidx--;
		}

		// get current info for coordinate and direction from it
		x = (coords[currentidx]).first;
		y = (coords[currentidx]).second;

		direction = (randDir[currentidx]).back();
		(randDir[currentidx]).pop_back();
		visited[y][x] = true;

		// replicate the recursive function
		if(direction == RIGHT)
		{	// if direction can be traveled
			if(carveHelper(x, y, x+1, y, count, RIGHT))
				Continue(x+1, y, currentidx);	// push the new cell
		}
		else if(direction == DOWN)
		{
			if(carveHelper(x, y, x, y+1, count, DOWN))
				Continue(x, y+1, currentidx);
		}
		else if(direction == LEFT)
		{
			if(carveHelper(x, y, x-1, y,count, LEFT))
				Continue(x-1, y, currentidx);
		}
		else	// it's up
		{
			if(carveHelper(x, y, x, y-1, count, UP))
				Continue(x, y-1, currentidx);
		}
	}
}	// done

/*	pushback new coordinates and new directions for me
 *		makes original function cleaner
 */
void SquareMaze::Continue(int newx, int newy, int &curridx)
{
	vector<int> ways;
	rearrange(ways);	// generate new random directions

	randDir.push_back(ways);
	coords.push_back(pair<int, int>(newx, newy));
	curridx++;
}	// done

/* Does the actual carving for carve, makes the code cleaner cause
 *		this needs to be done 4 different times
 *	Returns whether the direction is valid
 */
bool SquareMaze::carveHelper(int x, int y, int adjx, int adjy, int &size,
				int dir)
{	// out of bounds, or already visited
	if(adjx < 0 || adjx >= mWidth || adjy < 0 || adjy >= mHeight)
		return false;

	if(visited[adjy][adjx])	// need to be separate otherwise segfault
		return false;

	int currentidx = (y*mWidth)+x;	// calculation from 220
	int adjacentidx = (adjy*mWidth)+adjx;

	// find if they are already connected cells
	int u = blocks.find(currentidx);
	int v = blocks.find(adjacentidx);

	if(u != v)
	{
		blocks.setunion(u, v);
		setWall(x, y, dir, false);	// remove the wall
		size--;
		return true;
	}

	return false;	// dont go that way, already connected
}	// done

/*	Helper function to randomize the different directions
 *		this way I can just do a for loop through the vector
 *		Then I can just call carve every single time
 *		code is ugly, but it does the job better than random_shuffle
 */
void SquareMaze::rearrange(vector<int> &ways)
{
	// randomizing 0, 1, 2, 3
	ways.resize(4, 0);

	int i;
	for(i = 1; i < 4; i++)
		ways[i] = i;	// initialize with 0, 1, 2, 3

	random_shuffle(ways.begin(), ways.end());	// shuffle it
}	// done

/* This uses your representation of the maze to determine whether
 *	it is possible to travel in the given direction from the square
 *	at coordinates (x,y)
 *
 * For example, after makeMaze(2,2), the possible input coordinates will
 *	be (0,0), (0,1), (1,0), and (1,1)
 *
 */
bool SquareMaze::canTravel(int x, int y, int dir) const
{
	if(x < 0 || x >= mWidth || y < 0 || y >= mHeight)
	{
		cout << "Invalid coordinates." << endl;
		return false;
	}

	// no wall means false, so !false = can travel
	switch(dir)
	{
		case RIGHT:
			return !((mazeWall[y][x]).East);

		case DOWN:
			return !((mazeWall[y][x]).South);

		case LEFT:
			return !((mazeWall[y][x]).West);

		case UP:
			return !((mazeWall[y][x]).North);

		default:
			cout << "Invalid direction." << endl;
			return false;
	}
}	// done

/* setWall sets the members to whether or not the wall exists
 *		and which direction the wall is facing. Only need to worry about
 * 	setting right or down walls, but I implemented up and left 
 *		to make makeMaze easier.
 *	Constant time
 * param: x - xcoord, y - ycoord, dir - 0, 1, 2, or 3, exists - wall?
 * return: void
 */
void SquareMaze::setWall(int x, int y, int dir, bool exists)
{
	if(x < 0 || x >= mWidth || y < 0 || y >= mHeight)	// invalid x or y
	{
		cout << "Invalid coordinates." << endl;
		return;
	}

	if(dir == RIGHT)	// right wall
	{
		if(x == mWidth-1)	// don't do anything
			return;

		(mazeWall[y][x]).East = exists;	// right wall
		(mazeWall[y][x+1]).West = exists;
	}
	else if(dir == DOWN)
	{
		if(y == mHeight-1)	// check border cell
			return;

		(mazeWall[y][x]).South = exists;	// down wall
		(mazeWall[y+1][x]).North = exists;
	}
	else if(dir == LEFT)
	{
		if(x == 0)	// check if border cell
			return;

		(mazeWall[y][x]).West = exists;	// left wall
		(mazeWall[y][x-1]).East = exists;
	}
	else if(dir == UP)
	{
		if(y == 0)
			return;

		(mazeWall[y][x]).North = exists;	// up wall
		(mazeWall[y-1][x]).South = exists;
	}
	else
	{
		cout << "Invalid direction, rethink life." << endl;
	}
}	// done

/*	solveMaze will take the created maze and find the shortest
 *		path from start to finish, the solution will be a vector of
 *		directions. 
 *	If the dimensions are 0 then it will print Maze doesn't exist
 *	If solution is not empty, then makeMaze hasn't been called a second time
 *		and the solution hasn't changed.
 *	If Dimensions will never be less than 0
 *	parameters: none
 *	return: a vector of directions based on previous encodings
 */
vector<int> SquareMaze::solveMaze()
{
	if(mWidth == 0 || mHeight == 0)
	{
		cout << "Maze doesn't exists." << endl;
		return vector<int>();
	}

	if(!solution.empty())
		return solution;	// solution hasn't changed

	// just reusing, not actually random directions
	randDir.resize(0, vector<int>());	// initialize to 0
	coords.resize(0, pair<int,int>());	// same process as make

	// prevent revisiting
	visited.resize(0, vector<bool>());	// delete everything then resize
	// otherwise the following will copy data over
	visited.resize(mHeight, vector<bool>(mWidth, false));

	// push randDir's initial calculations
	(mazeWall[0][0]).North = true;
	PickDirections(0, 0);	// pushes onto randDir
	(mazeWall[0][0]).North = false;

	coords.push_back(pair<int,int>(0,0));

	// there's only three vectors to manage
	solveMazeHelper();	// do the solving

	return solution;
}

/*	Solves the maze iteratively
 *	Put in another function because it looks cleaner
 *	Very similar to makeMaze
 *
 */
void SquareMaze::solveMazeHelper()
{	// prevent revisiting

	int currentidx = 0;	// already initialized, vectors have something
	int x = 0;
	int y = 0;
	int bottom_cell = 0;
	endCellX = 0;
	endCellY = mHeight-1;	// always

	int dirs;
	// find the solution
	while(1)
	{	
		dirs = -1;	// reset the direction
		popOffDirs(dirs, currentidx);	// retrieve possible path

		// coordinates for current direction
		x = (coords[currentidx]).first;	
		y = (coords[currentidx]).second;
		// mark current cell as visited
		visited[y][x] = true;	

		// "traverse" and push new cell with costs
		if(dirs == RIGHT)
			// no wall and not visited, push on everything
			SolvePush(x+1, y, RIGHT, currentidx);
		else if(dirs == DOWN)
			SolvePush(x, y+1, DOWN, currentidx);
		else if(dirs == LEFT)
			SolvePush(x-1, y, LEFT, currentidx);
		else	// must be UP
			SolvePush(x, y-1, UP, currentidx);

		// Am I at the bottom?
		if((coords[currentidx]).second == endCellY)
		{	// copy is kinda expensive, but trying to figure out a better way
			bottom_cell++;	// gonna explore every single bottom cell
			if(solution.size() == possible.size()
					&& (coords[currentidx]).first < endCellX)	// if there's a longer path
			{	// use the one with smaller X
				solution = possible;	// save the better one
				endCellX = (coords[currentidx]).first;
			}
			else if(solution.size() < possible.size())
			{
				solution = possible;
				endCellX = (coords[currentidx]).first;
			}

			if(bottom_cell == mWidth)
				return;	// visited all of bottom
		}
	}
}

// Does pushing for SolveMaze
void SquareMaze::SolvePush(int newX, int newY, int dirs, int &vectidx)
{
	possible.push_back(dirs);	// push direction
	// push new cell after direction
	coords.push_back(pair<int,int>(newX, newY));
	// calculate costs for new cell
	PickDirections(newX, newY);
	vectidx++;	// increment, makes access easier to read
}

/*	Clears deadends if there's no where else to go from cell
 */
void SquareMaze::popOffDirs(int &dirs, int &vectidx)
{
	// this while loop checks for deadends
	int i;
	while(dirs == -1)	// if dirs == -1, then current vect is empty
	{	// use indexes as directions, costs are the elements
		for(i = 0; i < 4; i++)
		{	// check if -1, if not dirs = i
			if(randDir[vectidx][i] != -1)
			{
				if(dirs == -1)	// if we don't have a direction yet
				{
					dirs = i;
					randDir[vectidx][dirs] = -1;
					return;
				}
			}
		}

		// start popping off, found dead-end
		randDir.pop_back();
		coords.pop_back();
		possible.pop_back();	// probably shouldn't
								// have forgotten this
		vectidx--;
	}
}

/*	Uses manhattan heuristic to calculate best path
 *		if it doesn't work, it places next best
 *	code taken from my 220 code, partially
 *		A little expensive, but I'm trying to finish
 *		Before thinking of a better algorithm
 *		Too much to change if I changed the name, not calculating anymore
 */
void SquareMaze::PickDirections(int x, int y)
{
	int right = -1;	// if -1 means can't travel
	int down = -1;
	int left = -1;
	int up = -1;
	vector<int> cost;	// how much each direction costs
	cost.resize(4, -1);

	// calculate costs for each direction if valid
	if(!((mazeWall[y][x]).East) && !(visited[y][x+1]))	// if can travel in direction
		cost[0] = 1;

	if(!((mazeWall[y][x]).South) && !(visited[y+1][x]))
		cost[1] = 1;

	if(!((mazeWall[y][x]).West) && !(visited[y][x-1]))
		cost[2] = 1;

	if(!((mazeWall[y][x]).North) && !(visited[y-1][x]))
		cost[3] = 1;

	randDir.push_back(cost);	// index represents direction
}

/*	Draws the maze onto a PNG without the solution
 *		Color top and left border, then go through and color right
 *		and bottom walls
 * param: none
 * return: A pointer to a PNG without a solution
 */
PNG* SquareMaze::drawMaze() const
{
	PNG *draw;

	if(mWidth == 0 || mHeight == 0)
	{
		cout << "Create a maze first." << endl;
		draw = new PNG();
		return draw;
	}
	// I might need to create a PNG member
	// There's no PNG param, so if I create one here, I will lose it
	int maxX = (mWidth*10)+1;	// maximum image's x pixels
	int maxY = (mHeight*10)+1;	// y

	draw = new PNG(maxX, maxY);	// initialize white canvas

	RGBAPixel Black(0,0,0);

	// color the top border
	int x, y;	// axis counters, black is 0 across rgb
	for(x = 10; x < maxX; x++)	// remember not inclusive
		*(*draw)(x,0) = Black;	// gotta dereference the pointer

	// color left border
	for(y = 0; y < maxY; y++)
		*(*draw)(0,y) = Black;	// before I can use operator overload

	// traverse mazeWall, be calling can travel
	for(y = 0; y < mHeight; y++)
	{
		for(x = 0; x < mWidth; x++)
		{
			if((mazeWall[y][x]).East)
				paintRight(x, y, draw, Black);

			if((mazeWall[y][x]).South)
				paintDown(x, y, draw, Black);
		}
	}

	return draw;
}	// done

/* Private helpers that will paint a right or down wall for me
 *		Right walls are vertical
 *		Down walls are horizontal
 */
void SquareMaze::paintRight(int x, int y, PNG *&canvas, RGBAPixel &Color) const
{
	int k;
	for(k = 0; k <= 10; k++)
	{
		*(*canvas)((x+1)*10, (y*10)+k) = Color;
	}
}	// done

void SquareMaze::paintDown(int x, int y, PNG *&canvas, RGBAPixel &Color) const
{
	int k;
	for(k = 0; k <= 10; k++)
	{
		*(*canvas)((x*10)+k, (y+1)*10) = Color;
	}
}	// done

/* Draws the maze onto a PNG with the solution
 *	param: none
 *	return: a pointer to a PNG with the solution
 */
PNG* SquareMaze::drawMazeWithSolution()
{
	PNG *draw = drawMaze();	// get template

	if(mWidth == 0 && mHeight == 0)
	{
		cout << "Can't have a solution without a maze." << endl;
		return draw;
	}

	RGBAPixel Red(255,0,0);
	RGBAPixel White(255,255,255);

	// start at (5,5)
	int x = 5;
	int y = 5;

	// loop through instructions
	int instructSize = solution.size();
	int i, count;

	for(i = 0; i < instructSize; i++)
	{	// modify x and y along the way, which is simpler
		if(solution[i] == RIGHT)	// if it's RIGHT
		{	// increment x
			for(count = 0; count < 10; count++)
				*(*draw)(x+count, y) = Red;
			
			x += 10;
		}
		else if(solution[i] == DOWN)
		{
			for(count = 0; count < 10; count++)
				*(*draw)(x, y+count) = Red;
			
			y += 10;	// increment y goes down
		}
		else if(solution[i] == LEFT)
		{
			for(count = 0; count < 10; count++)
				*(*draw)(x-count, y) = Red;
			
			x -= 10;
		}
		else	// has to be UP
		{
			for(count = 0; count < 10; count++)
				*(*draw)(x, y-count) = Red;
				
			y -= 10;
		}
	}

	*(*draw)(x, y) = Red;	// mark the last 

	// create an exit, use white paint
	for(i = 1; i <= 9; i++)
		*(*draw)((endCellX*10)+i, (endCellY+1)*10) = White;

	return draw;
}	// done