#include "branchAndBound.h"

size_t lowerBoundOfMVC(Graph & g) {
    return g.findActiveVertexNumber()/2;
}

void searchMVC(Graph g, VCTYPE & vc, VCTYPE & mvc) {
    if(g.numberOfEdges == 0) {
        if(vc.size() < mvc.size()) {
            copyVCOutOfOrder(vc, mvc);
        }
        return ;
    }

    stack<pair<size_t, set<size_t>>> verticesDeleted;
    g.screenOutPartOfVertices(vc, verticesDeleted);

    //start to search
    size_t maxDegreeVertexID;
    g.findVertexWithBiggestDegree(maxDegreeVertexID);

    /*
     * search the route with the vertex with largest degree
     * */
    vc.push(maxDegreeVertexID);
    //store and then delete vertex
    set<size_t> vertexWithMaxDegree(g.vertices[maxDegreeVertexID]);
    g.removeVertex(maxDegreeVertexID);
    if(lowerBoundOfMVC(g) < mvc.size()) {
        searchMVC(g, vc, mvc);
    }
    //recover after search
    g.addVertex(maxDegreeVertexID, vertexWithMaxDegree);

    /*
     * search the route with adding all neighbors of the largest degree vertex
     * */
    set<size_t>::iterator it;
    //store and then delete vertices 
    for (it=g.vertices[maxDegreeVertexID].begin(); it!=g.vertices[maxDegreeVertexID].end(); ++it) {
        verticesDeleted.push(make_pair((*it), set<size_t>(g.vertices[*it])));
        g.removeVertex((*it));
    }
    if(lowerBoundOfMVC(g) < mvc.size()) {
        searchMVC(g, vc, mvc);
    }
    //recover after search
    g.addVertices(verticesDeleted);
}

void branchAndBound(Graph & g, VCTYPE & vc) {
    //init process
    if(vc.size() != 0) {
        cout << "In branch and bound: wrong start position to search" << endl;
        return;
    }

    stack<pair<size_t, set<size_t>>> verticesDelete;
    g.screenOutPartOfVertices(vc, verticesDelete);

    VCTYPE mvc(vc);
    g.getOneVerticesCover(mvc);
#ifdef DEBUG
    cout << "BAD, after init" << endl;
    outputGraph(g);
#endif
    

    searchMVC(g, vc, mvc);
}
