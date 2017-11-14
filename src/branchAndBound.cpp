#include "branchAndBound.h"

size_t lowerBoundOfMVC(Graph & g) {
    VCTYPE vc;
    g.getLowerBound(vc);
    return vc.size()/2;
}

void searchMVC(Graph g, VCTYPE & vc, VCTYPE & mvc) {
#ifdef DEBUG
    cout << "----------start searching" << g.numberOfEdges << " " << g.numberOfVertices << endl;
    outputGraph(g);
#endif

    if(g.numberOfEdges == 0) {
#ifdef DEBUG
        cout << "found a solution with" << vc.size() << endl;
#endif
        if(vc.size() < mvc.size()) {
            copyVCOutOfOrder(vc, mvc);
        }
        return ;
    }

    size_t originSizeOfVC = vc.size();

    stack<pair<size_t, set<size_t>>> verticesDeleted;
    g.screenOutPartOfVertices(vc, verticesDeleted);

#ifdef DEBUG
    cout << "***after screen out" << g.numberOfEdges << " " << g.numberOfVertices << endl;
    outputGraph(g);
#endif

    //start to search
    size_t maxDegreeVertexID;
    g.findVertexWithBiggestDegree(maxDegreeVertexID);

#ifdef DEBUG
    cout << "***start search maxDegree with " << maxDegreeVertexID << endl;
#endif

    /*
     * search the route with the vertex with largest degree
     * */
    vc.push(maxDegreeVertexID);
    //store and then delete vertex
    set<size_t> vertexWithMaxDegree(g.vertices[maxDegreeVertexID]);
    g.removeVertex(maxDegreeVertexID);
    if((lowerBoundOfMVC(g)+vc.size()) < mvc.size()) {
        searchMVC(g, vc, mvc);
    }
    //recover after search
    vc.pop();
    g.addVertex(maxDegreeVertexID, vertexWithMaxDegree);

#ifdef DEBUG
    cout << "***after search the maxde" << g.numberOfEdges << " " << g.numberOfVertices << endl;
    outputGraph(g);
#endif

    /*
     * search the route with adding all neighbors of the largest degree vertex
     * */
    set<size_t>::iterator it;
    //store and then delete vertices 
    size_t numberOfVerticesToAdd = g.degree(maxDegreeVertexID);
    for (it=g.vertices[maxDegreeVertexID].begin(); it!=g.vertices[maxDegreeVertexID].end(); ++it) {
        verticesDeleted.push(make_pair((*it), set<size_t>(g.vertices[*it])));
        g.removeVertex((*it));
        vc.push(*it);
    }
    if(lowerBoundOfMVC(g) < mvc.size()) {
        searchMVC(g, vc, mvc);
    }
    //recover after search
    while(numberOfVerticesToAdd--)
        vc.pop();
    g.addVertices(verticesDeleted);

    while(vc.size() > originSizeOfVC)
        vc.pop();

#ifdef DEBUG
    cout << "the end" << g.numberOfEdges << " " << g.numberOfVertices << endl;
    outputGraph(g);
#endif
}

void branchAndBound(Graph & g, VCTYPE & mvc) {
    //init process
    if(mvc.size() != 0) {
        cout << "In branch and bound: wrong start position to search" << endl;
        return;
    }

    stack<pair<size_t, set<size_t>>> verticesDelete;
    g.screenOutPartOfVertices(mvc, verticesDelete);

    VCTYPE vc(mvc);
    g.getLowerBound(mvc);
#ifdef DEBUG
    cout << "BAD, after init" << endl;
    //outputGraph(g);
#endif

    searchMVC(g, vc, mvc);
}
