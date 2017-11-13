#include "constructionHeuristics.h"
#include <iterator>

void constructionHeuristics(Graph &g, VCTYPE &vc, ofstream &output_trace, int cutOffTime, int randSeed) {
	int a = cutOffTime;
	int b = randSeed;
	while (g.numberOfEdges > 0) {
		Edge uv;
		for (int i = 0; i < g.numberOfVertices; i++) {
			if (g.vertices[i].size() > 0) {
				uv = Edge(i, *g.vertices[i].begin());
				break;
			}
		}
		cout << "uv = " << uv.v1 << ", " << uv.v2 << "\n";
		vc.push(uv.v1);
		vc.push(uv.v2);
		set<size_t> adjVertices = g.vertices[uv.v1];
		set<size_t> :: iterator itr;
		for (itr = adjVertices.begin(); itr != adjVertices.end(); ++itr) {
			cout << "e = " << uv.v1 << ", " << *itr << "\n";
			g.removeEdge(Edge(uv.v1, *itr));
		}
		g.removeVertex(uv.v1);
		g.removeVertex(uv.v2);
	}
}
