#include "constructionHeuristics.h"



void constructionHeuristics(Graph &g, VCTYPE &vc, ofstream &output_trace, int cutOffTime, int randSeed, bool write) {
	clock_t start, end;
	char deleted[g.numberOfVertices];
	memset(deleted, 0, g.numberOfVertices * sizeof(char));
    start = clock();
	while (g.numberOfEdges > 0) {
		int uMin = -1;
		set<size_t> :: iterator itr;
		int uMinDegree = g.numberOfVertices;
		for (int i = 0; i < g.numberOfVertices; i++) {
			int uDegree = g.degree(i);
			if (uDegree < uMinDegree && deleted[i] == 0 && uDegree > 0) {
				uMinDegree = uDegree;
				uMin = i;
			}
		}
		set<size_t> neighborhood = set<size_t>(g.vertices[uMin]);
		for (itr = neighborhood.begin(); itr != neighborhood.end(); ++itr) {
			vc.push(*itr);
			g.removeVertex(*itr);
			deleted[*itr] = 1;
		}
		g.removeVertex(uMin);
		deleted[uMin] = 1;
		uMinDegree = g.numberOfVertices;
	}
	end = clock();
	float compTime = (end - start) / (float) CLOCKS_PER_SEC;
	if (write) {
		output_trace << compTime << ", " << vc.size() << endl;
	}
}

