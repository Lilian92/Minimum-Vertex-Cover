/*
 * Approximation algorithm for MVC  
 * 
 * This algorithm compute an approximate solution to the MVC problem by repeating the following steps
 * until all edges have been removed from the input graph:
 *	1. Find the vertex of highest degree in the graph, call it u
 *	2. Find the vertex of highest degree adjacent to u, call it v
 *  3. Remove u and v
 * This represents the 2-approximation for MVC that selects an arbitrary edge and covers both vertices, removing all
 * edges incident to these vertices until all edges have been removed. It is modified to prioritize by vertex degree.
 * */

#include "constructionHeuristics.h"
#include <iterator>

void constructionHeuristics(Graph &g, VCTYPE &vc, ofstream &output_trace, int cutOffTime, int randSeed) {
	while (g.numberOfEdges > 0) {
		set<size_t> :: iterator itr;
		int uMaxDegree = -1, vMaxDegree = -1, uMax = -1, vMax = -1;
		for (int i = 0; i < g.numberOfVertices; i++) {
			int uDegree = g.vertices[i].size();
			if (uDegree > uMaxDegree) {
				uMaxDegree = uDegree;
				uMax = i;
			}
		}
		set<size_t> vCandidates = g.vertices[uMax];
		for (itr = vCandidates.begin(); itr != vCandidates.end(); ++itr) {
			int vDegree = g.vertices[*itr].size();
			if (vDegree > vMaxDegree) {
				vMaxDegree = vDegree;
				vMax = *itr;
			}
		}
		Edge uv = Edge(uMax, vMax);
		vc.push(uv.v1);
		g.removeVertex(uv.v1);

		vc.push(uv.v2);
		g.removeVertex(uv.v2);
	}
}
