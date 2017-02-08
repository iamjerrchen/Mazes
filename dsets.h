/* Jerry Chen
 * MP7 - Mazes
 *
 * This file will declare the DisjointSets class
 * 	and declare the various member variables and
 *		methods associated with the class.
 * 
 * This class data structure is not templatized
 */

#ifndef DSETS_H
#define DSETS_H

#include <vector>

class DisjointSets
{
	public:
		// technically not necessary cause only have a vector
		DisjointSets() {}	// default constructor, nothing

		// staples to a disjoint set ADT
		void addelements(int amt);	// add an element to the set
		int find(int elem);	// find the element, returns -1 for invalid elem
		void setunion(int a, int b); // join two sets
		// ^ nothing happens for invalid a or b

	private:

		std::vector<int> setSpace;	// vector holding the sets
};

#endif // DSETS_H