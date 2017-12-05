/*
 * Local Search Algorithm for MVC
 * 1. FIXME
 * */
#include <unordered_map>
#include <map>
#include "util.h"

typedef unordered_map <string,int> EdgeMap;
typedef set<string> EdgeSet;
typedef set<size_t> VSet;

void printedges(EdgeMap & edges);
void printedgesSet(EdgeSet & edges);
void printVSet(VSet & edges);
void UpdateEdges(Graph & g, VSet & vc, EdgeMap & edges, EdgeSet & uedges, EdgeSet & dedges, size_t vertexID, bool isadded);
void UncoveredEdges(EdgeMap & alledges, EdgeMap & edges, EdgeSet & uncoverededges);
void localSearch1(Graph & g, VCTYPE & vc, ofstream & output_trace, int cutOffTime, int randSeed);
void localSearch2(Graph & g, VCTYPE & vc, ofstream & output_trace, int cutOffTime, int randSeed);
