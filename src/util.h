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
#include <utility>

#define BnB "BnB"
#define APPROX "Approx"
#define LS1 "LS1"
#define LS2 "LS2"

//#define DEBUG 

using namespace std;

//Since we are searching the result with DFS(for branch and bound), so we use stack to store the result.
//FIXME : TBD
typedef stack<size_t> VCTYPE;

void copyVCOutOfOrder(VCTYPE & from, VCTYPE & to);
void outputVC(VCTYPE & vc);

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
    size_t activeNumberOfVertices;
    //the reason why we don't use matrix to store is that most of our testing graphs seem like very sparse.
    set<size_t> * vertices;

    //To store the sign of which vertices are active to avoid delete vertices derectly from Graph when search for result.
    //Active vertex mean not delete(vertices are deteled becasue 1. be pushed in to vertex cover set 2.its edges are all got covered)

    Graph(size_t a) :
    numberOfVertices(a), activeNumberOfVertices(a), numberOfEdges(0) {
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

    size_t degree(size_t vertexID) {
        if(vertexID >= numberOfVertices) {
            cout << "illegal vertex id" << endl;
            return 0;
        }

        return (vertices[vertexID]).size();
    }

    void removeVertex(size_t vertexID) {
        if(vertexID >= numberOfVertices) {
            cout << "illegal vertex id" << endl;
            return;
        }
        if (isDeleted(vertexID)) {
            return;
        }
        numberOfEdges -= vertices[vertexID].size();

        set<size_t>::iterator it;
        for (it=vertices[vertexID].begin(); it!=vertices[vertexID].end(); ++it) {
            // Remove the reverse edge
            vertices[*it].erase(vertexID);
        }
        activeNumberOfVertices--;
        vertices[vertexID].clear();
    }

    void addVertex(size_t vertexID, set<size_t> & newVertex) {
        if(vertexID >= numberOfVertices) {
            cout << "illegal vertex id" << endl;
            return ;
        }

        if(degree(vertexID) != 0) {
            cout << "adding an existing vertex" << endl;
            return ;
        }

        set<size_t>::iterator it;
        for (it=newVertex.begin(); it!=newVertex.end(); ++it) {
            addEdge(Edge(vertexID, (*it)));
        }
    }

    void addVertices(stack<pair<size_t, set<size_t>>> & verticesToAdd){
        while(!verticesToAdd.empty()) {
            pair<size_t, set<size_t>> ver = verticesToAdd.top();
            addVertex(ver.first, ver.second);
            verticesToAdd.pop();
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

    bool isDeleted(size_t vertexID) {
        return vertices[vertexID].size() == 0;
    }

    size_t findActiveVertexNumber() {
        size_t num = 0;
        for(size_t vertexID = 0; vertexID < numberOfVertices; vertexID ++) {
            if(degree(vertexID) > 0)
                num ++;
        }
        return num;
    }

    size_t findVertexWithBiggestDegree(size_t & maxDegreeVertexID) {
        size_t maxDegree = 0;
        maxDegreeVertexID = 0;
        for(size_t vertexID=0; vertexID < numberOfVertices; vertexID++) {
            if(degree(vertexID) > maxDegree) {
                maxDegree = degree(vertexID);
                maxDegreeVertexID = vertexID;
            }
        }
        return maxDegree;
    }

    //For MVC
    //For edges that one end with degree bigger than 1, and another end with degree equal to 1, the end with degree bigger than 1 would be put into the minimum vertices cover.
    //For edges that both ends' degree are equal to 1, any one of the end should be put into the minimum vertices cover.
    //Do the process until there is no vertex with one degree
    void screenOutPartOfVertices(VCTYPE & vc, stack<pair<size_t, set<size_t>>> & verticesDeleted) {
        while(true) {
            bool haveDegreeOne = false;
            for(size_t vertexID = 0; vertexID < numberOfVertices; vertexID ++) {
                if(degree(vertexID) == 1) {
                    haveDegreeOne = true;
                    size_t neighbor = *(vertices[vertexID].begin());
                    vc.push(neighbor);
                    verticesDeleted.push(make_pair(neighbor, set<size_t>(vertices[neighbor])));
                    removeVertex(neighbor);
                }
            }
            if(!haveDegreeOne)
                break;
        }
    }

    //For MVC
    //Getting one vertices cover
    //Based on always choose the vertex with biggest degree
    void getLowerBound(VCTYPE & vc) {
        stack<pair<size_t, set<size_t>>> verticesDeleted;
        while(numberOfEdges != 0) {
            size_t maxDegreeVertexID;
            findVertexWithBiggestDegree(maxDegreeVertexID);
            vc.push(maxDegreeVertexID);
            verticesDeleted.push(make_pair(maxDegreeVertexID, set<size_t>(vertices[maxDegreeVertexID])));
            removeVertex(maxDegreeVertexID);
        }

        addVertices(verticesDeleted);
    }
};

//input graph from file
Graph inputGraph(string graph_file);

//output graph
//For testing usage
void outputGraph(Graph & g, string discription = "");

//using dfs to find a cycle
void dfs_findMaxmiumCycle(stack<size_t> & cycle, Graph & g, size_t startVertexID);
