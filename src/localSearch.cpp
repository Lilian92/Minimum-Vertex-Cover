#include <unordered_set>
#include "localSearch.h"
#include <limits.h>
#include <ctime>
#include <math.h>

void localSearch1(Graph & g, VCTYPE & vc, ofstream & output_trace,
		int cutOffTime, int randSeed) {
	cout << "LocalSearch1" << endl;
	VSet candidate, best_candidate;
	VCTYPE GCandidate;
	srand(randSeed);

	EdgeMap edges, candidate_edges;
	EdgeSet uncovered_edges, double_edges;
	for (size_t ii = 0; ii < g.numberOfVertices; ii++) {
		set<size_t>::iterator it;
		for (it = g.vertices[ii].begin(); it != g.vertices[ii].end(); ++it) {
			std::stringstream edgeID;
			size_t a, b;
			if (ii < *(it)) {
				a = ii;
				b = *(it);
			} else {
				b = ii;
				a = *(it);
			}
			edgeID << a << "||" << b;
			std::string edgeIDS = edgeID.str();
			if (edges.find(edgeIDS) == edges.end()) {
				edges.insert(pair<string, int>(edgeIDS, 1));
			} else {
				edges[edgeIDS]++;
			}
		}
	}

	UncoveredEdges(edges, candidate_edges, uncovered_edges);
	clock_t begin = clock();
	clock_t best;
	g.getOneVerticesCover(GCandidate);
	while (!GCandidate.empty()) {
		size_t temp = GCandidate.top();
		UpdateEdges(g, candidate, candidate_edges, uncovered_edges,
				double_edges, temp, true);
		GCandidate.pop();
	}

	cout << "initial size:" << candidate.size() << endl;
	int minVC = INT_MAX;
	int i = 0;
	while ((double(clock() - begin) / CLOCKS_PER_SEC) < double(cutOffTime)) {
//	while (i>0) {
		i++;
		if (uncovered_edges.empty()) {
			if ((int) candidate.size() < minVC) {
				best = clock();
				best_candidate = candidate;
				minVC = candidate.size();
				output_trace << (double(clock() - begin) / CLOCKS_PER_SEC)
						<< " " << minVC << endl;
			}
			int j;
			if (double_edges.size() < 100) {
				j = double_edges.size();
			} else {
				double loopControl = 0.04 * double_edges.size() + 95.0;
				j = (int) loopControl;
				if (j > 300)
					j = 300;
			}
			size_t best_candidate;
			int cost = INT_MAX;
			set<size_t> neighbours;
			vector<size_t> MinCost;
			bool breakloop = false;

			while (j > 0) {
				j--;
				int random = rand() % double_edges.size();
				EdgeSet::iterator it(double_edges.begin());
				advance(it, random);
				string s = *(it);
				size_t pos = s.find("||");
				std::stringstream ustream(s.substr(0, pos));
				std::stringstream vstream(s.substr(pos + 2));
				size_t u, v;
				ustream >> u;
				vstream >> v;
				if (neighbours.find(v) == neighbours.end()) {
					UpdateEdges(g, candidate, candidate_edges, uncovered_edges,
							double_edges, v, false);
					if ((int) (uncovered_edges.size()) < cost) {
						if (cost != INT_MAX)
							MinCost.push_back(best_candidate);
						cost = uncovered_edges.size();
						best_candidate = v;
					}
					if (cost == 0) {
						breakloop = true;
						break;
					}
					UpdateEdges(g, candidate, candidate_edges, uncovered_edges,
							double_edges, v, true);
					neighbours.insert(v);
				}
				if (neighbours.find(u) == neighbours.end()) {
					UpdateEdges(g, candidate, candidate_edges, uncovered_edges,
							double_edges, u, false);
					if ((int) (uncovered_edges.size()) < cost) {
						if (cost != INT_MAX)
							MinCost.push_back(best_candidate);
						cost = uncovered_edges.size();
						best_candidate = u;
					}
					if (cost == 0) {
						breakloop = true;
						break;
					}
					UpdateEdges(g, candidate, candidate_edges, uncovered_edges,
							double_edges, u, true);
					neighbours.insert(u);
				}
				if (j == 0)
					neighbours.clear();
			}
			if (!breakloop) {
				UpdateEdges(g, candidate, candidate_edges, uncovered_edges,
						double_edges, best_candidate, false);
			}
			if ((rand() % 100) > 90 && MinCost.size() > 2
					&& (double(clock() - begin) / CLOCKS_PER_SEC) < 520) {
				int deletenodes = MinCost.size() / 2;
				for (int i = 0; i < deletenodes; i++) {
					size_t vertex = MinCost[MinCost.size() - (1 + i)];
					UpdateEdges(g, candidate, candidate_edges, uncovered_edges,
							double_edges, vertex, false);
				}
				MinCost.clear();
			}
		} else if (!uncovered_edges.empty()) {
			int j;
			if (uncovered_edges.size() < 50) {
				j = uncovered_edges.size();
			} else {
				j = 50;
			}
			size_t best_candidate = 0;
			int cost = INT_MAX;
			set<size_t> neighbours;
			while (j > 0) {
				j--;
				int random = rand() % uncovered_edges.size();
				EdgeSet::iterator it(uncovered_edges.begin());
				advance(it, random);
				string s = *(it);
				size_t pos = s.find("||");
				std::stringstream ustream(s.substr(0, pos));
				std::stringstream vstream(s.substr(pos + 2));
				size_t u, v;
				ustream >> u;
				vstream >> v;
				if (neighbours.find(u) == neighbours.end()) {
					UpdateEdges(g, candidate, candidate_edges, uncovered_edges,
							double_edges, u, true);
					if ((int) (uncovered_edges.size()) < cost) {
						cost = uncovered_edges.size();
						best_candidate = u;
					}
					UpdateEdges(g, candidate, candidate_edges, uncovered_edges,
							double_edges, u, false);
					neighbours.insert(u);
				}
				if (neighbours.find(v) == neighbours.end()) {
					UpdateEdges(g, candidate, candidate_edges, uncovered_edges,
							double_edges, v, true);
					if ((int) (uncovered_edges.size()) < cost) {
						cost = uncovered_edges.size();
						best_candidate = v;
					}
					UpdateEdges(g, candidate, candidate_edges, uncovered_edges,
							double_edges, v, false);
					neighbours.insert(v);
				}
				if (j == 0)
					neighbours.clear();
			}
			UpdateEdges(g, candidate, candidate_edges, uncovered_edges,
					double_edges, best_candidate, true);
		}
	}
	cout << "current candidate size:" << candidate.size() << " i:" << i << endl;
	cout << "best candidate size:" << best_candidate.size() << " time: "
			<< (double(best - begin) / CLOCKS_PER_SEC) << endl;
	VSet::iterator itrr;
	for (itrr = best_candidate.begin(); itrr != best_candidate.end(); itrr++) {
		vc.push(*(itrr));
	}

}

void localSearch2(Graph & g, VCTYPE & vc, ofstream & output_trace,
		int cutOffTime, int randSeed) {
	cout << "LocalSearch2" << endl;
	VSet candidate, best_candidate;
	VCTYPE GCandidate;
	srand(randSeed);

	EdgeMap edges, candidate_edges;
	EdgeSet uncovered_edges, double_edges;
	for (size_t ii = 0; ii < g.numberOfVertices; ii++) {
		set<size_t>::iterator it;
		for (it = g.vertices[ii].begin(); it != g.vertices[ii].end(); ++it) {
			std::stringstream edgeID;
			size_t a, b;
			if (ii < *(it)) {
				a = ii;
				b = *(it);
			} else {
				b = ii;
				a = *(it);
			}
			edgeID << a << "||" << b;
			std::string edgeIDS = edgeID.str();
			if (edges.find(edgeIDS) == edges.end()) {
				edges.insert(pair<string, int>(edgeIDS, 1));
			} else {
				edges[edgeIDS]++;
			}
		}
	}
	UncoveredEdges(edges, candidate_edges, uncovered_edges);
	clock_t begin = clock();
	clock_t best;
	g.getOneVerticesCover(GCandidate);
	while (!GCandidate.empty()) {
		size_t temp = GCandidate.top();
		UpdateEdges(g, candidate, candidate_edges, uncovered_edges,
				double_edges, temp, true);
		GCandidate.pop();
	}
	cout << "initial size:" << candidate.size() << endl;
	int minVC = INT_MAX;
	int i = 0;
	double k = 0.25, T = 600;
	while ((double(clock() - begin) / CLOCKS_PER_SEC) < double(cutOffTime)) {
		i++;
		T = (double(clock() - begin) / CLOCKS_PER_SEC);
		if (uncovered_edges.empty()) {
			if ((int) (candidate.size()) < minVC) {
				best = clock();
				best_candidate = candidate;
				minVC = candidate.size();
				output_trace << (double(clock() - begin) / CLOCKS_PER_SEC)
						<< " " << minVC << endl;
			}
			int random = rand() % double_edges.size();
			EdgeSet::iterator it(double_edges.begin());
			advance(it, random);
			string s = *(it);
			size_t pos = s.find("||");
			std::stringstream ustream(s.substr(0, pos));
			std::stringstream vstream(s.substr(pos + 2));
			size_t u, v;
			ustream >> u;
			vstream >> v;
			UpdateEdges(g, candidate, candidate_edges, uncovered_edges,
					double_edges, u, false);
			int cost1 = uncovered_edges.size(), cost2;
			double p = 0;
			if (cost1 != 0) {
				UpdateEdges(g, candidate, candidate_edges, uncovered_edges,
						double_edges, u, true);
				UpdateEdges(g, candidate, candidate_edges, uncovered_edges,
						double_edges, v, false);
				cost2 = uncovered_edges.size();
				if (cost2 != 0) {
					if (cost2 <= cost1) {
						p = exp(-(cost2 / (k * T)));
						double pu = (double) rand()
								/ (double) ((unsigned) RAND_MAX + 1);
						if (pu >= p) {
							UpdateEdges(g, candidate, candidate_edges,
									uncovered_edges, double_edges, v, true);
						}
					} else {
						UpdateEdges(g, candidate, candidate_edges,
								uncovered_edges, double_edges, v, true);
						p = exp(-(cost1 / (k * T)));
						double pu = (double) rand()
								/ (double) ((unsigned) RAND_MAX + 1);
						if (pu < p) {
							UpdateEdges(g, candidate, candidate_edges,
									uncovered_edges, double_edges, u, false);
						}
					}
				}
			}
		} else {
			int j;
			if (uncovered_edges.size() < 10) {
				j = uncovered_edges.size();
			} else {
				j = 10;
			}
			size_t best_candidate = 0;
			int cost = INT_MAX;
			set<size_t> neighbours;
			while (j > 0) {
				j--;
				int random = rand() % uncovered_edges.size();
				EdgeSet::iterator it(uncovered_edges.begin());
				advance(it, random);
				string s = *(it);
				size_t pos = s.find("||");
				std::stringstream ustream(s.substr(0, pos));
				std::stringstream vstream(s.substr(pos + 2));
				size_t u, v;
				ustream >> u;
				vstream >> v;
				if (neighbours.find(u) == neighbours.end()) {
					UpdateEdges(g, candidate, candidate_edges, uncovered_edges,
							double_edges, u, true);
					if ((int) (uncovered_edges.size()) < cost) {
						cost = uncovered_edges.size();
						best_candidate = u;
					}
					UpdateEdges(g, candidate, candidate_edges, uncovered_edges,
							double_edges, u, false);
					neighbours.insert(u);
				}
				if (neighbours.find(v) == neighbours.end()) {
					UpdateEdges(g, candidate, candidate_edges, uncovered_edges,
							double_edges, v, true);
					if ((int) (uncovered_edges.size()) < cost) {
						cost = uncovered_edges.size();
						best_candidate = v;
					}
					UpdateEdges(g, candidate, candidate_edges, uncovered_edges,
							double_edges, v, false);
					neighbours.insert(v);
				}
				if (j == 0)
					neighbours.clear();
			}
			UpdateEdges(g, candidate, candidate_edges, uncovered_edges,
					double_edges, best_candidate, true);
		}
	}
	cout << "current candidate size:" << candidate.size() << " i:" << i << endl;
	cout << "best candidate size:" << best_candidate.size() << " time: "
			<< (double(best - begin) / CLOCKS_PER_SEC) << endl;
	VSet::iterator it;
	VSet::iterator itrr;
	for (itrr = best_candidate.begin(); itrr != best_candidate.end(); itrr++) {
		vc.push(*(itrr));
	}
}

void UncoveredEdges(EdgeMap & alledges, EdgeMap & edges,
		EdgeSet & uncoverededges) {
	EdgeMap::iterator it;
	for (it = alledges.begin(); it != alledges.end(); ++it) {
		if (edges.find(it->first) == edges.end()) {
			uncoverededges.insert(it->first);
		}
	}
}

void UpdateEdges(Graph & g, VSet & vc, EdgeMap & edges, EdgeSet & uedges,
		EdgeSet & dedges, size_t vertexID, bool isadded) {
	if (isadded) {
		set<size_t>::iterator it;
		for (it = g.vertices[vertexID].begin();
				it != g.vertices[vertexID].end(); ++it) {
			std::stringstream edgeID;
			size_t a, b;
			if (vertexID < *(it)) {
				a = vertexID;
				b = *(it);
			} else {
				b = vertexID;
				a = *(it);
			}
			edgeID << a << "||" << b;
			std::string edgeIDS = edgeID.str();
			if (edges.find(edgeIDS) == edges.end()) {
				edges.insert(pair<string, int>(edgeIDS, 1));
				uedges.erase(edgeIDS);
			} else {
				if (edges[edgeIDS] == 0)
					uedges.erase(edgeIDS);
				edges[edgeIDS]++;
				if (edges[edgeIDS] == 2)
					dedges.insert(edgeIDS);
			}
			vc.insert(vertexID);
		}
	} else {
		set<size_t>::iterator it;
		for (it = g.vertices[vertexID].begin();
				it != g.vertices[vertexID].end(); ++it) {
			std::stringstream edgeID;
			size_t a, b;
			if (vertexID < *(it)) {
				a = vertexID;
				b = *(it);
			} else {
				b = vertexID;
				a = *(it);
			}
			edgeID << a << "||" << b;
			std::string edgeIDS = edgeID.str();
			if (edges.find(edgeIDS) != edges.end()) {
				if (edges[edgeIDS] == 2)
					dedges.erase(edgeIDS);
				else if (edges[edgeIDS] == 1)
					uedges.insert(edgeIDS);
				edges[edgeIDS]--;
			}
			if (vc.find(vertexID) != vc.end()) {
				vc.erase(vertexID);
			}
		}
	}
}

void printedges(EdgeMap & edges) {
	EdgeMap::iterator it;
	for (it = edges.begin(); it != edges.end(); ++it)
		cout << it->first << ": " << it->second << ' ';
	cout << '\n';
}

void printedgesSet(EdgeSet & edges) {
	EdgeSet::iterator it;
	for (it = edges.begin(); it != edges.end(); ++it)
		std::cout << *(it) << ' ';
	std::cout << '\n';
}
void printVSet(VSet & edges) {
	VSet::iterator it;
	for (it = edges.begin(); it != edges.end(); ++it)
		std::cout << *(it) << ' ';
	std::cout << '\n';
}

