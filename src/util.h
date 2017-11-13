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

#define DEBUG 

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
    size_t numberOfVertices; //it is settled from the start and wouldn't change with update.
    size_t numberOfEdges; //can be used to judge the end of searching. That is all edges are deleted(covered).
    //the reason why we don't use matrix to store is that most of our testing graphs seem like very sparse.
    set<size_t> * vertices;

    //To store the sign of which vertices are active to avoid delete vertices derectly from Graph when search for result.
    //Active vertex mean not delete(vertices are deteled becasue 1. be pushed in to vertex cover set 2.its edges are all got covered)

    //FIXME: init active status is all active or not active
    Graph(size_t a) :
    numberOfVertices(a), numberOfEdges(0) {
        vertices = new set<size_t>[numberOfVertices];
        for(size_t i=0; i<numberOfVertices; i++) {
            (vertices)[i] = set<size_t>();
        }
    }
    
    void removeEdge(Edge e) {
        if(e.v1 >= numberOfVertices || e.v2 >= numberOfVertices || e.v1 == e.v2) {
            cout << "illegal edge" << endl;
            return ;
        }

        bool res = (vertices[e.v1]).erase(e.v2);
        res |= (vertices[e.v2]).erase(e.v1);

        // if that edge is removed, we need to reduce the number of edges
        numberOfEdges -= res;
    }

    void addEdge(Edge e) {
        if(e.v1 >= numberOfVertices || e.v2 >= numberOfVertices || e.v1 == e.v2) {
            cout << e.v1 << " " << e.v2 << "illegal edge" << endl;
            return ;
        }

        std::pair<std::set<size_t>::iterator, bool> ret1, ret2;
    
        ret1 = (vertices[e.v1]).insert(e.v2);
        ret2 = (vertices[e.v2]).insert(e.v1);

        numberOfEdges += (ret1.second | ret2.second);
    }

    size_t degree(size_t vertex_id) {
        if(vertex_id >= numberOfVertices) {
            cout << "illegal vertex id" << endl;
            return 0;
        }
        
        return (vertices[vertex_id]).size();
    }

    void removeVertex(size_t vertex_id) {
        if(vertex_id >= numberOfVertices) {
            cout << "illegal vertex id" << endl;
            return ;
        }

        set<size_t>::iterator it;
        for (it=vertices[vertex_id].begin(); it!=vertices[vertex_id].end(); ++it) {
            removeEdge(Edge(vertex_id, (*it)));
        }
    }

    void addVertex(set<size_t> & newVertex, size_t vertex_id) {
        if(vertex_id >= numberOfVertices) {
            cout << "illegal vertex id" << endl;
            return ;
        }

        if(degree(vertex_id) != 0) {
            cout << "adding an existing vertex" << endl;
            return ;
        }

        set<size_t>::iterator it;
        for (it=newVertex.begin(); it!=newVertex.end(); ++it) {
            addEdge(Edge(vertex_id, (*it)));
        }
    }

    bool findFirstActiveVertex(size_t & startVertexID) {
        startVertexID = 0;
        while((degree(startVertexID) == 0) && startVertexID < numberOfVertices)
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
void outputGraph(Graph & g);

//using dfs to find a cycle
void dfs_findMaxmiumCycle(stack<size_t> & cycle, Graph & g, size_t startVertexID);
