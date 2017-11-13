#include "util.h" 

void copyVCOutOfOrder(VCTYPE & from, VCTYPE & to){
    VCTYPE temp(from);
    while(!to.empty()) {
        to.pop();
    }

    while(!temp.empty()) {
        to.push(temp.top());
        temp.pop();
    }
}

void outputVC(VCTYPE & vc) {
    VCTYPE temp(vc);
    while(!temp.empty()) {
        cout << temp.top() << "\t";
        temp.pop();
    }
    cout << "\n";
}

Graph inputGraph(string graph_file) {
    ifstream inputfile(graph_file);
    
    //init graph
    size_t numberOfVertices, numberOfEdges, unweighted;
    inputfile >> numberOfVertices;
    inputfile >> numberOfEdges;
    inputfile >> unweighted;
    
    Graph g(numberOfVertices);

    //input edges' info
    string line;
    std::getline(inputfile, line);
    std::stringstream   linestream(line);

    for(size_t v=1; v<=g.numberOfVertices; v++) {
        string line;
        std::getline(inputfile, line);
        std::stringstream   linestream(line);

        size_t u;
        while(linestream >> u) {
            g.addEdge(Edge(v-1, u-1));
        }
    }

    inputfile.close();
    return g;
}

void outputGraph(Graph & g) {
    cout << "number of vertices : " << g.numberOfVertices << endl;
    set<size_t>::iterator it;
    for(size_t i=0; i<g.numberOfVertices; i++) {
        if(g.degree(i) > 0) {
            cout << "vertex ID " << i << " with degree " << g.degree(i) << endl;
            for (it=((g.vertices)[i]).begin(); it!=((g.vertices)[i]).end(); ++it) {
                cout << *(it) << "\t";
            }
            cout << endl;
        }
    }
}

//FIXME
/*bool dfs_findMaximumCycle(stack<size_t> & cycle, Graph & g, size_t startVertexID) {
    bool * visited = new bool[g.numberOfVertices];
    for(size_t i=0; i<g.numberOfVertices; i++)
        visited[i] = false;

    if(g.activeSign[startVertexID] == 0) {
    }
        
    cycle.push(size_t(startVertexID, new size_t(0)));
    visited[startVertexID] = true;
    while(cycle.size() != 0) {
        size_t visitVertex = cycle.top();
        size_t vertexID = visitVertex.to;
        std::set<size_t>::iterator child;
        for(child = ((g.vercticesEdges)[vertexID]).begin(); child != ((g.vercticesEdges)[vertexID]).end(); child++) {
            size_t childVertexID = (*child).to;
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
    return true;
}
*/
