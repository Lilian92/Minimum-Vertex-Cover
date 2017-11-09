/*
CSE6140 HW1
This is an example of how your experiments should look like.
Feel free to use and modify the code below, or write your own experimental code, as long as it produces the desired output.
*/

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include <queue>
#include <set>
#include <stack>

using namespace std;

struct Edge {
    int v1;
    int v2;
    int weight;
    
    Edge( int a=0, int b=0, int w=0):
    v1(a), v2(b), weight(w){}
    
    //To get the minimum from priority tree, the operation's result is in contrast.
    friend bool operator<(Edge const &x, Edge const &y) { return x.weight > y.weight; }
};

struct ToVertex {
    int to;
    int * weight;
    
    ToVertex( int a=0, int * w= new int(0)):
    to(a), weight(w) {}
    
    //To get the minimum from priority tree, the operation's result is in contrast.
    friend bool operator<(ToVertex const &x, ToVertex const &y) { return x.to < y.to; }
};

struct Graph {
    int numberOfVertices;
    int numberOfEdges;
    set<ToVertex> * vercticesEdges;
    int totalWeight;

    Graph(int a, int b, int _totalWeight=0) :
    numberOfVertices(a), numberOfEdges(b), totalWeight(_totalWeight) {
        vercticesEdges = new set<ToVertex>[numberOfVertices];
        for(int i=0; i<numberOfVertices; i++) {
            (vercticesEdges)[i] = set<ToVertex>();
        }
    }
    
    bool updataEdgeWeight(Edge const & e) {
        std::set<ToVertex>::iterator it = (vercticesEdges)[e.v1].find(ToVertex(e.v2, new int(e.weight)));    
        if(it != (vercticesEdges)[e.v1].end()) {
            if(*(*it).weight > e.weight) {
                totalWeight -= (*(*it).weight - e.weight);
                *(*it).weight = e.weight;
                it = (vercticesEdges)[e.v2].find(ToVertex(e.v1, new int(e.weight)));
                *(*it).weight = e.weight;
            }
            return true;
        }
        return false;
    }
    
    void removeEdge(Edge e) {
        (vercticesEdges[e.v1]).erase(ToVertex(e.v2, new int(0)));
        (vercticesEdges[e.v2]).erase(ToVertex(e.v1, new int(0)));
        totalWeight -= e.weight;
    }

    void addEdge(Edge e) {
        (vercticesEdges[e.v1]).insert(ToVertex(e.v2, new int(e.weight)));
        (vercticesEdges[e.v2]).insert(ToVertex(e.v1, new int(e.weight)));
        totalWeight += e.weight;
    }
};

void insertToVertexToSet(set<ToVertex> & s, ToVertex a) {
    std::pair<std::set<ToVertex>::iterator, bool> ret;
    
    ret = s.insert(a);
    if(ret.second==false) {
        if((*(*(ret.first)).weight) > (*a.weight))
            *(*(ret.first)).weight = *a.weight;
    }
}

Graph parseEdges(string graph_file) {
    ifstream inputfile(graph_file);
    
    //init graph
    int numberOfVertices, numberOfEdges;
    inputfile >> numberOfVertices;
    inputfile >> numberOfEdges;
    Graph g(numberOfVertices, numberOfEdges);

    //input edges' info
    int count = g.numberOfEdges;
    Edge e1, e2;
    if(count > 0) {
        inputfile >> e1.v1;
        inputfile >> e1.v2;
        inputfile >> e1.weight;
        count--;
    }
    
    while(count) {
        {
            inputfile >> e2.v1;
            inputfile >> e2.v2;
            inputfile >> e2.weight;
            count--;
            while(count && ( ((e1.v1 == e2.v1) && (e1.v2 == e2.v2)) 
            || ((e1.v1 == e2.v2) && (e1.v2 == e2.v1)) )) {
                if(e1.weight > e2.weight)
                    e1.weight = e2.weight;
                    
                inputfile >> e2.v1;
                inputfile >> e2.v2;
                inputfile >> e2.weight;
                count--;
            }
        }
        
        if(e1.v1 != e1.v2) {
            insertToVertexToSet((g.vercticesEdges)[e1.v1], ToVertex(e1.v2, new int(e1.weight)));
            insertToVertexToSet((g.vercticesEdges)[e1.v2], ToVertex(e1.v1, new int(e1.weight)));
        }
        
        e1 = e2;
    }
    if(e1.v1 != e1.v2) {
        insertToVertexToSet((g.vercticesEdges)[e1.v1], ToVertex(e1.v2, new int(e1.weight)));
        insertToVertexToSet((g.vercticesEdges)[e1.v2], ToVertex(e1.v1, new int(e1.weight)));
    }

    inputfile.close();
    return g;
}

void outputGraph(ofstream & output, Graph & g) {
    output << g.numberOfVertices << endl;
    set<ToVertex>::iterator it;
    for(int i=0; i<g.numberOfVertices; i++) {
        for (it=((g.vercticesEdges)[i]).begin(); it!=((g.vercticesEdges)[i]).end(); ++it) {
            Edge edg(i, (*it).to, *((*it).weight));
            output << edg.v1 << "\t";
            output << edg.v2 << "\t";
            output << edg.weight << endl;
        }
    }
}

void addSetToPQAndClear(priority_queue<Edge> & pq, set<ToVertex> & myset, int id) {
    std::set<ToVertex>::iterator it;
    for (it=myset.begin(); it!=myset.end(); ++it) {
        Edge edg(id, (*it).to, *((*it).weight));
        pq.push(edg);
    }
    myset.clear();
}

int computeMST(Graph & g) {
    priority_queue<Edge> prioEdges;
    Graph mst(g.numberOfVertices, g.numberOfVertices - 1);
    
    bool inT[g.numberOfVertices];
    for(int i=0; i<g.numberOfVertices; i++)
        inT[i] = false;
    
    inT[0] = true;
    addSetToPQAndClear(prioEdges, (g.vercticesEdges)[0], 0);
    
    int count = 1;
    while(count < g.numberOfVertices) {
        Edge minEdgeToOutOfT = prioEdges.top();
        prioEdges.pop();
        
        int v1 = minEdgeToOutOfT.v1;
        int v2 = minEdgeToOutOfT.v2;
        if((inT[v1] != true) || (inT[v2] != true)) {
            int newInT = (inT[v1] != true) ? v1 : v2;
                
            inT[newInT] = true;
            addSetToPQAndClear(prioEdges, (g.vercticesEdges)[newInT], newInT);
            
            mst.addEdge(minEdgeToOutOfT);
            count ++;
        }
    }
    
    g = mst;
    return mst.totalWeight;
}

void dfs_findCycle(stack<ToVertex> & cycle, Graph & g, int startVertexID, int endVertexID) {
    bool * visited = new bool[g.numberOfVertices];
    for(int i=0; i<g.numberOfVertices; i++)
        visited[i] = false;
        
    cycle.push(ToVertex(startVertexID, new int(0)));
    visited[startVertexID] = true;
    while(cycle.size() != 0) {
        ToVertex visitVertex = cycle.top();
        int vertexID = visitVertex.to;
        std::set<ToVertex>::iterator child;
        for(child = ((g.vercticesEdges)[vertexID]).begin(); child != ((g.vercticesEdges)[vertexID]).end(); child++) {
            int childVertexID = (*child).to;
            if( !visited[childVertexID] ) {
                visited[childVertexID] = true;
                cycle.push(*child);
                if(childVertexID == endVertexID)
                    return ;
                break;
            }
        }
        if(child == ((g.vercticesEdges)[vertexID]).end()) {
            cycle.pop();
        }
    }
    
    return;
}

int recomputeMST(int u, int v, int weight, Graph & g) {
    if(weight > g.totalWeight)
        return g.totalWeight;

    Edge added(u, v, weight);
    
    if(g.updataEdgeWeight(added))
        return g.totalWeight;
    
    stack<ToVertex> cycle;
    dfs_findCycle(cycle, g, u, v);
    
    Edge e_Max;
    e_Max.weight = weight;
	while(cycle.size() > 1) {
	    ToVertex ch = cycle.top();
	    cycle.pop();
	    
	    if((*(ch.weight)) > e_Max.weight) {
	        ToVertex pa = cycle.top();
	        e_Max.weight = *(ch.weight);
	        e_Max.v1 = pa.to;
	        e_Max.v2 = ch.to;
	    }
	}
	
	if(e_Max.weight > weight) {
	    g.removeEdge(e_Max);
	    g.addEdge(Edge(u, v, weight));
	}

    return g.totalWeight;
}

int main(int argc, char *argv[]) {

	/*
	1. inputs: graph file, change file, name of output file
	2. parseEdges to parse graph file
	3. calculate MST (returns integer, weight of MST); we print this integer to the output file
	4. loop through change file, call function pass in new edge and MST

	*/

	if (argc < 4) {
		cout << "Usage: " << argv[0] << " <graph_file> <change_file> <output_file>" << endl;
		return 1;
	}

	string graph_file = argv[1]; 
	string change_file = argv[2];
	string output_file = argv[3];

	ofstream output;
	output.open(output_file);
	
	ofstream coutFile;
	coutFile.open("out_perf", ios::app);

	//Write this function to parse edges from graph file to create your graph object
	Graph G = parseEdges(graph_file);
	coutFile << G.numberOfVertices << "\t";
	coutFile << G.numberOfEdges << "\t";

	//Run your MST function on graph G and collect as output the total weight of the MST
	clock_t startMST = clock();
	int MSTweight = computeMST(G);
	clock_t endMST = clock();
    //outputGraph(output, G);

	//Subtract the start time from the finish time to get the actual algorithm running time
	clock_t totalTime = 1000 * (endMST - startMST) / (float) CLOCKS_PER_SEC;

	//Write initial MST weight and time to output file
	output << MSTweight << " " << totalTime << endl;
	coutFile << totalTime << "\t";

	//Iterate through changes file
	ifstream changes(change_file);
	
	int newMSTWeight = -1;
	
	if (changes.is_open()) {
		int numChanges;
		changes >> numChanges; //read number of changes

		int counter = 0;
		clock_t totalNewMST_sum = 0;
		while (counter < numChanges) {
			int u, v, weight;
			changes >> u; //read u
			changes >> v; //read v
			changes >> weight; //read w(u,v)

			//Run your recomputeMST function to recalculate the new weight of the MST given the addition of this new edge
			//Note: you are responsible for maintaining the MST in order to update the cost without recalculating the entire MST
			clock_t startNewMST = clock();
			newMSTWeight = recomputeMST(u, v, weight, G);
			clock_t endNewMST = clock();
			//outputGraph(output, G);
			
			clock_t totalNewMST = 1000 * (endNewMST - startNewMST) / (float) CLOCKS_PER_SEC;
			totalNewMST_sum += totalNewMST;

			//Write new weight and time to output file
			output << newMSTWeight << " " << totalNewMST << endl;

			counter++;
		}
		coutFile << totalNewMST_sum << "\t";
		changes.close();
	}

	output.close();
	coutFile << "\n";
	coutFile.close();
	
	return 0;
}
