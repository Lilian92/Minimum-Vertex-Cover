#include "util.h" 

Graph inputGraph(string graph_file) {
    ifstream inputfile(graph_file);
    
    //init graph
    size_t numberOfVertices, numberOfEdges, unweighted;
    inputfile >> numberOfVertices;
    inputfile >> numberOfEdges;
    inputfile >> unweighted;
    
    Graph g(numberOfVertices, numberOfEdges);

    //input edges' info
    for(size_t v=0; v<g.numberOfVertices; v++) {
        string line;
        std::getline(inputfile, line);
        std::stringstream   linestream(line);

        size_t u;
        while(linestream >> u)
            addEdge(Edge(v, u))
    }

    inputfile.close();
    return g;
}

void outputGraph(ofstream & output, Graph & g) {
    output << "number of vertices : " << g.numberOfActiveVertices << endl;
    set<size_t>::iterator it;
    for(size_t i=0; i<g.numberOfVertices; i++) {
        if(g.activeSign[i] == 1) {
            output << "vertex " << i << endl;
            for (it=((g.vertices)[i]).begin(); it!=((g.vertices)[i]).end(); ++it) {
                if(s.activeSign[*(it)] == 1) {
                    output << *(it) << "\t";
                }
            }
            output << endl;
        }
    }
}

//FIXME
bool dfs_findMaximumCycle(stack<size_t> & cycle, Graph & g, size_t startVertexID) {
/*    bool * visited = new bool[g.numberOfVertices];
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
*/    
    return true;
}
