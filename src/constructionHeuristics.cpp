

#include "constructionHeuristics.h"
#include <iterator>
void constructionHeuristics(Graph &g, VCTYPE &vc, ofstream &output_trace, int cutOffTime, int randSeed) {
	clock_t start, end;
    start = clock();
	while (g.numberOfEdges > 0) {
		set<size_t> :: iterator itr;
		int uMinDegree = g.numberOfVertices, uMin = -1;
		for (int i = 0; i < g.numberOfVertices; i++) {
			int uDegree = g.degree(i);
			if (uDegree < uMinDegree && !g.isDeleted(i)) {
				uMinDegree = uDegree;
				uMin = i;
			}
		}
		set<size_t> neighborhood = set<size_t>(g.vertices[uMin]);
		for (itr = neighborhood.begin(); itr != neighborhood.end(); ++itr) {
			vc.push(*itr);
			g.removeVertex(*itr);
		}
		g.removeVertex(uMin);
	}
	end = clock();
	float compTime = (end - start) / (float) CLOCKS_PER_SEC;
	output_trace << compTime << ", " << vc.size() << endl;
}

