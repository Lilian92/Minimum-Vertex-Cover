/*
 * Construction Heuristic for MVC 
 * Finds the vertex of minimum degree
 * Adds all neighbors to vertex cover
 * Removes incident edges for every neighbor from graph
 * Repeat until all edges have been removed
*/
#include "util.h"
void constructionHeuristics(Graph & g, VCTYPE & vc, ofstream & output_trace, int cutOffTime, int randSeed, bool write);