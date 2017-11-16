/*
 * Construction Heuristics Algorithm for MVC
 * 1. FIXME
 * */
#include "util.h"

void constructionHeuristics(Graph & g, VCTYPE & vc, ofstream & output_trace, int cutOffTime, int randSeed);
void calculateVertexCover(Graph &g, VCTYPE &vc, int randSeed);

Graph getSpanningTree(Graph &g, size_t startVertexID);
void dfs(Graph &g, Graph *t, size_t vertexID);