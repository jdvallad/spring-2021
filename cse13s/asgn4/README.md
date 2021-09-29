# Assignment 4

This program takes in a user inputted list of cities as well as a graph. The program then finds the
shortest path from the origin vertex that passes through every vertex exactly once and returns back
to the origin vertex.

## Building

    $ make

## Running

    $ ./tsp -PARAMETERS

Acceptable parameters can be from 'hvgui:o:'

-h will display a help message and then return.
-v will set verbose mode, which will print every hamiltonian path, not just the shortest one.
-g will print graph to stdout and wait for user to continue.
-u will make the graph undirected, allowing for travel from one vertex to the other in either direction.
-i FILE will set the input file, with stdin as the default.
-o FILE will ste the output file, with stdout as the default.

## Cleaning

    $ make clean
