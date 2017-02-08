/* Jerry Chen
 * MP7 - Mazes
 * 
 * This file will define the DisjointSets class
 * 
 * Each element has an integer index
 * The up-trees are stored in a single vector of ints
 * The universe of the elements are index 0
 * A nonnegative number is the index of the parent of the
 *		current node, a negative number in a root node is the
 *		negative of the set size
 *	Each index is used as a 'node', the int at the index
 *		holds the representative of that 'node'
 *
 *	The default compiler-supplied Big Three will work flawlessly
 * 	because the only member data is a vector of integers
 *		and this vector should initially be empty
 */

#include "dsets.h"

/* addelements will add amt of elements to the end of the vector
 *		each new element will be unconnected and new root nodes
 * Root size will be initialized to 0, then subtract as more
 *		nodes are added to denote that they are a root
 *	param: int amt
 * return: void
 */
void DisjointSets::addelements(int amt)
{
	int i;
	for(i = 0; i < amt; i++)
	{	// push back amt number of unconnected root nodes
		setSpace.push_back(-1);	// only node is root itself
	}
}

/* find will search for the specified element in parameter
 *	This function will also compress paths meaning
 *		every node in the path will directly point to representative
 *	Best method of implementation is recursive
 * param: int elem
 *	return: 
 */
int DisjointSets::find(int elem)
{	// this sanity check is if elem is valid, only a case
	// in the first call to this function
	if(elem < 0 || elem >= (int)setSpace.size())
		return -1;	// invalid return -1

	// if it's a root
	if(setSpace[elem] < 0)
		return elem;	// pass it to it's children

	// made a mistake, I passed elem which is wrong
	// strange thing is I got a seg fault instead of infinite loop
	int rep = find(setSpace[elem]);	// hold representative
	setSpace[elem] = rep;	// set it equal
	return rep;	// pass on value
}

/*	setunion will join two sets/uptrees, A and B.
 *		The union will be based on size
 *	Should not assume that the arguments to setunion are roots
 *		of existing uptrees
 * param: int setA, int setB, both are indexes of setSpace
 * return: void
 */
void DisjointSets::setunion(int a, int b)
{
	// check for valid params
	int repA = find(a);	// get A's representative
	int repB = find(b);	// get B's rep

	if(repA == -1 || repB == -1)
		return;

	if(repA == repB)	// check if it's in the same subtree
		return;	// very important to prevent cycles

	// smaller size points to bigger one
	// if same size setB points to setA
	// with this comparison I only need an if and else
	if(setSpace[repA] > setSpace[repB])
	{
		setSpace[repB] += setSpace[repA];	// combine size
		setSpace[repA] = repB;	// point to b
	}
	else
	{
		setSpace[repA] += setSpace[repB];
		setSpace[repB] = repA;
	}
}