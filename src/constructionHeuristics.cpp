

#include "constructionHeuristics.h"
#include <iterator>
/*void constructionHeuristics(Graph &g, VCTYPE &vc, ofstream &output_trace, int cutOffTime, int randSeed) {
	while (g.numberOfEdges > 0) {
		set<size_t> :: iterator itr;
		int uMaxDegree = -1, vMaxDegree = -1, uMax = -1, vMax = -1;
		for (int i = 0; i < g.numberOfVertices; i++) {
			int uDegree = g.degree(i);
			if (uDegree > uMaxDegree && !g.isDeleted(i)) {
				uMaxDegree = uDegree;
				uMax = i;
			}
		}
		set<size_t> neighborhood = set<size_t>(g.vertices[uMax]);
		for (itr = neighborhood.begin(); itr != neighborhood.end(); ++itr) {
			int vDegree = g.degree(*itr);
			if (vDegree > vMaxDegree && !g.isDeleted(*itr)) {
				vMaxDegree = vDegree;
				vMax = *itr;
			}
		}
		Edge uv = Edge(uMax, vMax);
		if (vMax == 1) {
			vc.push(uv.v1);
		} else {
			vc.push(uv.v1);
			vc.push(uv.v2);
		}
		g.removeVertex(uv.v1);
		g.removeVertex(uv.v2);
	}
}*/
void constructionHeuristics(Graph &g, VCTYPE &vc, ofstream &output_trace, int cutOffTime, int randSeed) {
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
}

/*void constructionHeuristics(Graph &g, VCTYPE &vc, ofstream &output_trace, int cutOffTime, int randSeed) {
	srand(randSeed);
	size_t root = 0;
	g.findVertexWithBiggestDegree(root);
	cout << "Root: " << root << endl;
	Graph tree = getSpanningTree(g, root);
	bool rootAdded = false;
	for (int i = 0; i < tree.numberOfVertices; i++) {
		if (tree.degree(i) > 1) {
			if (i == root) {
				rootAdded = true;
			}
			vc.push(i);
		}
	}
	if (!rootAdded) {
		vc.push(root);
	}
}

Graph getSpanningTree(Graph &g, size_t startVertexID) {
	Graph t = Graph(g.numberOfVertices);
	dfs(g, &t, startVertexID);
	return t;
}

void dfs(Graph &g, Graph *t, size_t vertexID) {
	set<size_t> neighbors = set<size_t>(g.vertices[vertexID]);
	set<size_t> :: iterator itr;
	for (itr = neighbors.begin(); itr != neighbors.end(); ++itr) {
		if (t->degree(*itr) == 0) {
			t->addEdge(Edge(vertexID, *itr));
			dfs(g, t, *itr);
		}
	}
}*/

