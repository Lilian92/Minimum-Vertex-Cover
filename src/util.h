#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream> 
#include <vector>
#include <string>
#include <cstring>
#include <ctime>
#include <fstream>
#include <sstream>
#include <queue>
#include <set>
#include <stack>

#define BnB "BnB"
#define APPROX "Approx"
#define LS1 "LS1"
#define LS2 "LS2"

using namespace std;

//Since we are searching the result with DFS(for branch and bound), so we use stack to store the result.
//FIXME
typedef stack<size_t> VCTYPE;

struct Edge {
    size_t v1;
    size_t v2;
    
    Edge( size_t a=0, size_t b=0 ):
    v1(a), v2(b) {}
};

//Graph
struct Graph {
    size_t numberOfVertices;
    size_t numberOfEdges;
    //the reason why we don't use matrix to store is that most of our testing graphs seem like very sparse.
    set<size_t> * vertices;

    //To store the sign of which vertices are active to avoid delete vertices derectly from Graph when search for result.
    //Active vertex mean not delete(vertices are deteled becasue 1. be pushed in to vertex cover set 2.its edges are all got covered)
    size_t numberOfActiveVertices;
    bool * activeSign;

    //FIXME: init active status is all active or not active
    Graph(size_t a, size_t b) :
    numberOfVertices(a), numberOfEdges(b) {
        vertices = new set<size_t>[numberOfVertices];
        for(size_t i=0; i<numberOfVertices; i++) {
            (vertices)[i] = set<size_t>();
        }

        numberOfActiveVertices = numberOfVertices;
        activeSign = new bool[numberOfVertices];
        memset(activeSign, 1, sizeof(bool)*numberOfVertices);
    }
    
    void removeEdge(Edge e) {
        if(e.v1 >= numberOfVertices || e.v2 >= numberOfVertices) {
            cout << "illegal edge" << endl;
            return ;
        }

        (vertices[e.v1]).erase(e.v2);
        (vertices[e.v2]).erase(e.v1);
    }

    void addEdge(Edge e) {
        if(e.v1 >= numberOfVertices || e.v2 >= numberOfVertices) {
            cout << "illegal edge" << endl;
            return ;
        }

        (vertices[e.v1]).insert(e.v2);
        (vertices[e.v2]).insert(e.v1);
    }

//    size_t degree(size_t vertex_id) {
//        if(vertex_id >= numberOfVertices) {
//            cout << "illegal vertex id" << endl;
//            return ;
//        }
//        return vertices[vertex_id].size();
//    }

    void unactiveVertex(size_t vertex_id) {
        if(vertex_id >= numberOfVertices) {
            cout << "illegal vertex id" << endl;
            return ;
        }
        if( activeSign[vertex_id] == 1 ) {
            activeSign[vertex_id] = 0;
            numberOfActiveVertices --;
        }
    }

    void activeVertex(size_t vertex_id) {
        if(vertex_id >= numberOfVertices) {
            cout << "illegal vertex id" << endl;
            return ;
        }
        if( activeSign[vertex_id] == 0 ) {
            activeSign[vertex_id] = 1;
            numberOfActiveVertices ++;
        }
    }

    bool findFirstActiveVertex(size_t & startVertexID) {
        startVertexID = 0;
        while((activeSign[startVertexID] == 0) && startVertexID < numberOfVertices)
            startVertexID ++;
        if(startVertexID == numberOfVertices)
            return false;
        else
            return true;
    }
};

//input graph from file
Graph inputGraph(string graph_file);

//output graph
//For testing usage
void outputGraph(ofstream & output, Graph & g);

//using dfs to find a cycle
void dfs_findMaxmiumCycle(stack<size_t> & cycle, Graph & g, size_t startVertexID);
