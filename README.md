# Mazes

<i>Overview</i>

This program makes use of a disjoint set data structure to generate a perfect maze. The exit of the maze is positioned in a way that maximizes the travel distance from the entrance in any generated maze. There will be two image outputs. One is a blank maze that is unsolved. The other is a maze with the correct solution. I included two 800x800 size mazes in this repository.

For more information about the program, please refer to the documentation in the files.

# Problems

Due to the possible size of the mazes (I'm looking at you 800x800), a recursive function to generate the maze will cause a stackoverflow. There's too many calls to the function. Instead I had to change algorithm to an iterative implementation.
